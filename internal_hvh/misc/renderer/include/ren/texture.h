#ifndef EV0LVE_RENDERER_TEXTURE_H
#define EV0LVE_RENDERER_TEXTURE_H

#include <memory>
#include <string>

#include <ren/types.h>

namespace evo::ren
{
	class adapter_base;

	class texture : public std::enable_shared_from_this<texture>
	{
		enum creation
		{
			c_none,
			c_file,
			c_mem,
			c_rgba
		};

		creation way{};

		struct
		{
			std::string 	str{};
			void* raw{};
			size_t 			a{};
			size_t 			b{};
			size_t 			c{};
		} creation_data;

	public:
		texture( void* tex ) : object( tex ) {} // WARNING: only use as wrapper for raw pushed textures for renderer where dimensions are irrelevant
		texture( const std::string& f );
		texture( void* tex, size_t sz );
		texture( void* tex, uint32_t w, uint32_t h, uint32_t p );

		~texture()
		{
			destroy();

			if ( creation_data.raw )
				free( creation_data.raw );
		}

		operator void* ( )
		{
			return object;
		}
		operator bool() const
		{
			return is_valid();
		}

		template<typename T>
		inline T* as()
		{
			return reinterpret_cast< T* >( object );
		}

		inline bool is_valid() const
		{
			return object != nullptr;
		}

		ren::vec2 size() const
		{
			return way == c_rgba ? vec2( creation_data.a, creation_data.b ) : vec2( creation_data.b, creation_data.c );
		}

		void create();
		void destroy();

	private:
		void* object{};
		std::shared_ptr<adapter_base> adp{}; // might need you later m8
	};
}

#endif //EV0LVE_RENDERER_TEXTURE_H
