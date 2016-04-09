#include "sock.h"
 
bool MySock::InitWinSock(){
	if (int result = WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
		printf_s("WSAStartup failed: %d\n", result);
		return false;
	}
	return true;
}
 
bool MySock::ConfigureSock(){
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	int result = getaddrinfo("www.your-site.com", "80", &hints, &results);
	if (result != 0){
		printf_s("getaddrinf failed: %d\n", result);
		WSACleanup();
		return false;
	}
	else{
		prt = results;
	}
 
	mySocket = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
	if (mySocket == INVALID_SOCKET){
		printf_s("Error at socket(): %l\n", WSAGetLastError());
		freeaddrinfo(results);
		WSACleanup();
		return false;
	}
 
	printf_s("Connected to: www.your-site.com \n");
 
	return true;
}
 
bool MySock::Connect(){
	int result = connect(mySocket, prt->ai_addr, (int)prt->ai_addrlen);
	if(result == SOCKET_ERROR){
		freeaddrinfo(results);
		Disconnect();
               return false;
	}
	return true;
}
 
void MySock::Disconnect(){
	closesocket(mySocket);
	WSACleanup();
	cout << "Disconnected!" << endl;
}
 
bool MySock::Login(string buf){
 
	if (!InitWinSock()) return false; //ok
 
	if (!ConfigureSock()) return false; //ok
 
	if (!Connect()){ //ok
		Disconnect();
		return false;
	}
	//send
	int result = send(mySocket, buf.c_str(), buf.size(), 0);
	if (result == SOCKET_ERROR){
		printf_s("send failed: %d\n", WSAGetLastError());
		Disconnect();
		return false;
	}
	else{
		//cout << buf.c_str() << endl;
		printf_s("Sending...\n");
		printf_s("Bytes Sent: %ld\n", result);
	}
 
	result = shutdown(mySocket, SD_SEND);
	if (result == SOCKET_ERROR){
		printf_s("shutdown failed: %d\n", WSAGetLastError());
		Disconnect();
		return false;
	}
 
	//recv
	int recvbuflen = 512;
	char recvbuf[512];
	result = recv(mySocket, recvbuf, recvbuflen, 0);
	if (result > 0){
		printf_s("Bytes received: %d\n", result);
		printf_s("Checking...\n");
		string response = string(recvbuf);
		if (response.find("index_hwid.php") != string::npos){
			printf_s("NO LOGIN!\n");
		}
		else if (response.find("login_success_HWID.php") != string::npos){
			printf_s("LOGGED ON!\n");
		}
		//cout << response << endl;
	}
	else if (result == 0)
		printf_s("Connection closed\n");
	else
		printf_s("recv failed: %d\n", WSAGetLastError());
 
	Disconnect();
	return true;
}
