#pragma once

#include <cstdint>
#include <Windows.h>
#include <winternl.h>
#include <ntstatus.h>

// ReSharper disable CppInconsistentNaming
#define FILE_OPEN 0x00000001
#define FILE_NON_DIRECTORY_FILE 0x00000040
#define FILE_OPEN_BY_FILE_ID 0x00002000

#define THREAD_CREATE_FLAGS_CREATE_SUSPENDED 0x00000001
#define THREAD_CREATE_FLAGS_SKIP_THREAD_ATTACH 0x00000002
#define THREAD_CREATE_FLAGS_HIDE_FROM_DEBUGGER 0x00000004
#define THREAD_CREATE_FLAGS_HAS_SECURITY_DESCRIPTOR 0x00000010
#define THREAD_CREATE_FLAGS_ACCESS_CHECK_IN_TARGET 0x00000020
#define THREAD_CREATE_FLAGS_INITIAL_THREAD 0x00000080

#define NtCurrentThread()         ((HANDLE)(LONG_PTR)-1)

typedef struct _API_SET_NAMESPACE
{
	ULONG Version;
	ULONG Size;
	ULONG Flags;
	ULONG Count;
	ULONG EntryOffset;
	ULONG HashOffset;
	ULONG HashFactor;
} API_SET_NAMESPACE, * PAPI_SET_NAMESPACE;

typedef struct _API_SET_NAMESPACE_ENTRY
{
	ULONG Flags;
	ULONG NameOffset;
	ULONG NameLength;
	ULONG HashedLength;
	ULONG ValueOffset;
	ULONG ValueCount;
} API_SET_NAMESPACE_ENTRY, * PAPI_SET_NAMESPACE_ENTRY;

using NtAllocateVirtualMemory = NTSTATUS( NTAPI* )( HANDLE process_handle, PVOID* base_address,
												 std::uint32_t* bits, size_t* size, std::uint32_t type, std::uint32_t access );

using NtReadVirtualMemory = NTSTATUS( NTAPI* )( HANDLE process_handle, PVOID base_address, PVOID buffer,
											  ULONG number_of_bytes_to_read, PULONG number_of_bytes_readed );

using NtWriteVirtualMemory = NTSTATUS( NTAPI* )( HANDLE process_handle, PVOID base_address, PVOID buffer,
											   ULONG number_of_bytes_to_write, PULONG number_of_bytes_written );

using NtFreeVirtualMemory = NTSTATUS( NTAPI* )( HANDLE process_handle, PVOID* base_address, PSIZE_T region_size, ULONG free_type );

using NtProtectVirtualMemory = NTSTATUS( NTAPI* )( HANDLE process_handle, PVOID* base_address, PULONG number_of_bytes_to_protect,
												ULONG new_access_protection, PULONG old_access_protection );

using NtYieldExecution = NTSTATUS( NTAPI* )( );

using NtCreateThreadEx = NTSTATUS( NTAPI* )( PHANDLE thread_handle, ACCESS_MASK desired_access, POBJECT_ATTRIBUTES object_attributes,
										  HANDLE process_handle, PVOID start_routine, PVOID argument, ULONG create_flags,
										  ULONG_PTR zero_bits, SIZE_T stack_size, SIZE_T maximum_stack_size, PVOID attribute_list );

using NtResumeThread = NTSTATUS( NTAPI* )( HANDLE thread_handle, PULONG suspend_count );

using NtSuspendThread = NTSTATUS( NTAPI* )( HANDLE thread_handle, PULONG suspend_count );

using NtSuspendProcess = NTSTATUS( NTAPI* )( HANDLE process_handle );

using NtResumeProcess = NTSTATUS( NTAPI* )( HANDLE process_handle );

using NtGetContextThread = NTSTATUS( NTAPI* )( HANDLE thread_handle, PCONTEXT context );

using NtSetContextThread = NTSTATUS( NTAPI* )( HANDLE thread_handle, PCONTEXT context );

using RtlAddVectoredExceptionHandler = NTSTATUS( NTAPI* )( IN ULONG FirstHandler, IN PVECTORED_EXCEPTION_HANDLER VectoredHandler );

typedef ULONG( NTAPI* RTLP_UNHANDLED_EXCEPTION_FILTER ) ( _In_ struct _EXCEPTION_POINTERS* ExceptionInfo );
typedef RTLP_UNHANDLED_EXCEPTION_FILTER* PRTLP_UNHANDLED_EXCEPTION_FILTER;

using RtlSetUnhandledExceptionFilter = void( NTAPI* )( IN PRTLP_UNHANDLED_EXCEPTION_FILTER TopLevelExceptionFilter );

using NtDuplicateObject = NTSTATUS( NTAPI* )( IN HANDLE SourceProcessHandle, IN PHANDLE SourceHandle, IN HANDLE TargetProcessHandle, OUT PHANDLE TargetHandle, IN ACCESS_MASK DesiredAccess OPTIONAL, IN BOOLEAN InheritHandle, IN ULONG Options );

using RtlGetFullPathName_U = NTSTATUS( NTAPI* )( PCWSTR FileName, ULONG Size, PWSTR Buffer, PWSTR* ShortName );

using RtlExitUserProcess = void( NTAPI* )( _In_ NTSTATUS ExitStatus );

using NtTerminateProcess = NTSTATUS( NTAPI* )(
  IN HANDLE               ProcessHandle OPTIONAL,
  IN NTSTATUS             ExitStatus );

using NtQueryLicenseValue = NTSTATUS( NTAPI* )(
	PUNICODE_STRING ValueName,
	ULONG* Type,
	PVOID Data,
	ULONG DataSize,
	ULONG* ResultDataSize );


using DbgUiIssueRemoteBreakin = NTSTATUS( NTAPI* )( IN HANDLE Process );

using DbgUiRemoteBreakin = void( NTAPI* )( void );

using DbgUiConnectToDbg = NTSTATUS( NTAPI* )( void );

using DbgUserBreakPoint = void( NTAPI* )( void );

using NtSetDebugFilterState = NTSTATUS( NTAPI* )( ULONG ComponentId, ULONG Level, BOOLEAN State );

using NtQueryVirtualMemory = NTSTATUS( NTAPI* )( IN HANDLE process_handle, IN PVOID base_address OPTIONAL,
												 IN int MemoryInformationClass, OUT PVOID MemoryInformation, IN SIZE_T MemoryInformationLength, PSIZE_T ReturnLength OPTIONAL );

typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR FullPathName[ 256 ];
} RTL_PROCESS_MODULE_INFORMATION, * PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
	ULONG NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[ 1 ];
} RTL_PROCESS_MODULES, * PRTL_PROCESS_MODULES;

typedef struct _RTL_BUFFER
{
	PUCHAR    Buffer;
	PUCHAR    StaticBuffer;
	SIZE_T    Size;
	SIZE_T    StaticSize;
	SIZE_T    ReservedForAllocatedSize; // for future doubling
	PVOID     ReservedForIMalloc; // for future pluggable growth
} RTL_BUFFER, * PRTL_BUFFER;

typedef struct _RTL_UNICODE_STRING_BUFFER
{
	UNICODE_STRING String;
	RTL_BUFFER     ByteBuffer;
	UCHAR          MinimumStaticBufferForTerminalNul[ sizeof( WCHAR ) ];
} RTL_UNICODE_STRING_BUFFER, * PRTL_UNICODE_STRING_BUFFER;


using RtlNtPathNameToDosPathName = NTSTATUS( NTAPI* )(
	__in ULONG Flags,
	__inout PRTL_UNICODE_STRING_BUFFER Path,
	__out_opt PULONG Disposition,
	__inout_opt PWSTR* FilePart
);

using NtOpenThread = NTSTATUS( NTAPI* )(
  _Out_ PHANDLE            ThreadHandle,
  _In_  ACCESS_MASK        DesiredAccess,
  _In_  POBJECT_ATTRIBUTES ObjectAttributes,
  _In_  CLIENT_ID* ClientId
);

// ReSharper restore CppInconsistentNaming

static const auto current_process = reinterpret_cast< HANDLE >( -1 );


template <typename T>
struct _LIST_ENTRY_T
{
	T Flink;
	T Blink;
};

template <typename T>
struct _UNICODE_STRING_T
{
	WORD Length;
	WORD MaximumLength;
	T Buffer;
};

template <typename T>
struct _NT_TIB_T
{
	T ExceptionList;
	T StackBase;
	T StackLimit;
	T SubSystemTib;
	T FiberData;
	T ArbitraryUserPointer;
	T Self;
};

template <typename T>
struct _CLIENT_ID_T
{
	T UniqueProcess;
	T UniqueThread;
};

template <typename T>
struct _GDI_TEB_BATCH_T
{
	DWORD Offset;
	T HDC;
	DWORD Buffer[ 310 ];
};

template <typename T>
struct _TEB_T
{
	typedef T type;

	_NT_TIB_T<T> NtTib;
	T EnvironmentPointer;
	_CLIENT_ID_T<T> ClientId;
	T ActiveRpcHandle;
	T ThreadLocalStoragePointer;
	T ProcessEnvironmentBlock;
	DWORD LastErrorValue;
	DWORD CountOfOwnedCriticalSections;
	T CsrClientThread;
	T Win32ThreadInfo;
	DWORD User32Reserved[ 26 ];
	T UserReserved[ 5 ];
	T WOW32Reserved;
	DWORD CurrentLocale;
	DWORD FpSoftwareStatusRegister;
	T SystemReserved1[ 54 ];
	DWORD ExceptionCode;
	T ActivationContextStackPointer;
	BYTE SpareBytes[ 36 ];
	DWORD TxFsContext;
	_GDI_TEB_BATCH_T<T> GdiTebBatch;
	_CLIENT_ID_T<T> RealClientId;
	T GdiCachedProcessHandle;
	DWORD GdiClientPID;
	DWORD GdiClientTID;
	T GdiThreadLocalInfo;
	T Win32ClientInfo[ 62 ];
	T glDispatchTable[ 233 ];
	T glReserved1[ 29 ];
	T glReserved2;
	T glSectionInfo;
	T glSection;
	T glTable;
	T glCurrentRC;
	T glContext;
	DWORD LastStatusValue;
	_UNICODE_STRING_T<T> StaticUnicodeString;
	wchar_t StaticUnicodeBuffer[ 261 ];
	T DeallocationStack;
	T TlsSlots[ 64 ];
	_LIST_ENTRY_T<T> TlsLinks;
	T Vdm;
	T ReservedForNtRpc;
	T DbgSsReserved[ 2 ];
	DWORD HardErrorMode;
	T Instrumentation[ 11 ];
	_GUID ActivityId;
	T SubProcessTag;
	T PerflibData;
	T EtwTraceData;
	T WinSockData;
	DWORD GdiBatchCount;
	DWORD IdealProcessorValue;
	DWORD GuaranteedStackBytes;
	T ReservedForPerf;
	T ReservedForOle;
	DWORD WaitingOnLoaderLock;
	T SavedPriorityState;
	T ReservedForCodeCoverage;
	T ThreadPoolData;
	T TlsExpansionSlots;
	T DeallocationBStore;
	T BStoreLimit;
	DWORD MuiGeneration;
	DWORD IsImpersonating;
	T NlsCache;
	T pShimData;
	USHORT HeapVirtualAffinity;
	USHORT LowFragHeapDataSlot;
	T CurrentTransactionHandle;
	T ActiveFrame;
	T FlsData;
	T PreferredLanguages;
	T UserPrefLanguages;
	T MergedPrefLanguages;
	DWORD MuiImpersonation;
	USHORT CrossTebFlags;
	USHORT SameTebFlags;
	T TxnScopeEnterCallback;
	T TxnScopeExitCallback;
	T TxnScopeContext;
	DWORD LockCount;
	DWORD SpareUlong0;
	T ResourceRetValue;
	T ReservedForWdf;
};

template <typename T, typename NGF, int A>
struct _PEB_T
{
	typedef T type;

	union
	{
		struct
		{
			BYTE InheritedAddressSpace;
			BYTE ReadImageFileExecOptions;
			BYTE BeingDebugged;
			BYTE BitField;
		};
		T dummy01;
	};
	T Mutant;
	T ImageBaseAddress;
	T Ldr;
	T ProcessParameters;
	T SubSystemData;
	T ProcessHeap;
	T FastPebLock;
	T AtlThunkSListPtr;
	T IFEOKey;
	T CrossProcessFlags;
	T UserSharedInfoPtr;
	DWORD SystemReserved;
	DWORD AtlThunkSListPtr32;
	T ApiSetMap;
	T TlsExpansionCounter;
	T TlsBitmap;
	DWORD TlsBitmapBits[ 2 ];
	T ReadOnlySharedMemoryBase;
	T HotpatchInformation;
	T ReadOnlyStaticServerData;
	T AnsiCodePageData;
	T OemCodePageData;
	T UnicodeCaseTableData;
	DWORD NumberOfProcessors;
	union
	{
		DWORD NtGlobalFlag;
		NGF dummy02;
	};
	LARGE_INTEGER CriticalSectionTimeout;
	T HeapSegmentReserve;
	T HeapSegmentCommit;
	T HeapDeCommitTotalFreeThreshold;
	T HeapDeCommitFreeBlockThreshold;
	DWORD NumberOfHeaps;
	DWORD MaximumNumberOfHeaps;
	T ProcessHeaps;
	T GdiSharedHandleTable;
	T ProcessStarterHelper;
	T GdiDCAttributeList;
	T LoaderLock;
	DWORD OSMajorVersion;
	DWORD OSMinorVersion;
	WORD OSBuildNumber;
	WORD OSCSDVersion;
	DWORD OSPlatformId;
	DWORD ImageSubsystem;
	DWORD ImageSubsystemMajorVersion;
	T ImageSubsystemMinorVersion;
	T ActiveProcessAffinityMask;
	T GdiHandleBuffer[ A ];
	T PostProcessInitRoutine;
	T TlsExpansionBitmap;
	DWORD TlsExpansionBitmapBits[ 32 ];
	T SessionId;
	ULARGE_INTEGER AppCompatFlags;
	ULARGE_INTEGER AppCompatFlagsUser;
	T pShimData;
	T AppCompatInfo;
	_UNICODE_STRING_T<T> CSDVersion;
	T ActivationContextData;
	T ProcessAssemblyStorageMap;
	T SystemDefaultActivationContextData;
	T SystemAssemblyStorageMap;
	T MinimumStackCommit;
	T FlsCallback;
	_LIST_ENTRY_T<T> FlsListHead;
	T FlsBitmap;
	DWORD FlsBitmapBits[ 4 ];
	T FlsHighIndex;
	T WerRegistrationData;
	T WerShipAssertPtr;
	T pContextData;
	T pImageHeaderHash;
	T TracingFlags;
	T CsrServerReadOnlySharedMemoryBase;
};

typedef _PEB_T<DWORD, DWORD64, 34> _PEB32;
typedef _PEB_T<DWORD64, DWORD, 30> _PEB64;

template<typename T>
struct _VM_COUNTERS_T
{
	T PeakVirtualSize;
	T VirtualSize;
	ULONG PageFaultCount;
	T PeakWorkingSetSize;
	T WorkingSetSize;
	T QuotaPeakPagedPoolUsage;
	T QuotaPagedPoolUsage;
	T QuotaPeakNonPagedPoolUsage;
	T QuotaNonPagedPoolUsage;
	T PagefileUsage;
	T PeakPagefileUsage;
};

template<typename T>
struct _SYSTEM_THREAD_INFORMATION_T
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG WaitTime;
	T StartAddress;
	_CLIENT_ID_T<T> ClientId;
	LONG Priority;
	LONG BasePriority;
	ULONG ContextSwitches;
	ULONG ThreadState;
	ULONG WaitReason;
};

template<typename T>
struct _SYSTEM_EXTENDED_THREAD_INFORMATION_T
{
	_SYSTEM_THREAD_INFORMATION_T<T> ThreadInfo;
	T StackBase;
	T StackLimit;
	T Win32StartAddress;
	T TebBase;
	T Reserved[ 3 ];
};

template<typename T>
struct _SYSTEM_PROCESS_INFORMATION_T
{
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER WorkingSetPrivateSize;
	ULONG HardFaultCount;
	ULONG NumberOfThreadsHighWatermark;
	ULONGLONG CycleTime;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	_UNICODE_STRING_T<T> ImageName;
	LONG BasePriority;
	T UniqueProcessId;
	T InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG SessionId;
	T UniqueProcessKey;
	_VM_COUNTERS_T<T> VmCounters;
	T PrivatePageCount;
	IO_COUNTERS IoCounters;
	_SYSTEM_EXTENDED_THREAD_INFORMATION_T<T> Threads[ 1 ];
};

typedef _TEB_T<DWORD>     _TEB32;

typedef struct _LDR_MODULE
{
	LIST_ENTRY              InLoadOrderModuleList;
	LIST_ENTRY              InMemoryOrderModuleList;
	LIST_ENTRY              InInitializationOrderModuleList;
	PVOID                   BaseAddress;
	PVOID                   EntryPoint;
	ULONG                   SizeOfImage;
	UNICODE_STRING          FullDllName;
	UNICODE_STRING          BaseDllName;
	ULONG                   Flags;
	SHORT                   LoadCount;
	SHORT                   TlsIndex;
	LIST_ENTRY              HashTableEntry;
	ULONG                   TimeDateStamp;

} LDR_MODULE, * PLDR_MODULE;

typedef struct _PEB_LOADER_DATA
{
	ULONG                   Length;
	BOOLEAN                 Initialized;
	PVOID                   SsHandle;
	LIST_ENTRY              InLoadOrderModuleList;
	LIST_ENTRY              InMemoryOrderModuleList;
	LIST_ENTRY              InInitializationOrderModuleList;
} PEB_LOADER_DATA, * PPEB_LOADER_DATA;

#define FLG_HEAP_ENABLE_TAIL_CHECK   0x10
#define FLG_HEAP_ENABLE_FREE_CHECK   0x20
#define FLG_HEAP_VALIDATE_PARAMETERS 0x40
#define NT_GLOBAL_FLAG_DEBUGGED (FLG_HEAP_ENABLE_TAIL_CHECK | FLG_HEAP_ENABLE_FREE_CHECK | FLG_HEAP_VALIDATE_PARAMETERS)