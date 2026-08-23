#ifndef _touch_h_
#define _touch_h_

#include "Command.h"

class Touch : public Command {
public:
	using Command::Command;

	virtual void execute(istream& in, ostream& out) override;
};

#endif

