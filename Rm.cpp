#include "Rm.h"
#include "Exceptions.h"

#include<filesystem>

using namespace std;

void Rm::execute(istream& in, ostream& out) {
	if (!filesystem::exists(this->arg)) {
		throw SemanticError("Error! File with this name does not exist!");
	}

	filesystem::remove(this->arg);
}
