#include <iostream>

using std::cout;
using std::endl;

template<class T>
void compare(T y, T x) {
	if (y > x)
		cout << "Y is bigger" << endl;
	else
		cout << "X is bigger" << endl;
}

int main() {

	int n = 10;
	

	compare<int>(n, 1);
	
	

	return 0;
}
