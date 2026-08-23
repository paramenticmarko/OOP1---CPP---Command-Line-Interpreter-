#include "Tr.h"

#include<iostream>

void replaceAll(string& s, const string& from, const string& to) {
    if (from.empty()) {
        return;
    }

    size_t pos = 0;
    pos = s.find(from, pos);

    while (pos != string::npos) {
        s.replace(pos, from.length(), to);
        pos += to.length();
        pos = s.find(from, pos);
    }
}

void Tr::execute(istream& in, ostream& out) {
	string text = this->commandArg(this->arg,in);
    replaceAll(text, opt, ext);
    out << text;
    
    if (&out == &cout) {
        out << endl;
    }
}
