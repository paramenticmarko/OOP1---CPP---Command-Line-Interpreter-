#ifndef _tr_h_
#define _tr_h_

#include "Command.h"

class Tr : public Command {
public:
	using Command::Command;

	virtual void execute(istream& in,ostream& out) override;
};

#endif

