#include <iostream>

using std::cout;
using std::endl;

class Test {
protected: //ony this class can access this var and you don't have direct access
	int num;

private: //we need a function/method to access this var in other class
	int num1;

public: //everybody can access this one
	int num2;

	/* constructor */

	Test(int number) {
		this->num = number;
	}
	
	Test() {

	}

	/* destructor */
	~Test() {
		cout << "Destroying object" << endl;
	}

	void setNum(int num) {
		this->num = num;
	}

	void setNum1(int num1) {
		this->num1 = num1;
	}

	void setNum2(int num2) {
		this->num2 = num2;
	}

	int getNum() {
		return this->num;
	}

	int getNum1() {
		return this->num1;
	}
};

int main() {

	Test* t = new Test();
	Test* t2 = new Test(999);

	t->setNum(100);
	t->setNum1(200);
	t->setNum2(300);

	cout << "Protected: " << t->getNum() << " Private: " << t->getNum1() << endl;
	cout << "Constructor: " << t2->getNum() << endl;

	delete t;

	/* in C++ you NEED to delete objects */
	

	return 0;
}
