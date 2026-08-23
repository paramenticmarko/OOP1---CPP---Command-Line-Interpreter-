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
	ParsedLine* parseCommandLine(string line); // tokenize + parse single command
    ParsedPipeline* parsePipeline(string line); // tokenize + split by '|' + parse 
    ParsedLine* parseSingleCommandTokens(vector<string> tokens); // tokens -> cmd + redirect

private:
	vector<string> tokenize(string line); // line -> tokens
	Command* createCommand(vector<string> tokens); // cmd name -> cmd obj
};

#endif