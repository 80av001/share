//============================================================================
// Name        : testcpp.cpp
// Author      : Ibrahim Unalan
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include <iostream>
#include <cassert>


template <typename T>class Node;
template <typename T>class DoubleLinkedList;
//
// Iterator Class
//
template <typename T>
class Iterator {
    private:
        Node<T>* cur_p;
        DoubleLinkedList<T>* list;
        friend class DoubleLinkedList<T>;
    public:
        Iterator(Node<T>* head, DoubleLinkedList<T>* list) : cur_p(head), list(list) {}
        bool ended() const; // checks if iteration is ended

        T& operator*() const; //dereference
        Iterator& operator++(); //pre-increment
		Iterator operator++(int); //post-increment
		Iterator& operator--(); //pre-decrement
		Iterator operator--(int); //post-decrement
		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;
    };

template <typename T>
bool Iterator<T>::ended() const {
	return cur_p == nullptr;
}

template <typename T>
T& Iterator<T>::operator*() const {
	if (cur_p == nullptr)
		throw std::runtime_error("Dereferencing null object");
	return cur_p->data;
}

template <typename T>
Iterator<T>& Iterator<T>::operator++() {
	if (cur_p == nullptr)
		throw std::runtime_error("Incrementing null object");
	cur_p = cur_p->next;
	return *this;
}

template <typename T>
Iterator<T> Iterator<T>::operator++(int) {
	Iterator temp = *this;
	++(*this);
	return temp;
}

template <typename T>
Iterator<T>& Iterator<T>::operator--() {
	if (cur_p == nullptr)
		throw std::runtime_error("decrementing null object");
	cur_p = cur_p->prev;
	return *this;
}

template <typename T>
Iterator<T> Iterator<T>::operator--(int) {
	Iterator temp = *this;
	--(*this);
	return temp;
}

template <typename T>
bool Iterator<T>::operator==(const Iterator& other) const {
	return cur_p == other.cur_p;
}

template <typename T>
bool Iterator<T>::operator!=(const Iterator& other) const {
	return !(*this == other);
}



template <typename T>
class Node {
public:
    T data;
    Node<T>* prev;
    Node<T>* next;

    Node(T value) : data(value), prev(nullptr), next(nullptr) {}
};



template <typename T>
class DoubleLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;
    friend class Iterator<T>;
public:
    DoubleLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~DoubleLinkedList() { clear(); }

    void pushFrontVal(T value); //Pushes a new node at the head with given value
    void pushBackVal(T value); //Pushes a new node at the tail with given value
    T& getFrontVal() const; // returns the first value
    T& getBackVal() const; // returns the last value
    void insertAfter(const Iterator<T> &it, T value); //Inserts a new node with given value to after given iterator position
    void insertBefore(const Iterator<T> &it, T value); //Inserts a new node with given value to before given iterator position
    void remove(const Iterator<T> &it); // removes specified iterator position from the list
    void popFront(); // Removes the head node
    void popBack(); // Removes the tail node
    void clear(); // Removes all nodes
    int getSize() const; // Returns number of nodes in the list

    Iterator<T> front() const; //returns iterator for the first node
    Iterator<T> back() const; // returns iterator for the last node
};

template <typename T>
T& DoubleLinkedList<T>::getFrontVal() const {
	if (head)
		return head->data;
	throw std::runtime_error("List is empty");
}

template <typename T>
T& DoubleLinkedList<T>::getBackVal() const {
	if (tail)
		return tail->data;
	throw std::runtime_error("List is empty");
}

template <typename T>
void DoubleLinkedList<T>::pushFrontVal(T value) {
	Node<T>* newNode = new Node<T>(value);
	if (head == nullptr) {
		head = tail = newNode;
	} else {
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
	}
	size++;
}

template <typename T>
void DoubleLinkedList<T>::pushBackVal(T value) {
	Node<T>* newNode = new Node<T>(value);
	if (tail == nullptr) {
		head = tail = newNode;
	} else {
		newNode->prev = tail;
		tail->next = newNode;
		tail = newNode;
	}
	size++;
}


template <typename T>
void DoubleLinkedList<T>::insertAfter(const Iterator<T> &it, T value) {
    if (it.cur_p == nullptr)
    	throw std::invalid_argument("can't be inserted after null node");
    Node<T>* newNode = new Node<T>(value);
    newNode->next = it.cur_p->next;
    newNode->prev = it.cur_p;
    if (it.cur_p->next != nullptr) {
    	it.cur_p->next->prev = newNode;
    } else {
    	tail = newNode;
    }
    it.cur_p->next = newNode;
    size++;
}


template <typename T>
void DoubleLinkedList<T>::insertBefore(const Iterator<T> &it, T value) {
    if (it.cur_p == nullptr) {
    	throw std::invalid_argument("can't be inserted before null node");
    }
    Node<T>* newNode = new Node<T>(value);
    newNode->prev = it.cur_p->prev;
    newNode->next = it.cur_p;
    if (it.cur_p->prev != nullptr) {
    	it.cur_p->prev->next = newNode;
    } else {
        head = newNode;
    }
    it.cur_p->prev = newNode;
    size++;
}


template <typename T>
void DoubleLinkedList<T>::remove(const Iterator<T> &it) {
    if (it.cur_p == nullptr)
    	throw std::invalid_argument("can't be removed null node");
    if (it.cur_p->prev != nullptr) {
    	it.cur_p->prev->next = it.cur_p->next;
    } else {
        head = it.cur_p->next;
    }
    if (it.cur_p->next != nullptr) {
    	it.cur_p->next->prev = it.cur_p->prev;
    } else {
        tail = it.cur_p->prev;
    }
    delete it.cur_p;
    size--;
}

template <typename T>
void DoubleLinkedList<T>::popFront() {
    if (head == nullptr) return;
    Node<T>* temp = head;
    head = head->next;
    if (head != nullptr) head->prev = nullptr;
    delete temp;
    size--;
}

template <typename T>
void DoubleLinkedList<T>::popBack() {
    if (tail == nullptr) return;
    Node<T>* temp = tail;
    tail = tail->prev;
    if (tail != nullptr) tail->next = nullptr;
    delete temp;
    size--;
}

template <typename T>
void DoubleLinkedList<T>::clear() {
	while (head != nullptr) {
		Node<T>* temp = head;
		head = head->next;
		delete temp;
	}
	tail = nullptr;
	size = 0;
}


template <typename T>
Iterator<T> DoubleLinkedList<T>::front() const {
	return Iterator<T>(head, (DoubleLinkedList<T>*)this);
}

template <typename T>
Iterator<T> DoubleLinkedList<T>::back() const {
    return Iterator<T>(tail, (DoubleLinkedList<T>*)this);
}

template <typename T>
int DoubleLinkedList<T>::getSize() const {
    return size;
}

template <typename T>
void assertList(DoubleLinkedList<T> &list, T expected[], string testStr) {
	int k = 0;
	std::cout << testStr;
	for (auto it = list.front(); !it.ended(); ++it) {
		std::cout << *it <<"=="<<expected[k] << ",";
		assert(*it == expected[k++]);
	}
	std::cout << " => Passed" << std::endl;
}
void assertExp(bool exp, string testStr) {
	std::cout <<testStr ;
	assert(exp);
	std::cout << " => Passed" << std::endl;
}

void unitTestDoubleLinkedList() {
	DoubleLinkedList<int> list;

	list.pushFrontVal(1);
	list.pushFrontVal(2);
	list.pushFrontVal(3);
	{
		int expected[] = {3, 2, 1};
		assertList(list, expected, "pushFrontVal()");

		assertExp(list.getSize() == 3, "getSize()");
		assertExp(list.getFrontVal() == 3, "getFrontVal()");
		assertExp(list.getBackVal() == 1, "getBackVal()");
		assertExp(*(list.front()) == 3, "front()");
		assertExp(*(list.back()) == 1, "back()");
		assertExp(*(++list.front()) == 2, "Iterator::Pre-increment, Dereferencing");
		assertExp(*(--list.back()) == 2, "Iterator::Pre-decrement, Dereferencing");
		assertExp(list.front() != list.back(), "Iterator::Not Equal");
		Iterator<int> it = list.front();
		assertExp(list.front() == it, "Iterator::Equal");
	}

	list.pushBackVal(4);
	list.pushBackVal(5);
	list.pushBackVal(6);
	{
		int expected[] = {3, 2, 1, 4, 5, 6};
		assertList(list, expected, "pushBackVal()");
	}

	list.insertAfter(list.front(), 7);
	list.insertAfter(list.back(), 8);
	list.insertAfter(list.front(), 9);
	{
		int expected[] = {3, 9,7, 2, 1, 4, 5, 6, 8};
		assertList(list, expected, "insertAfter()");
	}

	list.insertBefore(list.front(), 10);
	list.insertBefore(list.back(), 11);
	{
		int expected[] = {10, 3, 9,7, 2, 1, 4, 5, 6, 11, 8};
		assertList(list, expected, "insertBefore()");
	}

	list.popBack();
	list.popBack();
	{
		int expected[] = {10, 3, 9,7, 2, 1, 4, 5, 6};
		assertList(list, expected, "popBack()");
	}

	list.popFront();
	list.popFront();
	{
		int expected[] = {9,7, 2, 1, 4, 5, 6};
		assertList(list, expected, "popFront()");
	}

	list.remove(list.front());
	list.remove(list.back());
	{
		int expected[] = {7, 2, 1, 4, 5};
		assertList(list, expected, "remove()");
	}

	list.clear();
	{
		int expected[] = {};
		assertList(list, expected, "clear()");
	}


	std::cout << "unitTestDoubleLinkedList::All tests passed successfully!" << std::endl;
}


int main() {

	unitTestDoubleLinkedList();

	return 0;
}
