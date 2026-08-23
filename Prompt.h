#ifndef _prompt_h_
#define _prompt_h_

#include "Command.h"

class Prompt : public Command {
public:
	using Command::Command;

	virtual void execute(istream& in, ostream& out) override;
};

#endif
