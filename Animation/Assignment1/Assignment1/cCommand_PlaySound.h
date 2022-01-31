#pragma once
#include "iCommand.h"

class cCommand_PlaySound : public iCommand {
public:
	cCommand_PlaySound(int _index);
	virtual void Execute();
private:
	int index;
};