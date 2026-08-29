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

#include <unordered_map>

unordered_map<string, Command* (*)(const vector<string>&)> Parser::commands = {

        { "touch", +[](const vector<string>& tokens) -> Command* {
            if (tokens.size() != 2)
                throw CommandError("Touch: Wrong number of arguments!");
            return new Touch(tokens[1]);
        }},

        { "wc", +[](const vector<string>& tokens) -> Command* {
            if (tokens.size() > 3)
                throw CommandError("Wc: Wrong number of arguments!");
            string opt = "";
            string arg = "";
            if (tokens.size() > 1) opt = tokens[1];
            if (tokens.size() > 2) arg = tokens[2];
            return new Wc(opt, arg);
        }},

        { "tr", +[](const vector<string>& tokens) -> Command* {
            string arg = "";
            string opt = "";
            string ext = "";
            int i = 1;

            if (i < (int)tokens.size() && !tokens[i].empty() && tokens[i][0] != '-') {
                arg = tokens[i];
                i++;
            }
            if (i >= (int)tokens.size() || tokens[i].empty() || tokens[i][0] != '-') {
                throw CommandError("Tr: Missing -what!");
            }
            opt = tokens[i].substr(1);
            if (!opt.empty() && opt.front() == '"' && opt.back() == '"') {
                opt = opt.substr(1, opt.size() - 2);
            }
            i++;
            if (i < (int)tokens.size() && !tokens[i].empty()) {
                ext = tokens[i];
                if (!ext.empty() && ext.front() == '"' && ext.back() == '"') {
                    ext = ext.substr(1, ext.size() - 2);
                }
                i++;
            }
            if (i != (int)tokens.size())
                throw CommandError("Tr: Wrong number of arguments!");
            return new Tr(opt, arg, ext);
        }},

        { "time", +[](const vector<string>& tokens) -> Command* {
            if (tokens.size() != 1) {
                throw CommandError("Time: Wrong number of arguments!");
            }
            return new Time("");
        }},

        { "date", +[](const vector<string>& tokens) -> Command* {
            if (tokens.size() != 1) {
                throw CommandError("Date: Wrong number of arguments!");
            }
            return new Date("");
        }},

        { "rm", +[](const vector<string>& tokens) -> Command* {
            if (tokens.size() != 2) {
                throw CommandError("Rm: Wrong number of arguments!");
            }
            return new Rm(tokens[1]);
        }},

        { "echo", +[](const vector<string>& tokens) -> Command* {
            if (tokens.size() > 2)
                throw CommandError("Echo: Wrong number of arguments!");
            string arg = "";
            if (tokens.size() > 1) arg = tokens[1];
            return new Echo(arg);
        }},

        { "prompt", +[](const vector<string>& tokens) -> Command* {
            if (tokens.size() != 2)
                throw CommandError("Prompt: Wrong number of arguments!");
            return new Prompt(tokens[1]);
        }},

        { "truncate", +[](const vector<string>& tokens) -> Command* {
            if (tokens.size() != 2)
                throw CommandError("Truncate: Wrong number of arguments!");
            return new Truncate(tokens[1]);
        }},

        { "head", +[](const vector<string>& tokens) -> Command* {
            if (tokens.size() > 3) {
                throw CommandError("Head: Wrong number of arguments!");
            }
            string opt = "";
            string arg = "";
            if (tokens.size() > 1) opt = tokens[1];
            if (tokens.size() > 2) arg = tokens[2];
            return new Head(opt, arg);
        }},

        { "batch", +[](const vector<string>& tokens) -> Command* {
            if (tokens.size() != 2) {
                throw CommandError("Batch: Wrong number of arguments!");
            }
            string arg = "";
            if (tokens.size() > 1) arg = tokens[1];
            return new Batch(arg);
        }},

};

bool isRedOrPipeToken(const string& str) {
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

Command* Parser::createCommand(const vector<string>& tokens) {
    if (tokens.empty()) return nullptr;

    string cmd = tokens[0];

    auto it = commands.find(cmd);
    if (it != commands.end()) {
        return it->second(tokens);
    }

    throw CommandError("Unknown command: " + cmd);
}

ParsedLine* Parser::parseCommandLine(const string& line) {
    vector<string> tokens = tokenize(line);
    return parseSingleCommandTokens(tokens);
}

ParsedLine* Parser::parseSingleCommandTokens(const vector<string>& tokens) {
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

ParsedPipeline* Parser::parsePipeline(const string& line) {
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

vector<string> Parser::tokenize(const string& line) {
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

// PipeRef - referenca na N-tu komandu u pipeline-u (npr. "echo 123 | wc -c | 2")
    /*bool isNumber = !cmd.empty() && all_of(cmd.begin(), cmd.end(), ::isdigit);
    if (isNumber) {
        if (tokens.size() != 1)
            throw CommandError("Pipeline reference: Wrong number of arguments!");
        return new PipeRef(cmd);
    }
    */


