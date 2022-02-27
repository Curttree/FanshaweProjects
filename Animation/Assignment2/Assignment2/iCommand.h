#pragma once

class iCommand {
public:
	virtual ~iCommand() {}
	virtual void Execute() = 0;
protected:
	iCommand() {}
};