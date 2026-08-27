#include "Parser.h"
#include "Exceptions.h"
#include "Echo.h"
#include "Time.h"
#include "Date.h"
#include "Touch.h"
#include "Wc.h"
#include "Prompt.h"
#include "Truncate.h"
#include "Rm.h"
#include "Head.h"
#include "Tr.h"
#include "Batch.h"

bool isRedOrPipeToken(string str) {
    return str == "<" || str == ">" || str == ">>" || str == "|";
}

void deletePipeline(ParsedPipeline* pipe) {
    if (!pipe) return;

    for (int i = 0; i < (int)pipe->commands.size(); i++) {
        delete pipe->commands[i]->command;
        delete pipe->commands[i];
    }

    delete pipe;
}

ParsedLine* Parser::parseCommandLine(string line) {
    vector<string> tokens = tokenize(line);
    return parseSingleCommandTokens(tokens);
}

ParsedLine* Parser::parseSingleCommandTokens(vector<string> tokens) {
    if (tokens.empty()) return nullptr;

    ParsedLine* parsed = new ParsedLine();

    vector<string> commandTokens;
    int i = 0;

    while (i < (int)tokens.size() && 
        tokens[i] != "<" &&
        tokens[i] != ">" &&
        tokens[i] != ">>") 
        {
            commandTokens.push_back(tokens[i]);
            i++;
        }

    parsed->command = createCommand(commandTokens);

    while (i < (int)tokens.size()) {
        if (tokens[i] == "<") {
            if (parsed->hasInputRedirect) {
                delete parsed->command;
                delete parsed;
                throw SyntaxError("Has more than one input redirection!");
            }
            if (i + 1 >= (int)tokens.size() || isRedOrPipeToken(tokens[i + 1])) {
                delete parsed->command;
                delete parsed;
                throw SyntaxError("Missing input file after < !");
            }
            parsed->hasInputRedirect = true;
            parsed->inputFile = tokens[i + 1];
            i += 2;
        }
        else if (tokens[i] == ">") {
            if (parsed->hasOutputRedirect) {
                delete parsed->command;
                delete parsed;
                throw SyntaxError("Has more than one output redirection!");
            }
            if (i + 1 >= (int)tokens.size() || isRedOrPipeToken(tokens[i + 1])) {
                delete parsed->command;
                delete parsed;
                throw SyntaxError("Missing output file after > !");
            }

            parsed->hasOutputRedirect = true;
            parsed->appendOutput = false;
            parsed->outputFile = tokens[i + 1];
            i += 2;
        }
        else if (tokens[i] == ">>") {
            if (parsed->hasOutputRedirect) {
                delete parsed->command;
                delete parsed;
                throw SyntaxError("Has more than one output redirection!");
            }
            if (i + 1 >= (int)tokens.size() || isRedOrPipeToken(tokens[i + 1])){
                delete parsed->command;
                delete parsed;
                throw SyntaxError("Missing output file after >> !");
            }
            parsed->hasOutputRedirect = true;
            parsed->appendOutput = true;
            parsed->outputFile = tokens[i + 1];
            i += 2;
        }
        else {
            delete parsed->command;
            delete parsed;
            throw SyntaxError("Invalid command redirection!");
        }
    }

    return parsed;
}

ParsedPipeline* Parser::parsePipeline(string line) {
    vector<string> tokens = tokenize(line);
    if (tokens.empty()) return nullptr;

    ParsedPipeline* pipeline = new ParsedPipeline();
    vector<string> currentTokens;

    for (int i = 0; i < (int)tokens.size(); i++) {
        if (tokens[i] == "|") {
            if (currentTokens.empty()) {
                delete pipeline;
                throw SyntaxError("Invalid pipeline syntax!");
            }

            ParsedLine* parsed = parseSingleCommandTokens(currentTokens);
            if (parsed == nullptr) {
                delete pipeline;
                throw SyntaxError("Invalid pipeline syntax!");
            }

            pipeline->commands.push_back(parsed);
            currentTokens.clear();
        }
        else {
            currentTokens.push_back(tokens[i]);
        }
    }

    if (currentTokens.empty()) {
        deletePipeline(pipeline);
        throw SyntaxError("Invalid pipeline syntax!");
    }

    ParsedLine* parsed = parseSingleCommandTokens(currentTokens);
    if (parsed == nullptr) {
        deletePipeline(pipeline);
        throw SyntaxError("Invalid pipeline syntax!");
    }

    pipeline->commands.push_back(parsed);

    return pipeline;
}


vector<string> Parser::tokenize(string line) {
    vector<string> tokens;
    string current;
    bool leftQuoteMark = false;

    for (int i = 0; i < (int)line.size(); i++) {
        char c = line[i];

        if (!leftQuoteMark) {
            if (isspace(c)) {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
            }
            else if (c == '<') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
                tokens.push_back("<");
            }
            else if (c == '>') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }

                if (i + 1 < (int)line.size() && line[i + 1] == '>') {
                    tokens.push_back(">>");
                    i++;
                }
                else {
                    tokens.push_back(">");
                }
            }
            else if (c == '|') {
                if (!current.empty()) {
                    tokens.push_back(current);
                    current.clear();
                }
                tokens.push_back("|");
            }
            else {
                current.push_back(c);
                if (c == '"') leftQuoteMark = true;
            }
        }
        else {
            current.push_back(c);
            if (c == '"') leftQuoteMark = false;
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    return tokens;
}

Command* Parser::createCommand(vector<string> tokens) {
    if (tokens.empty()) return nullptr;
    string cmd = tokens[0];

    if (cmd == "touch") {
        if (tokens.size() != 2)
            throw CommandError("Touch: Wrong number of arguments!");
        return new Touch(tokens[1]);
    }
    if (cmd == "time") {
        if (tokens.size() != 1)
            throw CommandError("Time: Wrong number of arguments!");
        return new Time("");
    }
    if (cmd == "date") {
        if (tokens.size() != 1)
            throw CommandError("Date: Wrong number of arguments!");
        return new Date("");
    }
    if (cmd == "echo") {
        if(tokens.size()>2)
            throw CommandError("Echo: Wrong number of arguments!");
        string arg = "";
        if (tokens.size() > 1) arg = tokens[1];
        return new Echo(arg);
    }
    if (cmd == "wc") {
        if(tokens.size()>3)
            throw CommandError("Wc: Wrong number of arguments!");
        string opt = "";
        string arg = "";

        if (tokens.size() > 1) opt = tokens[1];
        if (tokens.size() > 2) arg = tokens[2];

        return new Wc(opt, arg);
    }
    if (cmd == "prompt"){
        if(tokens.size()!=2)
            throw CommandError("Prompt: Wrong number of arguments!");
        return new Prompt(tokens[1]);
    }
    if (cmd == "truncate"){
        if(tokens.size()!=2)
            throw CommandError("Truncate: Wrong number of arguments!");
        return new Truncate(tokens[1]);
    }
    if (cmd == "rm") {
        if(tokens.size()!=2)
            throw CommandError("Rm: Wrong number of arguments!");
        return new Rm(tokens[1]);
    }
    if (cmd == "head") {
        if(tokens.size()>3)
            throw CommandError("Head: Wrong number of arguments!");
        string opt = "";
        string arg = "";

        if (tokens.size() > 1) opt = tokens[1];
        if (tokens.size() > 2) arg = tokens[2];

        return new Head(opt, arg);
    }
    if (cmd == "tr") {
        string arg = "";
        string opt = "";
        string ext = "";

        int i = 1;

        if (i < tokens.size() && !tokens[i].empty() && tokens[i][0] != '-') {
            arg = tokens[i];
            i++;
        }

        if (i >= tokens.size() || tokens[i].empty() || tokens[i][0] != '-') {
            throw CommandError("Tr: Missing -what!");
        }

        opt = tokens[i].substr(1);
        if (!opt.empty() && opt.front() == '"' && opt.back() == '"') {
            opt = opt.substr(1, opt.size() - 2);
        }

        i++;

        if (i < tokens.size() && !tokens[i].empty()) {
            ext = tokens[i];
            if (!ext.empty() && ext.front() == '"' && ext.back() == '"') {
                ext = ext.substr(1, ext.size() - 2);
            }
            i++;
        }

        if(i!=tokens.size())
            throw CommandError("Tr: Wrong number of arguments!");

        return new Tr(opt, arg, ext);
    }
    if (cmd == "batch") {
        if(tokens.size()!=2)
            throw CommandError("Batch: Wrong number of arguments!");

        string arg = "";
        if (tokens.size() > 1) arg = tokens[1];
        return new Batch(arg);
    }
    // PipeRef - referenca na N-tu komandu u pipeline-u (npr. "echo 123 | wc -c | 2")
    /*bool isNumber = !cmd.empty() && all_of(cmd.begin(), cmd.end(), ::isdigit);
    if (isNumber) {
        if (tokens.size() != 1)
            throw CommandError("Pipeline reference: Wrong number of arguments!");
        return new PipeRef(cmd);
    }
    */
    throw CommandError("Unknown command: " + cmd);
}


