#include "Truncate.h"
#include "Exceptions.h"

#include<fstream>
#include<filesystem>

using namespace std;

void Truncate::execute(istream& in, ostream& out) {
	if (!filesystem::exists(this->arg)) {
		throw SemanticError("Error! File with this name does not exist!");
	}

	ofstream file(this->arg, ios::trunc);
}
