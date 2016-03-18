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

	t->setNum(100);
	t->setNum1(200);
	t->setNum2(300);

	cout << "Protected: " << t->getNum() << " Private: " << t->getNum2() << endl;

	delete t;

	/* in C++ you NEED to delete objects */
	

	return 0;
}
