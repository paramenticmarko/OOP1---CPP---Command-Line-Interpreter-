#include "Command.h"
#include "Reader.h"

#include<iostream>

// constructors.

Command::Command(string opt, string arg, string ext) :
	opt(opt), arg(arg), ext(ext) {
}

Command::Command(string opt, string arg) :
	opt(opt), arg(arg), ext("") {
}

Command::Command(string arg) :
	opt(""), arg(arg), ext("") {
}

Command::~Command() {} // destr

string Command::commandArg(const string& arg, istream& in)
{
	if (arg == "") // read arg from stdin
	{
		string text = "";

		while (true)
		{
			string line;
			getline(in, line);

			if (!in) break;

			text += line;
			if (in.eof()) break;
			text += '\n';
		}

		if (!text.empty() && text.back() == '\n') text.pop_back();

		return text;
	}
	else if (arg[0] == '"' && arg.back() == '"') // arg is a string
	{
		string text = "";

		for (char c : arg)
		{
			if (c == '"') continue;
			text += c;
		}

		return text;
	}
	else // arg is st else (e.g. filename)
	{
		FileReader* reader = new FileReader(arg);
		string text = "";

		while (true)
		{
			string line = reader->getLine();
			if (reader->isEof() && line.empty()) break;
			text += line + '\n';
			if (reader->isEof()) break;
		}

		if (!text.empty()) text.pop_back();
		delete reader;
		return text;
	}
}

