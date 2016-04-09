#include "hwid.h"
 
class MySock{
public:
	static MySock& Instance(){
		static MySock ms;
		return ms;
	}
 
	WSADATA wsaData;
	SOCKET mySocket;
	bool InitWinSock();
	struct addrinfo *results = NULL, *prt = NULL, hints;
	bool ConfigureSock();
	bool Connect();
	void Disconnect();	
	bool Login(string buf);
 
};
