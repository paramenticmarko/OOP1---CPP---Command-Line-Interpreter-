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

	string getPrompt() const;

	void setPrompt(const string& prompt);

	void run();

	void executeLine(const string& line);
	void executeLine(const string& line, istream& in, ostream& out);

private:
	void runPipeline(ParsedPipeline* parsed, istream& defaultIn, ostream& defaultOut);

	Interpreter();
	Interpreter(const Interpreter&) = delete;
	void operator=(const Interpreter&) = delete;
	~Interpreter();

	Reader* reader;
	Parser* parser;
	string prompt;
};

#endif