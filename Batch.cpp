#include "Batch.h"
#include "Interpreter.h"

#include<fstream>
#include<iostream>

void Batch::execute(istream& in, ostream& out) {
	ifstream file(this->arg);
	string line;

	while (getline(file, line)) {
		Interpreter::getInstance().executeLine(line, in, out);

		if (&in == &cin) {
			in.clear();
		}
	}
}
