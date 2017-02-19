#pragma once 

template <typename T>
class List
{ 
	struct Node
	{
		Node* next;
		T data;

		Node(const T& data_, Node* next_ = nullptr)
			:data{ data_ }, next{ next_ }
		{
		}
	};

	Node* head = nullptr;//pointer
	Node* tail = nullptr;//pointer
	int size = 0;
private:
	void removeAllElement();  
	void copyList(const List& obj);
	Node* search(T value);

public:
	template <typename T>
	friend ostream& operator<<(ostream& oss, const List<T>& obj); 
	List() = default;  
	List(const T& data);  
	List(const List& obj);	
	List operator=(const List& obj);  
	~List();				  
	void pushFront(const T& data_);	
	T popBack();			
	T popFront();	 
	void pushBack(const T& data_);
	bool searchValue(T value);	
	bool deleteElement(T value);
};
