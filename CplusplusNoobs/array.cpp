#include <iostream>

using std::cout;
using std::endl;

int main() {

	/*
	Array is like a box where you can store different things in diferent compartiments
	*/
	
	int numbers[10] = {};

	for (int i = 0; i < 10; ++i) {
		numbers[i] = i;
	}

	for (int i = 0; i < 10; ++i) {
		cout << "Item: " << i << " value: " << numbers[i] << endl;
	}

	
	return 0;
}
