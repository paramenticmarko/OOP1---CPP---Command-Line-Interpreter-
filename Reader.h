#ifndef _reader_h_
#define _reader_h_

#include <string>
#include <istream>

using namespace std;

class Reader {
public:
	explicit Reader(istream* input);
	virtual ~Reader() {}

	virtual string getLine() const;
	bool isEof() const;
	void clearInput();

protected:
	istream* input;
};

class ConsoleReader : public Reader {
public:
	ConsoleReader();

	virtual string getLine() const override;
};

class FileReader : public Reader {
public:
	explicit FileReader(const string& path);
	~FileReader();
};

#endif