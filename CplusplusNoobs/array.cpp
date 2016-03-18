#include <iostream>

using std::cout;
using std::endl;

int main() {

	int numbers[10] = {};

	for (int i = 0; i < 10; ++i) {
		numbers[i] = i;
	}

	for (int i = 0; i < 10; ++i) {
		cout << "Item: " << i << " value: " << numbers[i] << endl;
	}

	/*
	Array is like a box where you can store different things in diferent compartiments
		LINES X COLLS

		int number[10][10]  this mean a matrix of 10x10, 10 lines by 10 collums
	
	*/

	int n1[10][10] = {};

	n1[3][4] = 5;

	n1[0][0] = 1;

	cout << "Line 3 and Col 4 value: " << n1[3][4] << endl;
	cout << "Line 0 and Col 0 value: " << n1[0][0] << endl;



	return 0;
}
