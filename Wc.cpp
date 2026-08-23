#include "Wc.h"
#include "Reader.h"

#include<iostream>
#include<cctype>

void Wc::execute(istream& in, ostream& out) {
	string text = this->commandArg(this->arg,in);

	if (this->opt == "-c") {
		size_t count = text.length();

		if (&in == &cin && this->arg == "") count++;

		out << count << endl;
	}

	else if (this->opt == "-w") {
		int counter = 0;
		string current = "";
		for (char c : text) {
			if (isspace(c)) {
				if (!current.empty()) {
					counter++;
					current.clear();
				}
			}
			else {
				current.push_back(c);
			}
		}

		if (!current.empty()) {
			counter++;
		}

		out << counter<< endl;
	}
}
