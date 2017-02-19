#include <iostream>	
using namespace std;

class List
{
	struct Entry
	{
		char* key = nullptr;
		char* value = nullptr;
		Entry* next = nullptr;

		Entry() = default;

		Entry(const char* key_, const char* value_, Entry* next_ = nullptr)
			:next{ next_ }
		{
			int size = strlen(key_) + 1;
			key = new char[size];
			strcpy_s(key, size, key_);
			size = strlen(value_) + 1;
			value = new char[size];
			strcpy_s(value, size, value_);
		}

		~Entry()
		{
			if (key) {
				delete[] key;
				key = nullptr;
				delete[] value;
				value = nullptr;
			}
		}
	};

	Entry* head = nullptr;//pointer
	Entry* tail = nullptr;//pointer
	int size = 0;

	void removeAllElements()
	{
		Entry* nxt = nullptr;
		while (head) {
			nxt = head->next;
			delete head;
			head = nxt;
		}
		size = 0;
	}

	void copyList(const List& obj)
	{
		Entry* curr = obj.head;
		while (curr) {
			pushBack(curr->key, curr->value);
			curr = curr->next;
		}
	}

	Entry* search(char* key_)
	{
		Entry* curr = head;
		while (curr)
		{
			if (!strcmp(curr->key, key_))
				return curr;
			curr = curr->next;
		}
		return nullptr;
	}

public:
	friend ostream& operator<<(ostream& os, const List& obj);
	friend ostream& operator<<(ostream& os, const List::Entry& obj);
	friend class HashTableChains;
	friend class HashTableOpenedAddressing;

	List() = default;

	List(const char* key, const char* value) :
		head{ new Entry(key, value) }, tail{ head }, size{ 1 }
	{
	}

	List(const List& obj)
	{
		copyList(obj);
	}

	List operator=(const List& obj)
	{
		if (&obj != this) {
			removeAllElements();
			copyList(obj);
		}
		return *this;
	}

	~List()
	{
		removeAllElements();
	}

	void pushBack(const char* key, const char* value)
	{
		if (head) {
			tail->next = new Entry(key, value);
			tail = tail->next;
		}
		else
			head = tail = new Entry(key, value);
		++size;
	}

inline Entry* getHead() const
	{
		return head;
	}

	bool deleteEntry(char* key_)
	{
		Entry* curr = head;
		Entry* prev = nullptr;
		while (curr)
		{
			if (!strcmp(curr->key, key_))
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

ostream& operator<<(ostream& os, const List& obj)
{
	List::Entry* curr = obj.head;
	while (curr) {
		os << curr->key << ": " << curr->value << endl;
		curr = curr->next;
	}
	return os;
}

ostream& operator<<(ostream& os, const List::Entry& obj)
{
	if (&obj)
		os << obj.key << ": " << obj.value << endl;
	else
		os << "Not found.\n";
	return os;
}

class HashTableChains
{
	List* HashTable = nullptr;
	float loadFactor = 0;
	int size = 0;
	int quantity = 0;

	void expand()
	{
		float goldenSection = 1.618;
		List* NewTable = new List[size * goldenSection];
		for (int i = 0; i < size; ++i)
			NewTable[i] = HashTable[i];
		delete[] HashTable;
		size *= goldenSection;
		HashTable = new List[size];
		for (int i = 0; i < size; ++i)
			HashTable[i] = NewTable[i];
		delete[] NewTable;
		loadFactor = (float)quantity / (float)size;
	}

	void copyTable(const HashTableChains& obj)
	{
		for (int i = 0; i < size; ++i)
			HashTable[i] = obj.HashTable[i];
	}

	void deleteTable()
	{
		if (HashTable)
			delete[] HashTable;
	}

	int hash(char* key) const
	{
		int length = strlen(key);
		int hash = 0;
		for (int i = 0; i < size; ++i) {
			hash += (int)key[i] * i;
		}
		hash %= size;
		return hash;
	}

public:
	friend ostream& operator<<(ostream& os, const HashTableChains& obj);

	HashTableChains() = default;

	HashTableChains(int size_) :
		HashTable{ new List[size_] }, size{ size_ }
	{
	}

	HashTableChains(const HashTableChains& obj)
		:size{ obj.size }, quantity{ obj.quantity }, HashTable{ new List[obj.size] }
	{
		copyTable(obj);
	}

	HashTableChains& operator=(const HashTableChains& obj)
	{
		if (this != &obj) {
			deleteTable();
			size = obj.size;
			quantity = obj.quantity;
			HashTable = new List[size];
			copyTable(obj);
		}
		return *this;
	}

	~HashTableChains()
	{
		deleteTable();
	}

	void addEntry(char* key, char* value)
	{
		int index = hash(key);
		HashTable[index].pushBack(key, value);
		++quantity;
		loadFactor = (float)quantity / (float)size;
		if (loadFactor >= 1)
			expand();
	}  

	List::Entry* searchKey(char* key) const
	{
		List::Entry* result;
		for (int i = 0; i < size; ++i) {
			if (&HashTable[i]) {
				result = HashTable[i].search(key);
				if (result)
					return result;
			}
		}
		return nullptr;
	}

	bool deleteKey(char* key)
	{
		for (int i = 0; i < size; ++i)
			if (&HashTable[i] && HashTable[i].deleteEntry(key))
				return true;
		return false;
	}  

	char* getKey(int index, int number) const
	{
		if (index < size && index >= 0) {
			List::Entry* curr = HashTable[index].getHead();
			while (number-- > 0 && curr->next)
				curr = curr->next;
			return curr->key;
		}
		return nullptr;
	}

	inline int getSize() const
	{
		return size;
	}
};


ostream& operator<<(ostream& os, const HashTableChains& obj)
{
	for (int i = 0; i < obj.size; ++i) {
		if (obj.HashTable[i].getHead())
			os << i << ":  " << obj.HashTable[i] << endl;
	}
	return os;
}

class HashTableOpenedAddressing
{
	struct Entry {
		char* key = nullptr;
		char* value = nullptr;
		bool deleted = 0;

		Entry() = default;

		Entry(const char* key_, const char* value_)
		{
			int size = strlen(key_) + 1;
			key = new char[size];
			strcpy_s(key, size, key_);
			size = strlen(value_) + 1;
			value = new char[size];
			strcpy_s(value, size, value_);
		}

		~Entry() = default;

		Entry& operator=(const Entry& obj)
		{
			int length = strlen(obj.key) + 1;
			key = new char[length];
			strcpy_s(key, length, obj.key);
			length = strlen(obj.value) + 1;
			value = new char[length];
			strcpy_s(value, length, obj.value);
			return *this;
		}

		void deleteEntry()
		{
			delete[] key;
			key = nullptr;
			delete[] value;
			value = nullptr;
			deleted = true;
		} 
	};

	Entry* HashTable = nullptr;
	int size = 0;

	void copyTable(const HashTableOpenedAddressing& obj)
	{
		for (int i = 0; i < size; ++i) {
			if (obj.HashTable[i].key) {
				int length = strlen(obj.HashTable[i].key) + 1;
				HashTable[i].key = new char[length];
				strcpy_s(HashTable[i].key, length, obj.HashTable[i].key);
				length = strlen(obj.HashTable[i].value) + 1;
				HashTable[i].value = new char[length];
				strcpy_s(HashTable[i].value, length, obj.HashTable[i].value);
			}
		}
	}

	void deleteTable()
	{
		if (HashTable)
			for (int i = 0; i < size; ++i)
				if (HashTable[i].key) {
					delete[] HashTable[i].key;
					HashTable[i].key = nullptr;
					delete[] HashTable[i].value;
					HashTable[i].value = nullptr;
				}
		HashTable = nullptr;
	}  

	int hash(char* key) const
	{
		int length = strlen(key);
		int hash = 0;
		for (int i = 0; i < length; ++i) {
			hash += (int)key[i] * i;
		}
		hash %= size;
		return hash;
	}

	void expand()
	{
		float goldenSection = 1.618;
		Entry* NewTable = new Entry[size * goldenSection];
		for (int i = 0; i < size; ++i) {
			NewTable[i].key = HashTable[i].key;
			NewTable[i].value = HashTable[i].value;
		}
		delete[] HashTable;
		size *= goldenSection;
		HashTable = new Entry[size];
		for (int i = 0; i < size; ++i) {
			HashTable[i].key = NewTable[i].key;
			HashTable[i].value = NewTable[i].value;
		}
		delete[] NewTable;
	}

public:
	friend ostream& operator<<(ostream& os, HashTableOpenedAddressing& obj);
	friend ostream& operator<<(ostream& os, Entry& obj);

	HashTableOpenedAddressing() :
		size{ 2 }, HashTable{ new Entry[2] }
	{
	}

	HashTableOpenedAddressing(char* key_, char* value_) :
		size{ 2 }, HashTable{ new Entry[2] }
	{
	}

	~HashTableOpenedAddressing()
	{
		deleteTable();
	}

	HashTableOpenedAddressing(const HashTableOpenedAddressing& obj)
		:size{ obj.size }, HashTable{ new Entry[obj.size] }
	{
		copyTable(obj);
	}

	HashTableOpenedAddressing& operator=(const HashTableOpenedAddressing& obj)
	{
		if (this != &obj)
			deleteTable();
		size = obj.size;
		copyTable(obj);
		return *this;
	}

	void addEntry(char* key, char* value)
	{
		int index = hash(key);
		if (HashTable[index].key) {
			while (index < size && HashTable[index].key) {
				++index;
			}
			if (index == size)
				expand();
		}
		HashTable[index] = Entry(key, value);
		HashTable[index].deleted = 0;
	}


	inline int getSize() const
	{
		return size;
	}

	Entry* searchKey(const char* key) const
	{  // can't use start index = hash(key) for search, 'cause size may be changed, 
	   //so new hash may not coinside with original one
		for (int i = 0; i < size; ++i)
			if (!HashTable[i].deleted && !strcmp(HashTable[i].key, key))
				return &HashTable[i];
		return nullptr;
	}

	bool deleteKey(const char* key)
	{
		Entry* Found = searchKey(key);
		if (Found) {
			Found->deleteEntry();
			Found->deleted = true;
			return true;
		}
		return false;
	}
};

ostream& operator<<(ostream& os, HashTableOpenedAddressing& obj)
{
	for (int i = 0; i < obj.size; ++i)
		if (obj.HashTable[i].key)
			os << i << ". " << obj.HashTable[i].key << ": " << obj.HashTable[i].value << endl;
	return os;
}

ostream& operator<<(ostream& os, HashTableOpenedAddressing::Entry& obj)
{
	if (&obj)
		os << obj.key << ": " << obj.value << endl;
	else
		os << "Not found.\n";
	return os;
}

int main()
{
	cout << "Hash table with chain solution of collisions.\n\nCreate hash table HT1 for 4 elements.\n";
	HashTableChains HT1(4);
	cout << "Current size is: " << HT1.getSize() << endl;
	cout << "Add 5 entries to hash table with chains HT1.\n";
	HT1.addEntry("Ivan Ivanov", "+375-29-111-11-11");
	HT1.addEntry("John Doe", "+1-212-123-45-67");
	HT1.addEntry("Pyotr Sidorov", "+375-33-222-22-22");
	HT1.addEntry("Mary Doe", "+1-212-123-45-68");
	HT1.addEntry("Grigory Petrov", "+375-44-333-33-33");
	cout << "HT1:\n" << HT1;
	cout << "Current size is: " << HT1.getSize() << ": hash table was expanded because it's load factor got > 1." << endl;
	HashTableChains HT2 = HT1;
	cout << "Create new hash table HT2 = HT1:\n" << HT2 << endl;
	cout << "Add new entry \"Ronny Smith\" to hash table with chains HT1.\n";
	HT2.addEntry("Ronny Smith", "+1-212-987-65-43");
	cout << "Now HT2 is: \n" << HT2;
	HT1 = HT2;
	cout << "Equate HT1 to HT2 \nHT1:\n" << HT1 << endl;
	cout << "Search for entry \"Ronny Smith\" in HT2: \n" << *(HT2.searchKey("Ronny Smith")) << endl;
	cout << "Search for entry \"Ronny Zmith\" - it's a mistake - in HT2: \n" << *(HT2.searchKey("Ronny Zmith")) << endl;
	cout << "Delete key \"MaryDoe\" - it's a mistake - in HT2: " << HT2.deleteKey("MaryDoe") << endl;
	cout << "Delete key \"Mary Doe\" in HT2: " << HT2.deleteKey("Mary Doe") << endl;
	cout << "Now HT2 is: \n" << HT2;
	cout << "HT1:\n" << HT1;
	cout << "Press any key to proceed\n";
	cin.get();
	system("cls");

	cout << "Hash table with opened addressing. \n\nCreate hash table with opened addressing HT3 for 2 elements as default.\n";
	HashTableOpenedAddressing HT3;
	cout << "Size of HT3 is " << HT3.getSize() << endl << endl;
	cout << "Add the first entry:\n";
	HT3.addEntry("Semyon Semyonov", "+375-29-111-222-33");
	cout << "Now HT3 is: \n" << HT3 << endl;
	cout << "Size of HT3 is " << HT3.getSize() << endl << endl;
	cout << "Add two new entries that are the same: \n";
	HT3.addEntry("Semyon Semyonov", "+375-29-111-222-33");
	HT3.addEntry("Semyon Semyonov", "+375-29-111-222-33");
	cout << "Now HT3 is: \n" << HT3 << endl;
	cout << "Size of HT3 is " << HT3.getSize() << endl << endl;
	cout << "Add next 5 entries.\n";
	HT3.addEntry("John Doe", "+1-212-123-45-67");
	HT3.addEntry("Pyotr Sidorov", "+375-33-222-22-22");
	HT3.addEntry("Ivan Ivanov", "+375-29-111-11-11");
	HT3.addEntry("Mary Doe", "+1-212-123-45-68");
	HT3.addEntry("Grigory Petrov", "+375-44-333-33-33");
	cout << "Now HT3 is: \n" << HT3 << "and it's size is " << HT3.getSize() << endl << endl;
	HashTableOpenedAddressing HT4 = HT3;
	cout << "Create new hash table HT4 = HT3:\n" << HT4;
	cout << "Add a new entry to HT4:\n";
	HT4.addEntry("Ivan Ivanov", "+375-44-999-99-99");
	cout << "Now HT4 is: \n" << HT4 << "and it's size is " << HT4.getSize() << endl << endl;
	cout << "Now HT3 is: \n" << HT3 << "and it's size is " << HT3.getSize() << endl << endl;
	cout << "Search for key \"Grigory Petrov\" in HT4:\n" << *(HT4.searchKey("Grigory Petrov")) << endl;
	cout << "Search for key \"Grigoriy Petrov\" - it's a mistake - in HT4:\n" << *(HT4.searchKey("Grigoriy Petrov")) << endl;
	cout << "Delete key \"Semyon Semyonov\" in HT4: " << HT4.deleteKey("Semyon Semyonov") << endl;
	cout << "Delete key \"Semyon Semyonov\" in HT4: " << HT4.deleteKey("Semyon Semyonov") << endl;
	cout << "Delete key \"Grigoriy Petrov\" - it's a mistake - in HT4: " << HT4.deleteKey("Grigoriy Petrov") << endl;
	cout << "Now HT4 is: \n" << HT4 << "and it's size is " << HT4.getSize() << endl << endl;
	cout << "Add next 2 entries.\n";
	HT4.addEntry("Olga Ivanova", "+375-33-888-77-66");
	HT4.addEntry("Huanito Hulio", "+1-212-555-00-22");
	cout << "Now HT4 is: \n" << HT4 << "and it's size is " << HT4.getSize() << endl << endl;
	return 0;
}