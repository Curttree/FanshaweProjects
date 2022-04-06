#pragma once
template <class T>
// Custom 'smart' array implementation. Meant to somewhat mirror STL's vector.
class cCurtArray {
private:

	unsigned int capacity = 0;
	unsigned int DEFAULT_CAPACITY = 10;
	unsigned int size = 0;

	T** data;
public:
	cCurtArray() {
		this->capacity = DEFAULT_CAPACITY;
		init();
	}

	cCurtArray(unsigned int _size) {
		this->capacity = _size;
		init();
	}
	virtual ~cCurtArray() {
	}

	T getAtIndex(unsigned int index) {
		if (index >= capacity) {
			return 0;
		}

		T result = *(this->data[index]);

		return result;
	}

	void setAtIndex(unsigned int index, T new_data) {
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

	void push_back(T entry) {
		this->data[size] = new T();

		*(this->data[this->size]) = entry;

		size++;

		if (this->size > this->capacity) {
			allocateSpace();
		}

		return;
	}

	//Clears the array
	void clear() {
		for (unsigned int index = 0; index != this->size; index++) {
			if (&this->data[index]) {
				delete this->data[index];
				this->data[index] = 0;
			}
		}

		this->size = 0;

		this->capacity = DEFAULT_CAPACITY;
	}

	//Returns whether the current array has no non-null values.
	bool isEmpty() {
		return size == 0;
	}

	//Removes the value if one exists. Returns false if the provided index was null.
	bool erase(unsigned int index) {
		//Check if index is null or outside of bounds of array.
		if (index >= size || !this->data[index]) {
			return 0;
		}

		delete this->data[index];

		for (unsigned int x = index; x <= size; x++) {
			this->data[x] = this->data[x + 1];
		}

		//We have shifted all data back. Set index at size to null and decrement size.
		this->data[size] = 0;
		size--;
	}

	//Returns the number of non-null entries in the array.
	unsigned int get_size() {
		return size;
	}

	//Returns the current maximum capacity of the array (all null and non-null values)
	unsigned int get_capacity() {
		return capacity;
	}

	T begin() {
		return data[0];
	}

	T end() {
		return data[size];
	}

	T operator[](unsigned int index) {
		return this->getAtIndex(index);
	}
private:
	// Doubles the size of the array
	void allocateSpace() {
		this->capacity *= 2;
		init();
	}

	// Increases the size of the array by x ammount.
	void allocateSpace(unsigned int newIndicies) {
		this->capacity += newIndicies;
		init();
	}

	void init() {

		T** tempArray = new T * [this->capacity];

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

};

