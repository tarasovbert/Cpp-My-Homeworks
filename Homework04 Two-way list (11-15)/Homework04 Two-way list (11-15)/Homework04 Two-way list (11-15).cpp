#include <iostream>	
using namespace std;

template <typename T>
class List
{
	struct Node
	{
		T data;
		Node* prev = nullptr;
		Node* next = nullptr;

		Node(T data_, Node* prev_ = nullptr, Node* next_ = nullptr)
			:data{ data_ }, prev{ prev_ }, next{ next_ }
		{
		}
	};

	Node* head = nullptr;
	Node* tail = nullptr;
	int	size = 0;

	void clearList() {
		Node* nextHead = nullptr;
		while (head) {
			nextHead = head->next;
			delete head;
			head = nextHead;
		}
		size = 0;
	}

	void copyList(const List& obj)
	{
		if (&obj != this) {
			Node* curr = obj.head;
			while (curr) {
			pushBack(curr->data);
			curr = curr->next;				
			}
		}
	}

	Node* searchValue(T value)
	{
		if (head) {
			Node* curr = head;
			while (curr) {
				if (curr->data == value)
					return curr;
				curr = curr->next;
			}
			return nullptr;
		}
	}

public:

	template <typename T>
	friend ostream& operator<<(ostream& os, const List<T>& obj);  

	List() = default;

	List(const T& data)
		: head{ new Node(data) }, tail(head), size{ 1 }
	{
	}

	void pushFront(const T& data)
	{
		if (head) {
			head->prev = new Node(data, nullptr, head);
			head = head->prev;
		}
		else
			head = tail = new Node(data);
		++size;
	}

	void pushBack(const T& data)
	{
		if (head) {
			tail->next = new Node(data, tail);
			tail = tail->next;
		}
		else
			head = tail = new Node(data);
		++size;
	}

	T popFront()
	{
		if (head) {
			T popped = head->data;
			Node* curr = head->next;
			delete head;
			head = curr;
			--size;
			return popped;
		}
		return T();
	}

	T popBack()
	{
		if (head) {
			T popped = tail->data;
			if (size == 1) {
				clearList();
				return popped;
			}
			Node* curr = tail->prev;
			delete tail;
			tail = curr;
			tail->next = nullptr;
			if (!tail)
				head = nullptr;
			--size;
			return popped;
		}
		return T();
	}

	bool deleteNode(T data)
	{
		if (head) {
			Node* nodeToDelete = searchValue(data);
			if (nodeToDelete) {
				if (nodeToDelete == head) {
					popFront();
				}

				else if (nodeToDelete == tail) {
					popBack();
				}
				else {
					Node* prv = nodeToDelete->prev;
					Node* nxt = nodeToDelete->next;
					delete nodeToDelete;
					prv->next = nxt;
					nxt->prev = prv;
				}
				--size;
				return true;
			}
		}
		return false;
	}

	int getSize() const
	{
		return size;
	}

	static int menuTwoWayList()
	{
		int answer = 0;
		int items = 8;
		while (answer <= 0 || answer > items) {
			cout << "1. Push the node to the front of the list.\n"
				"2. Push the node to the back of the list.\n"
				"3. Pop the node from the front of the list.\n"
				"4. Pop the node from the back of the list.\n"
				"5. Show the quantity of nodes.\n"
				"6. Delete the node.\n"
				"7. Show the list.\n"
				"8. Exit.\n\n";
			answer = getAnswerInt(1, items);
		}
		return answer;
	}
};

int getAnswerInt(int NegativeBoard = INT_MIN, int PositiveBoard = INT_MAX) {
	int answerInt = 0;
	const int lengthForInt = 9;
	char answerString[lengthForInt + 1];
	while (answerInt < NegativeBoard || answerInt > PositiveBoard) {
		cin.getline(answerString, lengthForInt);
		answerInt = atoi(answerString);
		if (answerInt < NegativeBoard || answerInt > PositiveBoard)
			cout << "Error! Enter again: ";
	}
	return answerInt;
}

template<typename T>
ostream& operator<<(ostream& os, const List<T>& obj)
{
	if (!obj.head)
		os << "\b empty.\n";
	List<T>::Node* curr = obj.head;
	while (curr) {
		os << curr->data << " ";
		curr = curr->next;
	}
	return os;
}

int main() {
	cout << " \n\t\t Two-way list.\n Press \"Enter\"";
	List<int> ListInt;
	while (true) {
		cin.ignore();
		int switchAnswer = List<int>::menuTwoWayList();
		if (switchAnswer == 8)
			break;
		int caseAnswer;
		switch (switchAnswer) {
		case 1: {
			cout << "Enter a value (type int): ";
			cin >> caseAnswer;
			ListInt.pushBack(caseAnswer);
			break;
		}
		case 2: {
			cout << "Enter a value (type int): ";
			cin >> caseAnswer;
			ListInt.pushBack(caseAnswer);
			break;
		}
		case 3: {
			cout << "The node with value " << ListInt.popFront() << " popped from the list.\n";
			break;
		}
		case 4: {
			cout << "The node with value " << ListInt.popBack() << " popped from the list.\n";
			break;
		}
		case 5: {
			caseAnswer = ListInt.getSize();
			if (!caseAnswer)
				cout << "The list is empty.\n";
			else if (caseAnswer == 1)
				cout << "There is " << caseAnswer << " node in the list.\n";
			else
				cout << "There are " << caseAnswer << " nodes in the list.\n";
			break;
		}
		case 6: {
			cout << "Enter the value of node you want to delete: ";
			cin >> caseAnswer;
			if (ListInt.deleteNode(caseAnswer))
				cout << "The node with the value " << caseAnswer << "deleted.\n";
			else
				cout << "There isn't a node with the value " << caseAnswer << " in the list.\n";
			break;
		}
		case 7: {
			cout << "The list is: \n" << ListInt << endl;
			break;
		}
		}
	}
	return 0;
}