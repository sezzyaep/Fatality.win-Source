#pragma once

#include <cstdint>
#include <cstdio>

#define FI __forceinline

namespace vm
{
	enum instr_t
	{
		S64,
		S32,

		DECR,
		INCR,
		// memory operations
		// stack
		PUSHI,
		PUSHR,
		POP,

		// registers_
		LOADR,
		MOVRR,
		MOVBRR,
		MOVCRR,
		DEREFRR,
		DEREFBRR,

		// arithmetic
		ADD,
		ADDRR,
		SUB,
		SUBRR,
		MUL,
		MULRR,
		DIV,
		DIVRR,
		MOD,
		MODRR,
		XOR,
		XORRR,
		AND,
		ANDRR,

		SAL,
		SAR,

		// control flow
		JERR,  // jump if reg eq reg
		JNERR, // jump if reg not eq reg
		JE,    // jump if last 2 values on the stack are equal
		JNE,   // jump if last 2 values on the stack are not equal
		JGRR,
		JG, // jump if greater
		JGERR,
		JGE, // jump if greater or equal
		JLRR,
		JL, // jump if less
		JLERR,
		JLE, // jump if less or equal

		PUSHPC, // push program counter
		RET,

		VMEXIT, // pause the program and await the next call
		VMCALL, // call a function and pass the pointer of the vm state to it
	};

	using vmcall_t = void( struct vm_t* vm );

	template <typename t, size_t size> struct fixed_stack_t
	{
		t elements_[ size ];
		size_t index_{};

		FI void push( t const& v ) { elements_[ index_++ ] = v; }

		FI t& pop() { return elements_[ --index_ ]; }
	};

	struct vm_t
	{
		uint8_t* code_{};
		int32_t code_size_{};
		uint32_t ip_{};

		fixed_stack_t<uint32_t, 2048> stack_{};
		uint32_t registers_[ 256 ]{};
		uint64_t registers64_[ 256 ]{};
		bool use_64_{};

		void run();
	};

	FI void vm_t::run()
	{
		for ( ; ip_ < code_size_;)
		{
			// fetch the instruction
			switch ( code_[ ip_ ] )
			{
				case DECR:
					if ( use_64_ )
						--registers64_[ code_[ ip_ + 1 ] ];
					else
						--registers_[ code_[ ip_ + 1 ] ];
					ip_ += 2;
					break;

				case INCR:
					if ( use_64_ )
						++registers64_[ code_[ ip_ + 1 ] ];
					else
						++registers_[ code_[ ip_ + 1 ] ];
					ip_ += 2;
					break;

				case S32:
					use_64_ = false;
					ip_ += 1;
					break;

				case S64:
					use_64_ = true;
					ip_ += 1;
					break;
				case PUSHI:
					if ( use_64_ )
					{
						stack_.push( *( uint64_t* ) ( &code_[ ip_ + 1 ] ) );
						ip_ += 9;
						break;
					}
					stack_.push( *( uint32_t* ) ( &code_[ ip_ + 1 ] ) );
					ip_ += 5; // advance 5 bytes in the code.
					break;

				case PUSHR:
					if ( use_64_ )
						stack_.push( static_cast< uint32_t >( registers64_[ code_[ ip_ + 1 ] ] ) );
					else
						stack_.push( registers_[ code_[ ip_ + 1 ] ] );
					ip_ += 2;
					break;

				case POP:
					registers_[ code_[ ip_ + 1 ] ] = stack_.pop();
					ip_ += 2;
					break;

				case LOADR:
					if ( use_64_ )
						registers64_[ code_[ ip_ + 1 ] ] = ( *( uint64_t* ) ( &code_[ ip_ + 2 ] ) );
					else
						registers_[ code_[ ip_ + 1 ] ] = ( *( uint32_t* ) ( &code_[ ip_ + 2 ] ) );
					ip_ += use_64_ ? 10 : 6;
					break;

				case MOVRR:
					if ( use_64_ )
						registers64_[ code_[ ip_ + 1 ] ] = registers64_[ code_[ ip_ + 2 ] ];
					else
						registers_[ code_[ ip_ + 1 ] ] = registers_[ code_[ ip_ + 2 ] ];
					ip_ += 3;
					break;

				case MOVBRR:
					if ( use_64_ )
						registers64_[ code_[ ip_ + 1 ] ] = static_cast< uint8_t >( registers64_[ code_[ ip_ + 2 ] ] );
					else
						registers_[ code_[ ip_ + 1 ] ] = static_cast< uint8_t >( registers_[ code_[ ip_ + 2 ] ] );
					ip_ += 3;
					break;

				case MOVCRR: // implicitly supported in translator
					if ( use_64_ )
						registers64_[ code_[ ip_ + 1 ] ] = registers_[ code_[ ip_ + 2 ] ];
					else
						registers_[ code_[ ip_ + 1 ] ] = static_cast< uint32_t >( registers64_[ code_[ ip_ + 2 ] ] );
					ip_ += 3;
					break;

				case DEREFRR:
					if ( use_64_ )
						registers64_[ code_[ ip_ + 1 ] ] = *( uint64_t* ) registers_[ code_[ ip_ + 2 ] ];
					else

						registers_[ code_[ ip_ + 1 ] ] = *( uint32_t* ) registers_[ code_[ ip_ + 2 ] ];
					ip_ += 3;
					break;

				case DEREFBRR:
					if ( use_64_ )
						registers64_[ code_[ ip_ + 1 ] ] = *( uint8_t* ) registers_[ code_[ ip_ + 2 ] ];
					else

						registers_[ code_[ ip_ + 1 ] ] = *( uint8_t* ) registers_[ code_[ ip_ + 2 ] ];
					ip_ += 3;
					break;

#define GENERIC_STACK_OP(name, op)                                                             \
				case name: {                                                                   \
					auto a = stack_.pop();                                                     \
					auto b = stack_.pop();                                                     \
					stack_.push( a op b );                                                     \
					ip_ += 1;                                                                  \
					break;                                                                     \
				}

#define GENERIC_REGISTER_OP(name, op)                                                          \
				case name: {                                                                   \
					if( use_64_ ) {                                                            \
						auto& a = registers64_[ code_[ ip_ + 1 ] ];                            \
						auto& b = registers64_[ code_[ ip_ + 2 ] ];                            \
						a = a op b;                                                            \
						ip_ += 3;                                                              \
						break;                                                                 \
					}                                                                          \
					auto &a = registers_[ code_[ ip_ + 1 ] ];                                  \
					auto &b = registers_[ code_[ ip_ + 2 ] ];                                  \
					a = a op b;                                                                \
					ip_ += 3;                                                                  \
					break;                                                                     \
				}

					GENERIC_STACK_OP( ADD, +);
					GENERIC_STACK_OP( SUB, -);
					GENERIC_STACK_OP( MUL, * );
					GENERIC_STACK_OP( DIV, / );
					GENERIC_STACK_OP( MOD, % );
					GENERIC_STACK_OP( XOR, ^ );
					GENERIC_STACK_OP( AND, & );

					GENERIC_REGISTER_OP( ADDRR, +);
					GENERIC_REGISTER_OP( SUBRR, -);
					GENERIC_REGISTER_OP( MULRR, * );
					GENERIC_REGISTER_OP( DIVRR, / );
					GENERIC_REGISTER_OP( MODRR, % );
					GENERIC_REGISTER_OP( XORRR, ^ );
					GENERIC_REGISTER_OP( ANDRR, & );

#undef GENERIC_REGISTER_OP
#undef GENERIC_STACK_OP

				case SAL:
					if ( use_64_ )
						registers64_[ code_[ ip_ + 1 ] ] <<= code_[ ip_ + 2 ];
					else

						registers_[ code_[ ip_ + 1 ] ] <<= code_[ ip_ + 2 ];
					ip_ += 3;
					break;
				case SAR:
					if ( use_64_ )
						registers64_[ code_[ ip_ + 1 ] ] >>= code_[ ip_ + 2 ];
					else

						registers_[ code_[ ip_ + 1 ] ] >>= code_[ ip_ + 2 ];
					ip_ += 3;
					break;

#define REGISTER_CFLOW_OP(name, op)                                                            \
				case name: {                                                                   \
					if( use_64_ ) {                                                            \
						auto& a = registers64_[ code_[ ip_ + 1 ] ];                            \
						auto& b = registers64_[ code_[ ip_ + 2 ] ];                            \
						if (a op b)                                                            \
							ip_ = ( *( uint32_t* ) ( &code_[ ip_ ] + 3 ) );                    \
						else                                                                   \
							ip_ += 7;                                                          \
						break;                                                                 \
					}                                                                          \
					auto &a = registers_[ code_[ ip_ + 1 ] ];                                  \
					auto &b = registers_[ code_[ ip_ + 2 ] ];                                  \
					if ( a op b )                                                              \
						ip_ = ( *( uint32_t * )( &code_[ ip_ ] + 3 ) );                        \
					else                                                                       \
						ip_ += 7;                                                              \
					break;                                                                     \
				}

#define STACK_CFLOW_OP(name, op)                                                               \
				case name: {                                                                   \
					auto a = stack_.pop();                                                     \
					auto b = stack_.pop();                                                     \
					if ( a op b )                                                              \
						ip_ = ( *( uint32_t* )( &code_[ ip_ ] + 1 ) );                         \
					else                                                                       \
						ip_ += 5;                                                              \
					break;                                                                     \
				}

					REGISTER_CFLOW_OP( JERR, == );
					REGISTER_CFLOW_OP( JNERR, != );
					REGISTER_CFLOW_OP( JGRR, > );
					REGISTER_CFLOW_OP( JGERR, >= );
					REGISTER_CFLOW_OP( JLRR, < );
					REGISTER_CFLOW_OP( JLERR, <= );

					STACK_CFLOW_OP( JE, == );
					STACK_CFLOW_OP( JNE, != );
					STACK_CFLOW_OP( JG, > );
					STACK_CFLOW_OP( JGE, >= );
					STACK_CFLOW_OP( JL, < );
					STACK_CFLOW_OP( JLE, <= );

#undef STACK_CFLOW_OP
#undef REGISTER_CFLOW_OP

				case PUSHPC:
					stack_.push( ip_ );
					ip_ += 1;
					break;

				case RET:
					ip_ = stack_.pop();
					break;

				case VMEXIT:
					ip_ += 1;
					return;

				case VMCALL:
					auto fn = ( vmcall_t* ) stack_.pop();
					fn( this );
					ip_ += 1;
					break;
			}
		}
	}

	template <size_t size> struct code_builder_t
	{
		fixed_stack_t<uint8_t, size> code_;

		FI void i( instr_t inst ) { code_.push( inst ); }

		FI void v8( uint8_t v ) { code_.push( v ); }

		FI void v32( uint32_t v )
		{
			*( uint32_t* ) ( &code_.elements_[ code_.index_ ] ) = v;
			code_.index_ += 4;
		}

		FI void v64( uint32_t v )
		{
			*( uint64_t* ) ( &code_.elements_[ code_.index_ ] ) = v;
			code_.index_ += 8;
		}
	};
} // namespace vm

#undef FI