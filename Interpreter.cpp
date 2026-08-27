#include "Interpreter.h"

#include<string>
#include<iostream>
#include<fstream>
#include<sstream>

Interpreter::Interpreter() // priv constr
{
	this->reader = new ConsoleReader();
	this->parser = new Parser();
	this->prompt = "$";
}

Interpreter::~Interpreter() // priv destr
{
	delete reader;
	delete parser;
}

Interpreter& Interpreter::getInstance() // singleton
{
	static Interpreter instance;
	return instance;
}

string Interpreter::getPrompt() const
{
	return this->prompt;
}

void Interpreter::setPrompt(const string& prompt)
{
	this->prompt = prompt;
}

void Interpreter::executeLine(const string& line) { // std in/out
	executeLine(line, cin, cout);
}

void Interpreter::executeLine(const string& line, istream& defaultIn, ostream& defaultOut)
{
	ParsedPipeline* parsed = nullptr;

	try
	{
		parsed = this->parser->parsePipeline(line); // parse line

		if (parsed != nullptr && !parsed->commands.empty())
		{
			// exceptions

			for (int i = 0; i < (int)parsed->commands.size(); i++) { //semantic 
				ParsedLine* cmd = parsed->commands[i];

				bool isFirst = (i == 0);
				bool isLast = (i == (int)parsed->commands.size() - 1);

				if (!isFirst && cmd->hasInputRedirect)
					throw SemanticError("Only the first command can have input redrection!");
				if (!isLast && cmd->hasOutputRedirect)
					throw SemanticError("Only the last command can have output redirection!");
				if (cmd->hasInputRedirect && cmd->command->getArg() != "")
					throw SemanticError("Input is already defined.");
				if (!isFirst && (dynamic_cast<Time*>(cmd->command) != nullptr || dynamic_cast<Date*>(cmd->command) != nullptr))
					throw SemanticError("Time and date cannot be in a pipeline if not first command!");
			}

			runPipeline(parsed, defaultIn, defaultOut);

		}

	}
	catch (const CommandError& e)
	{
		cout << e.what() << endl;
	}
	catch (const SemanticError& e)
	{
		cout << e.what() << endl;
	}
	catch (const SyntaxError& e)
	{
		cout << e.what() << endl;
	}

	if (parsed != nullptr)
	{
		for (int i = 0; i < (int)parsed->commands.size(); i++)
		{
			delete parsed->commands[i]->command;
			delete parsed->commands[i];
		}
		delete parsed;
	}
}

void Interpreter::runPipeline(ParsedPipeline* parsed, istream& defaultIn, ostream& defaultOut)
{
	string previousOutput = "";
	//vector<string> stageOutputs(parsed->commands.size());  

	for (int i = 0; i < (int)parsed->commands.size(); i++)
	{
		ParsedLine* cmd = parsed->commands[i];

		ifstream fin;
		ofstream fout;
		istringstream pipeInput(previousOutput);
		ostringstream pipeOutput;

		istream* in = nullptr;
		ostream* out = nullptr;

		bool first = (i == 0);
		bool last = (i == (int)parsed->commands.size() - 1);

		if (first)
		{
			if (cmd->hasInputRedirect)
			{
				fin.open(cmd->inputFile);
				if (!fin.is_open())
				{
					throw SemanticError("File with this name does not exist: " + cmd->inputFile);
				}
				in = &fin;
			}
			else
			{
				in = &defaultIn;
			}
		}
		else
		{
			in = &pipeInput;
		}

		if (last)
		{
			if (cmd->hasOutputRedirect)
			{
				if (cmd->appendOutput)
					fout.open(cmd->outputFile, ios::app);
				else
					fout.open(cmd->outputFile, ios::trunc);

				if (!fout.is_open())
				{
					throw SemanticError("Could not open output file: " + cmd->outputFile);
				}
				out = &fout;
			}
			else
			{
				out = &defaultOut;
			}
		}
		else
		{
			out = &pipeOutput;
		}

		cmd->command->execute(*in, *out);
		/* PipeRef logika :
		PipeRef* ref = dynamic_cast<PipeRef*>(cmd->command);
		if (ref != nullptr) {
		    int n = stoi(ref->getArg());
		    if (n < 1 || n > i) throw SemanticError("Invalid pipeline reference!");
		    Command* targetCommand = parsed->commands[n - 1]->command;
		    istringstream refInput(stageOutputs[n - 1]);
		    targetCommand->execute(refInput, *out);
		} else {
		    cmd->command->execute(*in, *out);
		}
		*/
		if (!last)
		{
			previousOutput = pipeOutput.str();
			//stageOutputs[i] = pipeOutput.str(); 
		}
	}
}

void Interpreter::run()
{
	string line;

	while (true)
	{
		cout << this->getPrompt() << " "; // print prompt
		line = reader->getLine(); // read line
		if (reader->isEof() && line.empty()) break; // check eof

		this->executeLine(line,cin,cout);
		reader->clearInput();
	}
}
	
