#include "Interpreter.h"
#include "Exceptions.h"

#include<iostream>

using namespace std;

int main() {
	Interpreter::getInstance().run();
	return 0;
}
