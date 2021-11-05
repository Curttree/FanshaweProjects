#pragma once
#include <string>
#include <vector>
#include <extern/glm/vec4.hpp>

class iMediator;

struct sMessage
{
public:
	sMessage() {};
	std::string command;
	std::vector< std::string > vec_sData;
	std::vector< float > vec_fData;
	std::vector< glm::vec4 > vec_v4Data;
};

class iMessage
{
public:
	virtual ~iMessage() {};
	virtual bool RecieveMessage(sMessage theMessage) = 0;
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse) = 0;
	virtual bool SetReciever(iMediator* pTheReciever) = 0;
};
