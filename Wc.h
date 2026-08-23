#ifndef _wc_h_
#define _wc_h_

#include "Command.h"

class Wc : public Command {
public:
	using Command::Command;

	virtual void execute(istream& in, ostream& out) override;
};

#endif

