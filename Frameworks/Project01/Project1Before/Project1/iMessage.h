#pragma once
#include <string>
#include <vector>
#include <glm/vec4.hpp>

// Pretty sure if passing this in a message is counterintuitive. 
// See if I can refactor rendering to not require this.
class cFireworkObject;

class iMediator;

struct sMessage
{
public:
	sMessage() {};
	std::string command;
	std::vector< int > vec_iData;
	std::vector< float > vec_fData;
	std::vector< glm::vec4 > vec_v4Data;
	std::vector< cFireworkObject* > vec_fireworkData;
};

class iMessage
{
public:
	virtual ~iMessage() {};
	virtual bool RecieveMessage(sMessage theMessage) = 0;
	virtual bool RecieveMessage(sMessage theMessage, sMessage& theResponse) = 0;
	virtual bool SetReciever(iMediator* pTheReciever) = 0;
};
