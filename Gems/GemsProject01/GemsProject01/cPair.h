#pragma once
//Essentially the cPair_Templated class. Couldn't think of meaningful ways to change it.
template <class T, class U>
class cPair
{
public:
	cPair() {};
	cPair(T first, U second)
	{
		this->First = first;
		this->Second = second;
	}
	~cPair() {};

	T First;
	U Second;
};
