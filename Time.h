#ifndef _time_h_
#define _time_h_

#include "Command.h"

class Time :public Command {
public:
	using Command::Command;

	virtual void execute(istream& in, ostream& out) override;
};

#endif

