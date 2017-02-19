#include <iostream>
#include "Class List.h"

	List::List() = default;

	List::List(const T& data) :
		head{ new Node(data) }, tail{ head }, size{ 1 }
	{

	}
	void List::removeAllElement()
	{
		Node* nxt = nullptr;
		while (head) {
			nxt = head->next;
			delete head;
			head = nxt;
		}
		size = 0;
	}

	void List::copyList(const List& obj)
	{
		Node* curr = obj.head;
		while (curr) {
			pushBack(curr->data);
			curr = curr->next;
		}
	}

	Node* List::search(T value)
	{
		Node* curr = head;
		while (curr)
		{
			if (curr->data == value)
				return curr;
			curr = curr->next;
		}
		return nullptr;
	}


	List::List(const List& obj)
	{
		copyList(obj);
	}

	List::List operator=(const List& obj)
	{
		if (&obj != this) {
			removeAllElement();
			copyList(obj);
		}
		return *this;
	}

	List::~List()
	{
		removeAllElement();
	}

	void List::pushFront(const T& data_)
	{
		if (head)
			head = new Node(data_, head);
		else
			head = tail = new Node(data_);
		++size;
	}

	T List::popBack()
	{
		if (head)
		{
			Node* curr = head;
			T popped = tail->data;
			if (size > 1)
			{
				while (curr->next != tail)
				{
					curr = curr->next;
				}
				tail = curr;
				delete tail->next;
				tail->next = nullptr;
			}
			else
			{
				delete head;
				head = tail = nullptr;
			}
			--size;
			return popped;
		}
		return T();
	}

	T List::popFront()
	{
		if (head)
		{
			T popped = head->data;
			Node* curr = head->next;
			delete head;
			head = curr;
			--size;
			return popped;
		}
		return T();
	}

	void List::pushBack(const T& data_)
	{
		if (head) {
			tail->next = new Node(data_);
			tail = tail->next;
		}
		else
			head = tail = new Node(data_);
		++size;
	}

	bool List::searchValue(T value)
	{
		Node* curr = head;
		while (curr)
		{
			if (curr->data == value)
				return true;
			curr = curr->next;
		}
		return false;
	}

	bool List::deleteElement(T value)
	{
		Node* curr = head;
		Node* prev = nullptr;
		while (curr)
		{
			if (curr->data == value)
			{
				if (prev)
					prev->next = curr->next;
				else
					head = curr->next;
				delete curr;
				--size;
				return true;
			}
			prev = curr;
			curr = curr->next;
		}
		return false;
	}

};