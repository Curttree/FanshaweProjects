#pragma once

template <class T>
// Custom doubly-linked implementation.
class cCurtLinkedList {
public:
	cCurtLinkedList() {
		this->head = nullptr;
		this->current = this->head;
		this->tail = this->current;
		this->size = 0;
	}
	cCurtLinkedList(T _head) {
		cCurtNode* newHead = new cCurtNode();
		newHead->data = _head;
		this->head = newHead;
		this->current = newHead;
		this->tail = newHead;
		this->size = 1;
	}
	virtual ~cCurtLinkedList() {

	}

	class cCurtNode {
	public:
		cCurtNode() {};
		cCurtNode(T _data) {
			this->data = _data;
		}
		T data;
		cCurtNode* previous;
		cCurtNode* next;
	};

	cCurtNode* head;
	cCurtNode* current;
	cCurtNode* tail;

	bool isEmpty() {
		return this->size == 0;
	}

	T getCurrent() {
		if (this->current == nullptr) {
			return T();
		}
		T result = this->current->data;
		return result;
	}

	//Inserts at the current node.
	void insert(T _data) {
		cCurtNode* newNode = new cCurtNode(_data);
		if (this->head == nullptr) {
			//inserting into empty list.
			this->head = newNode;
			this->current = newNode;
			this->tail = newNode;
		}
		else {
			cCurtNode* oldCurrent = this->current;
			this->current = newNode;
			newNode->next = oldCurrent;

			if (oldCurrent->previous != nullptr) {
				//There was something before this node. Make it previous of our new node.
				newNode->previous = oldCurrent->previous;

				//We may be able to get away without the check, but as long as nothing went wrong, this should always be set.
				if (newNode->previous->next != nullptr) {
					newNode->previous->next = newNode;
				}
			}

			oldCurrent->previous = newNode;

			if (this->head == oldCurrent) {
				this->head = newNode;
			}

			//Don't bother checking tail. Since we shift down, old node should still be the tail.
		}
		size++;
		return;
	}

	//Inserts at the tail, but do not update current.
	void insertAtTail(T _data) {
		cCurtNode* newNode = new cCurtNode(_data);
		if (this->tail == nullptr) {
			//inserting into empty list.
			this->head = newNode;
			this->current = newNode;
			this->tail = newNode;
		}
		else {
			this->tail->next = newNode;
			newNode->previous = this->tail;
			this->tail = newNode;
		}
		size++;
		return;
	}

	//Deletes the current node.
	void deleteNode() {
		if (this->current == nullptr) {
			//Current node has already been deleted. Do nothing.
			return;
		}
		if (this->current->previous != nullptr) {
			this->current->previous->next = this->current->next;
		}
		if (this->current->next != nullptr) {
			this->current->next->previous = this->current->previous;
		}
		//Now that we have removed our node from the list, adjust our head, tail, current and remove the node.
		cCurtNode* toRemove = this->current;
		if (this->current->previous != nullptr) {
			moveBackwards();
			// If we are at the tail, shift the tail back as well.
			if (this->tail == toRemove) {
				this->tail = this->current;
			}
		}
		else if (this->current->next != nullptr) {
			//Can't shift back any more, shift forward instead
			moveForward();
			// If we are at the head, shift the head forward as well.
			if (this->head == toRemove) {
				this->head = this->current;
			}
		}
		else {
			//Nothing forward or backwards. This must be the last node.
			this->current = nullptr;
			this->head = nullptr;
			this->tail = nullptr;
		}

		delete toRemove;
		size--;
	}

	//Replaces the data associated with the current node with the provided data.
	void replace(T _data) {
		this->current->data = _data;
	}

	//Deletes all nodes.
	void empty() {
		if (this->size <= 0) {
			return;
		}

		moveToHead();
		while (this->size > 0) {
			deleteNode();
		}
	}

	cCurtNode* moveForward() {
		if (this->current == nullptr || this->current->next == nullptr) {
			return nullptr;
		}
		this->current = this->current->next;
		return this->current;
	}
	cCurtNode* moveBackwards() {
		if (this->current == nullptr || this->current->previous == nullptr) {
			return nullptr;
		}
		this->current = this->current->previous;
		return this->current;
	}
	cCurtNode* moveToHead() {
		this->current = this->head;
		return this->current;
	}
	cCurtNode* moveToTail() {
		this->current = this->tail;
		return this->current;
	}

	//Moves forward until we find the provided data. Returns false if we hit the end of the list without finding it.
	bool moveForwardTo(T data) {
		if (this->current != nullptr && this->current->data == data) {
			//Already at the data.
			return true;
		}
		cCurtNode* oldCurrent = this->current;
		bool atEnd = false;
		while (!atEnd) {
			if (this->current->next == nullptr) {
				atEnd = true;
			}
			else {
				//Next exists, move forward.
				cCurtNode* next = moveForward();
				if (next->data == data) {
					//We found our result. Keep the next current value.
					return true;
				}
			}
		}
		//We reached the end without finding our data.
		this->current = oldCurrent;
		return false;
	}

	//Moves backward until we find the provided data. Returns null if we hit the end of the list without finding it.
	bool moveBackwardsTo(T data) {
		if (this->current != nullptr && this->current->data == data) {
			//Already at the data.
			return true;
		}
		cCurtNode* oldCurrent = this->current;
		bool atStart = false;
		while (!atStart) {
			if (this->current->previous == nullptr) {
				atStart = true;
			}
			else {
				//Next exists, move backwards.
				cCurtNode* prev = moveBackwards();
				if (prev->data == data) {
					//We found our result. Keep the next current value.
					return true;
				}
			}
		}
		//We reached the end without finding our data.
		this->current = oldCurrent;
		return false;
	}

	//Slower way to move to exactly the data that we are looking for.
	bool moveTo(T data) {
		if (moveForwardTo(data) || moveBackwardsTo(data)) {
			return true;
		}
		return false;
	}

	//Returns the number of nodes in the linked list.
	unsigned int get_size() {
		return size;
	}
private:
	unsigned int size;
};