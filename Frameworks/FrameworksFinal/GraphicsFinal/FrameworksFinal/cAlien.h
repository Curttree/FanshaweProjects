#pragma once
#include "cEntity.h"

class cAlien : public cEntity {
public:
	cAlien(int type);
	cAlien(std::string _pose1, std::string _pose2);
	virtual ~cAlien();
private:
	std::string pose1;
	std::string pose2;
	int currentPose = 1;
};