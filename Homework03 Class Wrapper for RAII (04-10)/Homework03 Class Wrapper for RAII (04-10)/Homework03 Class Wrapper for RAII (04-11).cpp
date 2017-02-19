#include <iostream>

using namespace std;

template <typename T>
class UniqueOwner {

	T* resource = nullptr;

public:

	UniqueOwner() = default;

	UniqueOwner(T* resource_) :
		resource{ resource_ }
	{
	}

	UniqueOwner(UniqueOwner& obj)
	{
		resource = obj.resource;
		obj.resource = nullptr;
	}

	UniqueOwner& operator=(UniqueOwner& obj)
	{
		if (&obj != this) {
			resource = obj.resource;
			obj.resource = nullptr;
		}
		return *this;
	}

	~UniqueOwner() {
		if (resource)
			resource = nullptr;
	}

	static int menuUniqueOwner()
	{
		int answer = 0;
		int items = 5;
		while (answer <= 0 || answer > items) {
			cout << "1. Show the resourse of the main owner.\n"
				"2. Create a new copy of the main onwer.\n"
				"3. Create a new owner, than copy the main owner to it.\n"
				"4. Re-enter the value of the resourse.\n"
				"5. Exit.\n\n";
			answer = getAnswerInt(items);
		}
		return answer;
	}
	void showresource() const
	{
		if (resource)
			cout << *resource << endl;
		else
			cout << "no resource." << endl;
	}
};

template <typename T>
struct SharedResource {
	int count = 0;
	T value = NULL;

	SharedResource() = default;

	SharedResource(T value_) :
		value{ value_ }
	{
	}
};


template <typename T>
class SharedOwner
{
	SharedResource<T>* resource = nullptr;

public:

	SharedOwner() = default;

	SharedOwner(SharedResource<T>* resource_) :
		resource{ resource_ }
	{
		++(resource->count);
	}

	SharedOwner(const SharedOwner& obj)
	{
		resource = obj.resource;
		++(resource->count);
	}

	SharedOwner& operator=(const SharedOwner& obj)
	{
		if (&obj != this) {
			resource = obj.resource;
			++resource->count;
		}
		return *this;
	}

	~SharedOwner()
	{
		if (resource) {
			--resource->count;
			resource = nullptr;
		}
	}

	T getResource()
	{
		if (resource)
			return resource->value;
		return T();
	}

	void setPosession(SharedResource<T>* resource_)
	{
		resource = resource_;
		++resource->count;
	}

	void takeOffPosession()
	{
		--resource->count;
		resource = nullptr;

	}

	static int menuSharedOwners()
	{
		int answer = 0;
		int items = 6;
		while (answer <= 0 || answer > items) {
			cout << "1. Show the value of the resource.\n"
				"2. Re-enter the value of the resource.\n"
				"3. Show how much owners the resource has.\n"
				"4. Pass the posession of the resource to a new owner.\n"
				"5. Take off the posession of the resource from the owner.\n"
				"6. Exit.\n\n";
			answer = getAnswerInt(items);
		}
		return answer;
	}
};


int getAnswerInt(int board = INT_MAX) {
	int answerInt = 0;
	const int lengthForInt = 9;
	char answerString[lengthForInt + 1];
	while (answerInt <= 0 || answerInt > board) {
		cin.getline(answerString, lengthForInt);
		answerInt = atoi(answerString);
		if (answerInt <= 0 || answerInt > board)
			cout << "Error! Enter again: ";
	}
	return answerInt;
}

int main()
{
	cout << "\t\t RAII-wrappers: unique owners.\n\n";
	double resource = 0.0;
	cout << "Enter the value of the resource (a number, type \"double\"): ";
	cin >> resource;
	UniqueOwner<double> MainOwner(&resource);
	while (true) {
		cin.ignore();
		int answer = UniqueOwner<double>::menuUniqueOwner();
		if (answer == 5)
			break;
		switch (answer) {
		case 1: {
			cout << "Resourse of the main owner: ";
			MainOwner.showresource();
			break;
		}
		case 2: {
			UniqueOwner<double> NextOwner = MainOwner;
			cout << "Resourse of the main owner: ";
			MainOwner.showresource();
			cout << "Resourse of the next owner #1: ";
			NextOwner.showresource();
			MainOwner = NextOwner;
			break;
		}
		case 3: {
			UniqueOwner<double> NextOwner;
			NextOwner = MainOwner;
			cout << "Resourse of the main owner: ";
			MainOwner.showresource();
			cout << "Resourse of the next owner #2: ";
			NextOwner.showresource();
			MainOwner = NextOwner;
			break;
		}
		case 4: {
			cout << "Re-enter the value of the resource (a number, type \"double\"): ";
			cin >> resource;
			break;
		}
		}
	}

	cout << "\t\t RAII-wrappers: shared owners.\n\n";
	SharedResource<double> SharedResource;
	cout << "Enter the value of the shared resource(a number, type double): ";
	cin >> SharedResource.value;
	SharedOwner<double> SharedOwner1(&SharedResource);
	SharedOwner<double> SharedOwner2;
	while (true) {
		cin.ignore();
		int answer = SharedOwner<double>::menuSharedOwners();
		if (answer == 6)
			break;
		switch (answer) {
		case 1: {
			cout << "The value of the shared resource is " << SharedResource.value;
			break;
		}
		case 2: {
			cout << "Re-enter the value of the shared resource(number, type double): ";
			cin >> SharedResource.value;
			break;
		}
		case 3: {
			cout << "The shared resourse has " << SharedResource.count << " owners.\n";
			break;
		}
		case 4: {
			if (SharedOwner1.getResource()) {
				cout << "The first owner has a resource already. \n";
				if (SharedOwner2.getResource())
					cout << "The second owner has a resource already. \n";
				else {
					cout << "The second owner gets the shared resource.";
					SharedOwner2 = SharedOwner1;
					cout << "The resource " << SharedOwner2.getResource() << " got.\n";
				}
			}
			else {
				cout << "The first owner hasn't any resource. \n The first owner gets resource.";
				SharedOwner1.setPosession(&SharedResource);
				cout << "The resource " << SharedOwner1.getResource() << " got.\n";
			}
			break;
		}
		case 5: {
			if (SharedOwner2.getResource()) {
				cout << "Taking off the resource from the second owner.\n";
				SharedOwner2.takeOffPosession();
			}
			else if (SharedOwner1.getResource()) {
				cout << "Taking off the resource from the first owner.\n";
				SharedOwner1.takeOffPosession();
			}
			else
				cout << "The both owners don't posess any resource.\n";
		}
		}
	}
	return 0;
}
