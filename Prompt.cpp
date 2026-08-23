#include "Prompt.h"
#include "Interpreter.h"

void Prompt::execute(istream& in, ostream& out) {
	Interpreter::getInstance().setPrompt(this->commandArg(this->arg,in));
}
