#include "Reader.h"
#include "Exceptions.h"

#include<string>
#include<iostream>
#include<fstream>

using namespace std;

Reader::Reader(istream* input)
	:input(input) {
}

string Reader::getLine() {
	string line;
	getline(*input, line);
	return line;
}

bool Reader::isEof() {
	return input->eof();
}

void Reader::clearInput()
{
	this->input->clear();
}

ConsoleReader::ConsoleReader()
	:Reader(&cin) {
}

string ConsoleReader::getLine() {
	string line = Reader::getLine();

	if (line.size() > 512) {
		line = line.substr(0, 512);
	}

	return line;
}

FileReader::FileReader(string path)
	:Reader(new ifstream(path)) {
	if (!((ifstream*)input)->is_open())
		throw SemanticError("Ne postoji fajl: " + path);
}

FileReader::~FileReader() {
	delete input;
}