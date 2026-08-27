#ifndef _exceptions_h_
#define _exceptions_h_

#include <exception>
#include <string>

using namespace std;

class SyntaxError : public exception {
public:
	explicit SyntaxError(const string& msg) : msg("Error! " + msg) {}

	const char* what() const noexcept override
	{
		return msg.c_str(); // conversion to const char*
	}

private:
	string msg;
};

class SemanticError : public exception {
public:
	explicit SemanticError(const string& msg) : msg("Error! " + msg) {}

	const char* what() const noexcept override
	{
		return msg.c_str();
	}

private:
	string msg;
};

class CommandError : public exception {
public:
	explicit CommandError(const string& msg) : msg("Error! " + msg) {}

	const char* what() const noexcept override
	{
		return msg.c_str();
	}

private:
	string msg;
};

#endif