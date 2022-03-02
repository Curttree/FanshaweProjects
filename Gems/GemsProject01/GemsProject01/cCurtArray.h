#pragma once
template <class T>
// Custom 'smart' array implementation. Meant to somewhat mirror STL's vector.
class cCurtArray {
public:
	cCurtArray();
	cCurtArray(unsigned int size);
	virtual ~cCurtAray() {};

	T getAtIndex(unsigned int index);
	T setAtIndex(unsigned int index, T new_data);

	void push_back(T entry);

	//Clears the array
	void clear();

	//Returns whether the current array has no non-null values.
	bool isEmpty();

	//Removes the value if one exists. Returns false if the provided index was null.
	bool erase(unsigned int index);

	//Returns the number of non-null entries in the array.
	unsigned int get_size();

	//Returns the current maximum capacity of the array (all null and non-null values)
	unsigned int get_capacity();
private:
	// Doubles the size of the array
	void allocateSpace();
	// Increases the size of the array by x ammount.
	void allocateSpace(unsigned int newIndicies);
	void init();
	unsigned int capacity = 0;
	unsigned int DEFAULT_CAPACITY = 10;
	unsigned int size = 0;

	T** data;
};

