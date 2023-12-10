class IClientNetworkable;
class ClientClass;
struct RecvTable;

using CreateClientClassFn = IClientNetworkable * ( *)( int, int );
using CreateEventFn = IClientNetworkable * ( *)( );

class ClientClass
{
public:
	CreateClientClassFn m_pCreateFn;
	CreateEventFn m_pCreateEventFn;
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	ClientClass* m_pNext;
	ClassId m_ClassID;
};
