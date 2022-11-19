#include "global.h"
#include "Queue.h"


template<class T>
inline Node<T>::Node()
{
	body = nullptr;
	next = nullptr;
}

template<class T>
inline Node<T>::~Node()
{
}

template<class T>
Queue<T>::Queue()
{
	head = nullptr;
	tail = nullptr;
}

template<class T>
Queue<T>::~Queue()
{
	T* cur;
	while ( (cur = get()) != nullptr) {
		delete cur;
	}
}

template<class T>
void Queue<T>::put(T* thing)
{
	if (head == nullptr) {
		head = new Node<T>;
		head->body = thing;
		tail = head;
	}
	else {
		tail->next = new Node<T>;
		tail = tail->next;
		tail->body = thing;
	}
}

template<class T>
T * Queue<T>::get()
{
	T* result = nullptr;
	if (head != nullptr) {
		result = head->body;
		Node<T>* cur = head;
		head = head->next;
		delete cur;
		if (head == nullptr)
			tail = nullptr;
	}
	return result;
}

template<class T>
bool Queue<T>::isEmpty()
{
	return head == nullptr ? true : false;
}
