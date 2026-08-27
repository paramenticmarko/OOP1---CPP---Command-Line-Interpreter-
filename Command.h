#ifndef _command_h_
#define _command_h_

#include<string>

using namespace std;

class Command {
public:
	Command(string opt, string arg, string ext);
	Command(string opt, string arg);
	explicit Command(string arg);

	virtual ~Command();

	virtual void execute(istream& in, ostream& out) = 0;

	string commandArg(const string& arg, istream& in);

	string getArg() { 
		return this->arg; 
	}
	string getOpt() { 
		return this->opt; 
	}
	string getExt() {
		return this->ext; 
	}

protected:
	string opt; // option
	string arg; // argument 
	string ext; // extra argument
};

#endif

