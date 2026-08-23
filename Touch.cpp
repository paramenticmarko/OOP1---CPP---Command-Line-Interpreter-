#include "Touch.h"
#include "Exceptions.h"

#include<fstream>
#include<iostream>

using namespace std;

void Touch::execute(istream& in, ostream& out) {
	ifstream exists(this->arg);

	if (exists.good()) {
		throw SemanticError("File with this name already exists!");
	}

	ofstream output(this->arg);

	if (!output) {
		throw SemanticError("File cannot be created!");
	}
}
