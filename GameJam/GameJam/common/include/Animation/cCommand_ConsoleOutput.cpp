#include "cCommand_ConsoleOutput.h"
#include <iostream>

cCommand_ConsoleOutput::cCommand_ConsoleOutput(std::string _message) {
	message = _message;
}

void cCommand_ConsoleOutput::Execute() {
	std::cout << message << std::endl;
}