#pragma once
template <class T>
// Custom 'smart' array implementation. Meant to somewhat mirror STL's vector.
class cCurtArray {
public:
	cCurtArray();
	cCurtArray(unsigned int size);
	cCurtArray(T[] c_array);
	virtual ~cCurtAray();

	T getAtIndex(unsigned int index);
	T setAtIndex(unsigned int index);

	void push_back(T entry);

	//Clears the array
	void clear();

	//Returns whether the current array has no non-null values.
	bool isEmpty();

	//Removes the value if one exists. Returns false if the provided index was null.
	bool erase(unsigned int index);

	//Returns the number of non-null entries in the array.
	unsigned int size();

	//Returns the current maximum capacity of the array (all null and non-null values)
	unsigned int capacity();
private:
	// Increases the size of the array by x ammount.
	void allocateSpace(unsigned int newIndicies);
};

