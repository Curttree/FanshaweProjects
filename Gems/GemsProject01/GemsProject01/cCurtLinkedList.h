#pragma once

template <class T>
// Custom doubly-linked implementation.
class cCurtLinkedList {
public:
	cCurtLinkedList();
	cCurtLinkedList(T _head);
	virtual ~cCurtLinkedList();

	class cCurtNode {
	public:
		cCurtNode();
		cCurtNode(T _data);
		T data;
		cCurtNode* previous;
		cCurtNode* next;
	};

	cCurtNode* head;
	cCurtNode* current;

	//Inserts at the current node.
	void insert(T _data);

	//Deletes the current node.
	void deleteNode();

	//Replaces the data associated with the current node with the provided data.
	void replace(T _data);

	//Deletes all nodes.
	void empty();

	cCurtNode* moveForward();
	cCurtNode* moveBackwards();
	cCurtNode* moveToHead();
	cCurtNode* moveToTail();

	//Moves forward until we find the provided data. Returns null if we hit the end of the list without finding it.
	bool moveForwardTo(T data);

	//Moves backward until we find the provided data. Returns null if we hit the end of the list without finding it.
	bool moveBackwardsTo(T data);

	//Slower way to move to exactly the data that we are looking for.
	bool moveTo(T data);

	//Returns the number of nodes in the linked list.
	unsigned int size();
private:
	unsigned int size;
};