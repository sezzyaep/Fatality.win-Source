#include "../include_cheat.h"

NTSTATUS NTAPI hook::nt_query_virtual_memory( HANDLE process_handle, PVOID base_address, int memory_information_class, PMEMORY_BASIC_INFORMATION mem_information, ULONG length, PULONG result_length )
{
	const auto status = hook_list[ reinterpret_cast< uintptr_t >( nt_query_virtual_memory ) ].get_original<decltype( &nt_query_virtual_memory )>()( process_handle, base_address, memory_information_class, mem_information, length, result_length );
	if ( !status && !memory_information_class && mem_information && mem_information->AllocationBase == globals::module_base )
		mem_information->Type = MEM_IMAGE;

	return status;
}

int32_t __stdcall hook::get_module_handle_ex_a( uint32_t flags, const char* str, uint32_t* mod )
{
	uintptr_t** _ebp;
	__asm mov _ebp, ebp;
	auto& ret_addr = _ebp[ 1 ];

	if ( *ret_addr == 0x2374c085 )
	{
		ret_addr = reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( ret_addr ) + 0x47 );
		return 1;
	}

	return original( get_module_handle_ex_a )( flags, str, mod );
}

int __fastcall hook::particle_draw_model( void* ecx, void* edx, int flags, const RenderableInstance_t& instance )
{
	const auto brightness = 1.f + ( !vars::visuals.dark->get<bool>() ? 0.f : vars::visuals.darkness->get<float>() * 0.01f );
	const Vector def( brightness, brightness, brightness );

	CMatRenderContextPtr ctx( interfaces::material_system()( )->GetRenderContext() );
	const auto org = ctx->GetToneMappingScaleLinear();
	ctx->SetToneMappingScaleLinear( def );

	const auto ret = original( particle_draw_model )( ecx, edx, flags, instance );

	ctx->SetToneMappingScaleLinear( org );
	ctx.SafeRelease();

	return ret;
}