#ifndef _reader_h_
#define _reader_h_

#include <string>
#include <istream>

using namespace std;

class Reader {
public:
	Reader(istream* input);
	virtual ~Reader() {}

	virtual string getLine();
	bool isEof();
	void clearInput();

protected:
	istream* input;
};

class ConsoleReader : public Reader {
public:
	ConsoleReader();

	virtual string getLine() override;
};

class FileReader : public Reader {
public:
	FileReader(string path);
	~FileReader();
};

#endif