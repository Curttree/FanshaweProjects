#include "cCurtArray.h"

template <class T>
cCurtArray<T>::cCurtArray() {
	this->capacity = DEFAULT_CAPACITY;
	init(this->capacity);
}

template <class T>
cCurtArray<T>::cCurtArray(unsigned int _size) {
	this->capacity = _size;
	init(this->capacity);
}

template <class T>
T cCurtArray<T>::getAtIndex(unsigned int index) {
	if (index > size) {
		return 0;
	}

	T result = *(this->data[index]);

	return result;
}

template <class T>
T cCurtArray<T>::setAtIndex(unsigned int index, T new_data) {
	// Potentially dangerous. Might want to default to push_back if used outside of personal projects.
	// Leaving the option for now should I have a need for this.
	if (index > capacity) {
		allocateSpace(index - capacity);
	}

	if (this->data[index]) {
		delete this->data[index];
	}

	*(this->data[index]) = new_data;

	return;
}

template <class T>
T cCurtArray<T>::operator[](unsigned int index) {
	return this->getAtIndex(index);
}

template <class T>
T cCurtArray<T>::begin() {
	return data[0];
}

template <class T>
T cCurtArray<T>::end() {
	return data[size];
}

template <class T>
void cCurtArray<T>::push_back(T entry) {
	this->data[size] = new T();

	*(this->data[this->size]) = entry;

	size++;

	if (this->size > this->capacity) {
		allocateSpace();
	}

	return;
}

//Clears the array
template <class T>
void cCurtArray<T>::clear() {
	for (unsigned int index = 0; index != this->size; index++) {
		delete this->data[index];
		this->data[index] = 0;
	}

	this->size = 0;

	this->capacity = DEFAULT_CAPACITY;
}

//Returns whether the current array has no non-null values.
template <class T>
bool cCurtArray<T>::isEmpty() {
	return size == 0;
}

//Removes the value if one exists. Returns false if the provided index was null.
//Note: If we repeatedly call erase, we will not impact the capacity of the array. Call clear to reset the capacity.
template <class T>
bool cCurtArray<T>::erase(unsigned int index) {
	//Check if index is null or outside of bounds of array.
	if (index >= size || !this->data[index]) {
		return 0;
	}

	delete this->data[index];

	for (unsigned int x = index; x <= size; x++) {
		this->data[x] = *(this->data[x + 1]);
	}

	//We have shifted all data back. Set index at size to null and decrement size.
	this->data[size] = 0;
	size--;
}

//Returns the number of non-null entries in the array.
template <class T>
unsigned int cCurtArray<T>::get_size() {
	return size;
}

//Returns the current maximum capacity of the array (all null and non-null values)
template <class T>
unsigned int cCurtArray<T>::get_capacity() {
	return capacity;
}

// Doubles the size of the array
template <class T>
void cCurtArray<T>::allocateSpace() {
	this->capacity *= 2;
	init();
}

// Increases the size of the array by x ammount.
template <class T>
void cCurtArray<T>::allocateSpace(unsigned int newIndicies) {
	this->capacity += newIndicies;
	init();
}

template <class T>
void cCurtArray<T>::init() {

	T** tempArray = new T*[this->capacity];

	for (unsigned int index = 0; index != this->capacity; index++) {
		tempArray[index] = 0;
	}

	for (unsigned int index = 0; index != this->size; index++) {
		tempArray[index] = this->data[index];
	}

	this->data = tempArray;

	//TODO: Delete the old array.

	return;
}