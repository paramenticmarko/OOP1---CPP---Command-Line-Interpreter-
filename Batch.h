#ifndef _batch_h_
#define _batch_h_

#include "Command.h"

class Batch : public Command {
public:
	using Command::Command;

	virtual void execute(istream& in, ostream& out) override;
};

#endif

