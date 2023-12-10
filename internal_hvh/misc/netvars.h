#pragma once
#include "datamap.h"

struct DVariant
{
	union
	{
		float m_Float;
		long m_Int;
		char* m_pString;
		void* m_pData;
		Vector m_Vector;
		int64_t m_Int64;
	};

	int m_Type;
};

using RecvVarProxyFn = void( __cdecl* )( void*, void*, void* );

enum SendPropType : int
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array,
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes
};


struct RecvProp;
struct RecvTable
{
	RecvProp* m_pProps;
	int m_nProps;
	void* m_pDecoder;
	char* m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

struct RecvProp
{
	char* m_pVarName;
	int m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	const void* m_pExtraData;
	RecvProp* m_pArrayProp;
	void* m_ArrayLengthProxy;
	RecvVarProxyFn m_ProxyFn;
	void* m_DataTableProxyFn;
	RecvTable* m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char* m_pParentArrayPropName;
};

struct CRecvProxyData
{
	const RecvProp* m_pRecvProp;
	DVariant m_Value;
	int m_iElement;
	int m_ObjectID;
};

//===============================================
#define PNETVAR(funcname, type, netvarname) __forceinline type* funcname() \
{ \
	return reinterpret_cast<type*>( uintptr_t( this ) + server_offset_multi( netvarname ) ); \
}
//===============================================
#define ANETVAR(funcname, type, num, netvarname) __forceinline std::array<type, num>& funcname() \
{ \
	return *reinterpret_cast<std::array<type, num>*>( uintptr_t( this ) + server_offset_multi( netvarname ) ); \
}
//===============================================
#define NETVAR(funcname, type, netvarname) type& funcname() \
{ \
	return *reinterpret_cast<type*>( uintptr_t( this ) + server_offset_multi( netvarname ) ); \
}
//===============================================
#define NETVARA(funcname, type, netvarname, add) type& funcname() \
{ \
	return *reinterpret_cast<type*>( uintptr_t( this ) + server_offset_multi( netvarname ) + add ); \
}
//===============================================
#define NETVARRS(funcname, type, netvarname) type& funcname( int index, int offset = 4 ) \
{ \
	return *reinterpret_cast<type*>( uintptr_t( this ) + server_offset_multi( netvarname ) + index * offset ); \
}
//===============================================
#define OFFSET(funcname, type, offset) type& funcname() \
{ \
	return *reinterpret_cast<type*>(uintptr_t(this) + offset ); \
}
//===============================================
#define POFFSET(funcname, type, offset) type* funcname() \
{ \
	return reinterpret_cast<type*>(uintptr_t(this) + offset ); \
}
//===============================================
#define AOFFSET(funcname, type, num, offset) std::array<type, num>& funcname() \
{ \
	return *reinterpret_cast<std::array<type, num>*>( uintptr_t( this ) + offset ); \
}
//===============================================
#define APOFFSET(funcname, type, num, offset) std::array<type, num>& funcname() \
{ \
	return **reinterpret_cast<std::array<type, num>**>( uintptr_t( this ) + offset ); \
}
//===============================================
#define OFFSETRS(funcname, type, offset) type& funcname( int index ) \
{ \
	return *reinterpret_cast<type*>( uintptr_t( this ) + offset + index * 4 ); \
}
