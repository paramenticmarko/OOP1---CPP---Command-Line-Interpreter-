#ifndef _date_h_
#define _date_h_

#include "Command.h"

class Date :public Command {
public:
	using Command::Command;

	virtual void execute(istream& in,ostream& out) override;
};

#endif