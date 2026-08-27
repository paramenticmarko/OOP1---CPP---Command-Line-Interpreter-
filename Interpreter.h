#ifndef _interpreter_h_
#define _interpreter_h_

#include "Command.h"
#include "Parser.h"
#include "Reader.h"
#include "Exceptions.h"
#include "Time.h"
#include "Date.h"

#include<string>

using namespace std;

class Interpreter {
public:
	static Interpreter& getInstance();

	string getPrompt();

	void setPrompt(string prompt);

	void run();

	void executeLine(string line);
	void executeLine(string line, istream& in, ostream& out);

private:
	void runPipeline(ParsedPipeline* parsed, istream& defaultIn, ostream& defaultOut);

	Interpreter();
	~Interpreter();

	Reader* reader;
	Parser* parser;
	string prompt;
};

#endif