#ifndef _parser_h_
#define _parser_h_

#include "Command.h"

#include <string>
#include <vector>

using namespace std;

struct ParsedLine {
    Command* command = nullptr;

    bool hasInputRedirect = false;
    string inputFile = "";

    bool hasOutputRedirect = false;
    string outputFile = "";
    bool appendOutput = false;
};

struct ParsedPipeline {
    vector<ParsedLine*> commands;
};

class Parser {
public:
	ParsedLine* parseCommandLine(const string& line); // tokenize + parse single command
    ParsedPipeline* parsePipeline(const string& line); // tokenize + split by '|' + parse 
    ParsedLine* parseSingleCommandTokens(const vector<string>& tokens); // tokens -> cmd + redirect

private:
	vector<string> tokenize(const string& line); // line -> tokens
	Command* createCommand(const vector<string>& tokens); // cmd name -> cmd obj
};

#endif