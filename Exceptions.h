#ifndef _exceptions_h_
#define _exceptions_h_

#include <exception>
#include <string>

using namespace std;

class SyntaxError : public exception
{
private:
	string msg;

public:
	SyntaxError(string msg) : msg("Error! " + msg) {}

	const char* what() const noexcept override
	{
		return msg.c_str(); // conversion to const char*
	}
};

class SemanticError : public exception
{
private:
	string msg;

public:
	SemanticError(string msg) : msg("Error! " + msg) {}

	const char* what() const noexcept override
	{
		return msg.c_str();
	}
};

class CommandError : public exception
{
private:
	string msg;

public:
	CommandError(string msg) : msg("Error! " + msg) {}

	const char* what() const noexcept override
	{
		return msg.c_str();
	}
};

#endif