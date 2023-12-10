#pragma once

#define JM_XORSTR_DISABLE_AVX_INTRINSICS
#if defined(_M_ARM64) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
#include <arm_neon.h>
#elif defined(_M_X64) || defined(__amd64__) || defined(_M_IX86) || defined(__i386__)
#include <immintrin.h>
#else
#error Unsupported platform
#endif

#include <cstdint>
#include <cstddef>
#include <utility>
#include <type_traits>
#include <random>

#ifdef _MSC_VER
#define XORSTR_FORCEINLINE __forceinline
#else
#define XORSTR_FORCEINLINE __attribute__((always_inline)) inline
#endif

#if defined(__clang__) || defined(__GNUC__)
#define JM_XORSTR_LOAD_FROM_REG(x) ::jm::detail::load_from_reg(x)
#else
#define JM_XORSTR_LOAD_FROM_REG(x) (x)
#endif

namespace jm
{

    namespace detail
    {

        template<std::size_t Size>
        XORSTR_FORCEINLINE constexpr std::size_t _buffer_size()
        {
            return ( ( Size / 16 ) + ( Size % 16 != 0 ) ) * 2;
        }

        template<std::uint32_t Seed>
        XORSTR_FORCEINLINE constexpr std::uint32_t key4() noexcept
        {
            std::uint32_t value = Seed;
            for ( char c : __TIME__ )
                value = static_cast< std::uint32_t >( ( value ^ c ) * 16777619ull );
            return value;
        }

        template<std::size_t S>
        XORSTR_FORCEINLINE constexpr std::uint64_t key8()
        {
            constexpr auto first_part = key4<2166136261 + S>();
            constexpr auto second_part = key4<first_part>();
            return ( static_cast< std::uint64_t >( first_part ) << 32 ) | second_part;
        }

        // loads up to 8 characters of string into uint64 and xors it with the key
        template<std::size_t N, class CharT>
        XORSTR_FORCEINLINE constexpr std::uint64_t
            load_xored_str8( std::uint64_t key, std::size_t idx, const CharT* str ) noexcept
        {
            using cast_type = typename std::make_unsigned<CharT>::type;
            constexpr auto value_size = sizeof( CharT );
            constexpr auto idx_offset = 8 / value_size;

            std::uint64_t value = key;
            for ( std::size_t i = 0; i < idx_offset && i + idx * idx_offset < N; ++i )
                value ^=
                ( std::uint64_t{ static_cast< cast_type >( str[ i + idx * idx_offset ] ) }
            << ( ( i % idx_offset ) * 8 * value_size ) );

            return value;
        }

        // forces compiler to use registers instead of stuffing constants in rdata
        XORSTR_FORCEINLINE std::uint64_t load_from_reg( std::uint64_t value ) noexcept
        {
#if defined(__clang__) || defined(__GNUC__)
            asm( "" : "=r"( value ) : "0"( value ) : );
#endif
            return value;
        }

        template<std::uint64_t V>
        struct uint64_v
        {
            constexpr static std::uint64_t value = V;
        };

    } // namespace detail

    template<class CharT, std::size_t Size, class Keys, class Indices>
    class xor_string;

    template<class CharT, std::size_t Size, std::uint64_t... Keys, std::size_t... Indices>
    class xor_string<CharT, Size, std::integer_sequence<std::uint64_t, Keys...>, std::index_sequence<Indices...>>
    {
#ifndef JM_XORSTR_DISABLE_AVX_INTRINSICS
        constexpr static inline std::uint64_t alignment = ( ( Size > 16 ) ? 32 : 16 );
#else
        constexpr static inline std::uint64_t alignment = 16;
#endif

        alignas( alignment ) std::uint64_t _storage[ sizeof...( Keys ) ];

    public:
        using value_type = CharT;
        using size_type = std::size_t;
        using pointer = CharT*;
        using const_pointer = const CharT*;

        template<class L>
        XORSTR_FORCEINLINE xor_string( L l, std::integral_constant<std::size_t, Size>, std::index_sequence<Indices...> ) noexcept
            : _storage{ JM_XORSTR_LOAD_FROM_REG( detail::uint64_v<detail::load_xored_str8<Size>( Keys, Indices, l() )>::value )... }
        {}

        XORSTR_FORCEINLINE constexpr size_type size() const noexcept
        {
            return Size - 1;
        }

        XORSTR_FORCEINLINE void crypt() noexcept
        {
            // everything is inlined by hand because a certain compiler with a certain linker is _very_ slow
#if defined(__clang__)
            alignas( alignment )
                std::uint64_t arr[]{ JM_XORSTR_LOAD_FROM_REG( Keys )... };
            std::uint64_t* keys =
                ( std::uint64_t* ) JM_XORSTR_LOAD_FROM_REG( ( std::uint64_t ) arr );
#else
            alignas( alignment ) std::uint64_t keys[]{ JM_XORSTR_LOAD_FROM_REG( Keys )... };
#endif

#if defined(_M_ARM64) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
#if defined(__clang__)
            ( ( Indices >= sizeof( _storage ) / 16 ? static_cast< void >( 0 ) : __builtin_neon_vst1q_v(
                reinterpret_cast< uint64_t* >( _storage ) + Indices * 2,
                veorq_u64( __builtin_neon_vld1q_v( reinterpret_cast< const uint64_t* >( _storage ) + Indices * 2, 51 ),
                __builtin_neon_vld1q_v( reinterpret_cast< const uint64_t* >( keys ) + Indices * 2, 51 ) ),
                51 ) ), ... );
#else // GCC, MSVC
            ( ( Indices >= sizeof( _storage ) / 16 ? static_cast< void >( 0 ) : vst1q_u64(
                reinterpret_cast< uint64_t* >( _storage ) + Indices * 2,
                veorq_u64( vld1q_u64( reinterpret_cast< const uint64_t* >( _storage ) + Indices * 2 ),
                vld1q_u64( reinterpret_cast< const uint64_t* >( keys ) + Indices * 2 ) ) ) ), ... );
#endif
#elif !defined(JM_XORSTR_DISABLE_AVX_INTRINSICS)
            ( ( Indices >= sizeof( _storage ) / 32 ? static_cast< void >( 0 ) : _mm256_store_si256(
                reinterpret_cast< __m256i* >( _storage ) + Indices,
                _mm256_xor_si256(
                _mm256_load_si256( reinterpret_cast< const __m256i* >( _storage ) + Indices ),
                _mm256_load_si256( reinterpret_cast< const __m256i* >( keys ) + Indices ) ) ) ), ... );

            if constexpr ( sizeof( _storage ) % 32 != 0 )
                _mm_store_si128(
                    reinterpret_cast< __m128i* >( _storage + sizeof...( Keys ) - 2 ),
                    _mm_xor_si128( _mm_load_si128( reinterpret_cast< const __m128i* >( _storage + sizeof...( Keys ) - 2 ) ),
                _mm_load_si128( reinterpret_cast< const __m128i* >( keys + sizeof...( Keys ) - 2 ) ) ) );
#else
            ( ( Indices >= sizeof( _storage ) / 16 ? static_cast< void >( 0 ) : _mm_store_si128(
                reinterpret_cast< __m128i* >( _storage ) + Indices,
                _mm_xor_si128( _mm_load_si128( reinterpret_cast< const __m128i* >( _storage ) + Indices ),
                _mm_load_si128( reinterpret_cast< const __m128i* >( keys ) + Indices ) ) ) ), ... );
#endif
        }

        XORSTR_FORCEINLINE const_pointer get() const noexcept
        {
            return reinterpret_cast< const_pointer >( _storage );
        }

        XORSTR_FORCEINLINE pointer get() noexcept
        {
            return reinterpret_cast< pointer >( _storage );
        }

        XORSTR_FORCEINLINE pointer crypt_get() noexcept
        {
            // crypt() is inlined by hand because a certain compiler with a certain linker is _very_ slow
#if defined(__clang__)
            alignas( alignment )
                std::uint64_t arr[]{ JM_XORSTR_LOAD_FROM_REG( Keys )... };
            std::uint64_t* keys =
                ( std::uint64_t* ) JM_XORSTR_LOAD_FROM_REG( ( std::uint64_t ) arr );
#else
            alignas( alignment ) std::uint64_t keys[]{ JM_XORSTR_LOAD_FROM_REG( Keys )... };
#endif

#if defined(_M_ARM64) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
#if defined(__clang__)
            ( ( Indices >= sizeof( _storage ) / 16 ? static_cast< void >( 0 ) : __builtin_neon_vst1q_v(
                reinterpret_cast< uint64_t* >( _storage ) + Indices * 2,
                veorq_u64( __builtin_neon_vld1q_v( reinterpret_cast< const uint64_t* >( _storage ) + Indices * 2, 51 ),
                __builtin_neon_vld1q_v( reinterpret_cast< const uint64_t* >( keys ) + Indices * 2, 51 ) ),
                51 ) ), ... );
#else // GCC, MSVC
            ( ( Indices >= sizeof( _storage ) / 16 ? static_cast< void >( 0 ) : vst1q_u64(
                reinterpret_cast< uint64_t* >( _storage ) + Indices * 2,
                veorq_u64( vld1q_u64( reinterpret_cast< const uint64_t* >( _storage ) + Indices * 2 ),
                vld1q_u64( reinterpret_cast< const uint64_t* >( keys ) + Indices * 2 ) ) ) ), ... );
#endif
#elif !defined(JM_XORSTR_DISABLE_AVX_INTRINSICS)
            ( ( Indices >= sizeof( _storage ) / 32 ? static_cast< void >( 0 ) : _mm256_store_si256(
                reinterpret_cast< __m256i* >( _storage ) + Indices,
                _mm256_xor_si256(
                _mm256_load_si256( reinterpret_cast< const __m256i* >( _storage ) + Indices ),
                _mm256_load_si256( reinterpret_cast< const __m256i* >( keys ) + Indices ) ) ) ), ... );

            if constexpr ( sizeof( _storage ) % 32 != 0 )
                _mm_store_si128(
                    reinterpret_cast< __m128i* >( _storage + sizeof...( Keys ) - 2 ),
                    _mm_xor_si128( _mm_load_si128( reinterpret_cast< const __m128i* >( _storage + sizeof...( Keys ) - 2 ) ),
                _mm_load_si128( reinterpret_cast< const __m128i* >( keys + sizeof...( Keys ) - 2 ) ) ) );
#else
            ( ( Indices >= sizeof( _storage ) / 16 ? static_cast< void >( 0 ) : _mm_store_si128(
                reinterpret_cast< __m128i* >( _storage ) + Indices,
                _mm_xor_si128( _mm_load_si128( reinterpret_cast< const __m128i* >( _storage ) + Indices ),
                _mm_load_si128( reinterpret_cast< const __m128i* >( keys ) + Indices ) ) ) ), ... );
#endif

            return ( pointer ) ( _storage );
        }
    };

    template<class L, std::size_t Size, std::size_t... Indices>
    xor_string( L l, std::integral_constant<std::size_t, Size>, std::index_sequence<Indices...> )->xor_string<
        std::remove_const_t<std::remove_reference_t<decltype( l()[ 0 ] )>>,
        Size,
        std::integer_sequence<std::uint64_t, detail::key8<Indices>()...>,
        std::index_sequence<Indices...>>;
} // namespace jm

namespace random
{
	constexpr auto time = __TIME__;
	constexpr auto seed = static_cast< unsigned >( time[ 7 ] ) + static_cast< unsigned >( time[ 6 ] ) * 10 + static_cast< unsigned >( time[ 4 ] ) * 60 + static_cast< unsigned >( time[ 3 ] ) * 600 + static_cast< unsigned >( time[ 1 ] ) * 3600 + static_cast< unsigned >( time[ 0 ] ) * 36000;

	template <int n>
	struct gen
	{
	private:
		static constexpr unsigned a = 16807;
		static constexpr unsigned m = 2147483647;

		static constexpr unsigned s = gen<n - 1>::value;
		static constexpr unsigned lo = a * ( s & 0xFFFFu );
		static constexpr unsigned hi = a * ( s >> 16u );
		static constexpr unsigned lo2 = lo + ( ( hi & 0x7FFFu ) << 16u );
		static constexpr unsigned hi2 = hi >> 15u;
		static constexpr unsigned lo3 = lo2 + hi;

	public:
		static constexpr unsigned max = m;
		static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
	};

	template <>
	struct gen<0>
	{
		static constexpr unsigned value = seed;
	};

	template <int n, int m>
	struct _int
	{
		static constexpr auto value = gen<n + 1>::value % m;
	};

    template <int n, uint32_t m>
    struct _uint
    {
        static constexpr auto value = gen<n + 1>::value % m;
    };

	template <int n>
	struct _char
	{
		static const char value = static_cast< char >( 1 + _int<n, 0x7F - 1>::value );
	};

    template <typename T, typename... Args>
    T in_range( Args&& ... args )
    {
        static std::mt19937 mt( std::random_device{}( ) );
        std::uniform_int_distribution<T> dist( std::forward<Args>( args )... );
        return dist( mt );
    }
}


template <size_t n, unsigned k>
struct xorstr
{
private:
	constexpr char enc( const char c ) const
	{
		return c ^ key; // NOLINT
	}

public:
	template <size_t... s>
	constexpr __forceinline xorstr( const char* str, std::index_sequence<s...> ) :
		key( random::_char<k>::value ), encrypted{ enc( str[ s ] )... }
	{
	}

	const size_t size = n;
	const char key;
	std::array<char, n + 1> encrypted;
};

#define RANDOM_SEED random::in_range<uint32_t>(0, 0xFFFFFFFF)

template<typename T = uintptr_t>
class encrypted_ptr
{
public:
    __forceinline encrypted_ptr()
    {
        this->value = this->cipher = RANDOM_SEED;
    }

    __forceinline explicit encrypted_ptr( T* const value )
    {
        *this = encrypted_ptr( uintptr_t( value ) );
    }

    __forceinline explicit encrypted_ptr( uintptr_t value )
    {
        this->cipher = RANDOM_SEED;
        this->value = uintptr_t( value ) ^ this->cipher;
    }

    __forceinline T* operator()() const
    {
        return operator->();
    }

    __forceinline T& operator*() const
    {
        return *reinterpret_cast< T* >( this->value ^ this->cipher );
    }

    __forceinline T* operator->() const
    {
        return reinterpret_cast< T* >( this->value ^ this->cipher );
    }

    __forceinline bool operator==( const encrypted_ptr& other ) const
    {
        return this->operator->() == other.operator->();
    }

    explicit __forceinline operator bool() const
    {
        return !this->operator!();
    }

    __forceinline bool operator!() const
    {
        return this->operator->() == nullptr;
    }

    __forceinline uintptr_t at( ptrdiff_t rel ) const
    {
        return uintptr_t( operator->() ) + rel;
    }

    __forceinline encrypted_ptr<T>& deref( const size_t amnt )
    {
        for ( auto i = 0u; i < amnt; i++ )
            *this = encrypted_ptr<T>( *reinterpret_cast< T** >( this->value ^ this->cipher ) );
        return *this;
    }

private:
    volatile uintptr_t value{};
    volatile uintptr_t cipher{};
};

template <typename t = int32_t>
t __forceinline add( t a, t b, t c )
{
    return ( ( ( a ^ b ) + ( ( a & b ) << 1 ) ) | c ) + ( ( ( a ^ b ) + ( ( a & b ) << 1 ) ) & c );
}

#define strfy(x) #x
#define concat_impl(x, y) x##y
#define concat(x, y) concat_impl(x, y)

#define _i(s) xorstr<sizeof(s) - 1, __COUNTER__>(s, std::make_index_sequence<sizeof(s) - 1>())

#define _w(s) []() noexcept -> std::string { \
    constexpr auto e = _i(s); \
    std::string res; \
    res += e.key; \
	for (size_t i = 0; i < e.size; i++) \
        res += e.encrypted[i]; \
    return res; \
}()

#define _w2(s) [&]() noexcept -> std::string { \
    auto key = rand() % 0x7F;\
    std::string res; \
    res += key; \
    for (size_t i = 0; i < (s).length(); i++) \
        res += (s)[i] ^ key; \
    return res; \
}()

template<typename t, t v>
struct constant_holder
{
	enum class val_holder : t
	{
		val = v
	};
};

#define _rt(n, s) volatile auto (n) = reinterpret_cast<char*>(_alloca(((s).size + 1) * sizeof(char))); \
    for (auto i = 0u; i < (s).size; i++) { \
        volatile auto temp = (s); \
        (n)[i] = (s).encrypted[i] ^ temp.key; } \
    (n)[(s).size] = '\0'


#define xorstr(str) ::jm::xor_string([]() { return str; }, std::integral_constant<std::size_t, sizeof(str) / sizeof(*str)>{}, std::make_index_sequence<::jm::detail::_buffer_size<sizeof(str)>()>{})
#define xorstr_(str) xorstr(str).crypt_get()

#ifdef _DEBUG
#define _(n, s) auto (n) = std::string(s)
#else
#define _(n, s) auto (n) = std::string(xorstr_(s))
#endif

#define _r(s) xorstr_(s)
#define _s(s) _(s, #s)

#define _u(n, s) \
    volatile auto concat(n, _char ) = reinterpret_cast<char*>(_alloca(((s).length()) * sizeof(char))); \
    for (auto i = 0; i < (int)(s).length() - 1; i++) \
		concat(n, _char )[ i ] = ( s )[ i + 1 ] ^ ( s )[ 0 ]; \
	auto (n) = std::string( concat(n, _char ), std::max((int)(s).length() - 1, 0) )

#define _wc(s) []() { auto str = std::string(xorstr_(s)); \
    return std::wstring(str.begin(), str.end()); }()


#define XOR_STR(s) xorstr_(s)
#define XOR(s) xorstr_(s)

template<typename T, T A, T B>
class xor_value
{
public:
    __forceinline static T get() { return value ^ cipher; }

private:
    volatile static const inline T value{ A ^ B }, cipher{ B };
};

template <size_t N, char K>
struct xor_str
{
private:
    static constexpr char enc( const char c ) { return c ^ K; }

public:
    template <size_t... s>
    constexpr __forceinline xor_str( const char* str, std::index_sequence<s...> )
        : encrypted{ enc( str[ s ] )... }
    {}

    __forceinline std::string dec()
    {
        std::string dec;
        dec.resize( N );

        for ( auto i = 0u; i < N; i++ )
            dec[ i ] = encrypted[ i ] ^ K;

        return dec;
    }

    __forceinline std::string ot( bool decrypt = true )
    {
        std::string dec;
        dec.resize( N );

        for ( auto i = 0u; i < N; i++ )
        {
            dec[ i ] = decrypt ? ( encrypted[ i ] ^ K ) : encrypted[ i ];
            encrypted[ i ] = '\0';
        }

        return dec;
    }

    volatile char encrypted[ N ];
};

#define XOR_32(val) (decltype(val))(xor_value<uint32_t, (uint32_t) val, random::_uint<__COUNTER__, 0xFFFFFFFF>::value>::get())

#ifndef RELEASE
#define sig(name, signature) []{ _(n, name); return util::get_sig(n, _w(signature)); }()
#define sig_rel(name, signature) []{ auto res = sig(name, signature); return *(uintptr_t*)(res + 0x1) + res + 0x5; }()
#endif


using fnv_t = unsigned;

static constexpr fnv_t runtime_prime = 16777619u;

__declspec( dllexport ) extern uint32_t runtime_basis;
__declspec( dllexport ) extern char server_address[ 255 ];
__declspec( dllexport ) extern char token[ 255 ];

inline uint32_t fnv1a_rt( const char* key )
{
	const char* data = const_cast< char* >( key );
	auto hash = runtime_basis;

#ifdef RELEASE
	hash ^= __readfsdword( 0x20 );
#endif
	for ( size_t i = 0; i < strlen( key ); ++i )
	{
		const uint8_t value = data[ i ];
		hash = hash ^ value;
		hash *= runtime_prime;
	}

	return hash;
}

inline uint32_t fnv1a_rt_tol( const char* key )
{
    const char* data = const_cast< char* >( key );
    auto hash = runtime_basis;

#ifdef RELEASE
    hash ^= __readfsdword( 0x20 );
#endif
    for ( size_t i = 0; i < strlen( key ); ++i )
    {
        const uint8_t value = std::tolower( data[ i ] );
        hash = hash ^ value;
        hash *= runtime_prime;
    }

    return hash;
}

inline uint32_t fnv1a_rt_tol( const std::string& key )
{
    auto hash = runtime_basis;

#ifdef RELEASE
    hash ^= __readfsdword( 0x20 );
#endif
    for ( size_t i = 0; i < key.length(); ++i )
    {
        const uint8_t value = std::tolower( key.c_str()[ i ] );
        hash = hash ^ value;
        hash *= runtime_prime;
    }

    return hash;
}

inline uint32_t fnv1a_rt( const std::string& key )
{
	auto hash = runtime_basis;

#ifdef RELEASE
	hash ^= __readfsdword( 0x20 );
#endif
	for ( size_t i = 0; i < key.length(); ++i )
	{
		const uint8_t value = key.c_str()[ i ];
		hash = hash ^ value;
		hash *= runtime_prime;
	}

	return hash;
}

constexpr uint32_t fnv1a_ct( const char* str, const uint32_t value = 2166136261u ) noexcept
{
	return !*str ? value : fnv1a_ct( str + 1, static_cast< unsigned >( 1ull *
		( value ^ static_cast< uint8_t >( *str ) ) * runtime_prime ) );
}

#define CONSTANT(value) ((decltype(value))constant_holder<decltype(value), value>::val_holder::val)
#define FNV1A(s) (CONSTANT(fnv1a_ct(s)))