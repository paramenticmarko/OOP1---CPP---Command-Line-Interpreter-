#ifndef _echo_h_
#define _echo_h_

#include "Command.h"

class Echo : public Command {
public:
	using Command::Command;

	virtual void execute(istream& in, ostream& out) override;
};

#endif