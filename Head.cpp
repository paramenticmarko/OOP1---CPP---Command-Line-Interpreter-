#include "Head.h"

#include<string>
#include<iostream>
#include<cstdlib>
#include<sstream>

using namespace std;

void Head::execute(istream& inp, ostream& out) {
	istringstream in(this->commandArg(this->arg,inp));
	string text = "",line;
	int n = stoi((this->opt).substr(2));
	int i = 0;

	while (i++ < n && getline(in, line)) {
		text += line + '\n';
	}

	out << text;
}
