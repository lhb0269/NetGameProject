#pragma once

template<class T>
struct Node {
	T* body;
	Node<T>* next;

	Node();
	~Node();
};


template<class T>
class Queue {
	Node<T>* head;
	Node<T>* tail;

public:
	Queue();
	~Queue();
	void put(T*);
	T* get();
	bool isEmpty();
};