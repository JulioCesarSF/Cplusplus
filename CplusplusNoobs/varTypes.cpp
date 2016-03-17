#include <iostream>
#include <Windows.h>

using std::cout;
using std::endl;

int main() { 
	
	/* there is a lot for types of var, i will just list the most common */

	int number;
	float numberWithDot;
	double numberWithDot; /* 2x more precise than float, you can use "long double" for a bigger one */
	bool turned; /* true == 1 , false == 0*/
	char word[256]; /* this a array of characters "letter" */ 
	std::string name; /* this the most powerfull type to work with "words", string is a set of chars */

	/* if you are working with WINAPI probally you gonna see those 
	https://msdn.microsoft.com/en-us/library/windows/desktop/aa383751(v=vs.85).aspx
	*/
	DWORD pid;
	BOOL isIsTrue;
	BYTE test;
	CHAR teste2;
	HANDLE test1;

	return 0;
}
