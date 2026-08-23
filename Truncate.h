#ifndef _truncate_h_
#define _truncate_h_

#include "Command.h"

class Truncate : public Command {
public:
	using Command::Command;

	virtual void execute(istream& in, ostream& out) override;
};

#endif

