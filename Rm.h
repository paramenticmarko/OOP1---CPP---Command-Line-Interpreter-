#ifndef _rm_h_
#define _rm_h_

#include "Command.h"

class Rm : public Command {
public:
	using Command::Command;

	virtual void execute(istream& in, ostream& out) override;
};

#endif

