#include <iostream>

using namespace std;

#define SET_TO_UPPER_CASE -32
#define SET_TO_LOWER_CASE 32

class String
{
private:
	char* text_;
	int size = 0;

public:
	String(const char* text);   //1	- constructor
	void change(const char* text);	//2	
	inline char* getString(); //3			
	inline int lengthOfString(); //4		
	void add(const char* text); //5		
	void pasteSymbol(char symbol, int index);  //6
	void ASCII_codes(); //7		
	void toUppercase();//8
	void toLowercase();//9
	~String();
};

String::String(const char* text)   //1 - constructor
{
	size = strlen(text) + 1; // +1 is for /0
	text_ = new char[size];
	strcpy_s(text_, size, text);
}

void String::change(const char* text)	//2
{
	delete[] text_;
	size = strlen(text) + 1; // +1 is for /0
	text_ = new char[size];
	strcpy_s(text_, size, text);
}

char* String::getString() //3
{
	return text_;
}

int String::lengthOfString() //4
{
	return strlen(text_);
}

void String::add(const char* text) //5
{
	size = strlen(text) + size;
	char* textBuffer = text_;
	text_ = new char[size];
	strcpy_s(text_, size, textBuffer);
	delete[] textBuffer;
	strcat_s(text_, size, text);
}

void String::pasteSymbol(char symbol, int index)  //6
{
	if (index < 0 || index >= size) {
		cout << "Error! The " << index << "th place isn't exist.\n";
		return;
	}
	++size;
	char* textBuffer = text_;
	text_ = new char[size];
	strncpy_s(text_, index + 1, textBuffer, _TRUNCATE);	 //+1 is for textBuffer[0]
	text_[index] = symbol;
	for (int i = index + 1; i < size; ++i) {
		text_[i] = textBuffer[i - 1];
	}
}

void String::ASCII_codes() //7
{
	int i = 0;
	while (i < size - 1) //text_[size - 1] is \0 
		cout << (int)text_[i++] << " ";
	cout << endl;
}

void String::toUppercase()//8
{
	int i = 0;
	while (text_[i] != '\0') {
		if ((int)text_[i] >= 'a' && (int)text_[i] <= 'z')
			text_[i] += SET_TO_UPPER_CASE;
		++i;
	}
}

void String::toLowercase()//9
{
	int i = 0;
	while (text_[i] != '\0') {
		if ((int)text_[i] >= 'A' && (int)text_[i] <= 'Z')
			text_[i] += SET_TO_LOWER_CASE;
		++i;
	}
}
String::~String()
{
	delete[] text_;
}

int main()
{
	String string("A string.");	 //1
	cout << string.getString() << endl; //3
	string.change("Another string.");	 //2
	cout << string.getString() << endl;
	cout << "The length of the string is " << string.lengthOfString() << ".\n";	 //4
	string.add(" And one string more."); //5
	cout << string.getString() << endl;
	string.pasteSymbol('5', 5); //6
	cout << string.getString() << endl;
	cout << "ASCII codes of symbols of the string: ";
	string.ASCII_codes(); //7
	string.toUppercase();//8
	cout << string.getString() << endl;
	string.toLowercase();//9
	cout << string.getString() << endl;

	return 0;
}