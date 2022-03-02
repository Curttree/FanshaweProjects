#pragma once
#include <stdlib.h>

class cMathHelper{
	public:
		static cMathHelper* Instance();
		// Generate a random number between 0 and 1
		float getRandom();

		// Generate a random number between zero and a given high value
		float getRandom(float high);

		// Generate a random number in a given range
		float getRandom(float low, float high);
	private:
		cMathHelper();
		static cMathHelper* _instance;
};