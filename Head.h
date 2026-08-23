#ifndef _head_h_
#define _head_h_

#include "Command.h"

class Head : public Command {
public:
	using Command::Command;

	virtual void execute(istream& in, ostream& out) override;
};

#endif

