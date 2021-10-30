#pragma once
#include <string>
#include <vector>

struct sMessage
{
public:
	sMessage() {};
	std::string command;
};

class iMessage
{
public:
	virtual ~iMessage() {};
	virtual void RecieveMessage(sMessage theMessage) = 0;
	virtual void SetReciever(iMessage* pTheReciever) = 0;
};
