#include <iostream>

using std::cout;
using std::endl;

int main() { 

	/* first you need to know about logical operatos 
	
		> bigger
		<
		<> different
		== equal
		!= not equal
		! not
		&& and
		|| or
	
	*/

	int number;

	bool isItTrue = false;

	cout << "Type a number: " << endl;
	std::cin >> number;

	if (number > 0) { /* you are going to check a var */
		cout << "Number " << number << endl;
	}
	else { /* if it's not true we use else */
		cout << "Number is 0 or not greater than 0" << endl;
	}

	if (!isItTrue)
		cout << isItTrue;
	else
		cout << isItTrue;
	


	return 0;
}
