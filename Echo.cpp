#include "Echo.h"
#include "Reader.h"

#include<iostream>

void Echo::execute(istream& in, ostream& out) {
	string text = this->commandArg(this->arg,in);

	if (!text.empty()) {
		out << text;
		if (&out == &cout) out << endl;
	}
}