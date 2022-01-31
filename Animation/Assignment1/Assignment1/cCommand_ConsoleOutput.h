#pragma once
#include "iCommand.h"
#include <string>

class cCommand_ConsoleOutput : public iCommand {
public:
	cCommand_ConsoleOutput(std::string _message);
	virtual void Execute();
private:
	std::string message;
};