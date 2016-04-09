#include "hwid.h" //headers here
#include "sock.h"
 
int main(){	
	char username[MAX_PATH];
	char password[MAX_PATH];
	cout << "Username:" << endl;
	cin >> username;
	cout << "Password:" << endl;
	cin >> password;
	string FinalID = HwidF::Instance().ID();
	cout << " === Summary ===" << endl;	
	cout << " Username: " << username << " Password: " << password << endl;
	cout << " Your HWID: " << FinalID << endl;
 
	string logininfo = "user=" + string(username) + "&pass=" + string(password) + "&hwid=" + FinalID;
	int loginInfoSize = logininfo.length();
	string header;
	header = "POST /login_HWID.php HTTP/1.1\r\n";
	header += "Host: www.your-site.com:80\r\n";
	header += "Content-Type: application/x-www-form-urlencoded\r\n";
	header += "Content-Length: " + to_string(loginInfoSize) + "\r\n";
	header += "Accept-Charset: utf-8\r\n";
	header += "\r\n";
	header += logininfo + "\r\n";
	header += "\r\n";
 
	cout << " ===Send Form===" << endl;
	MySock::Instance().Login(header);
 
	while (true){
		Sleep(1);
	}
	return 0;
}
