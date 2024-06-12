#include "pch.h"
#include "ConsoleApplication1.h"

using namespace std;

/*
void display(int arr[], int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

void insertionSort(int arr[], int size)
{
	int j, temp;
	for (int i = 1; i < size; i++)
	{
		j = i;
		while (j > 0 && arr[j - 1] > arr[j])
		{
			temp = arr[j];
			arr[j] = arr[j - 1];
			arr[j - 1] = temp;
			j--;
		}
	}
}

void selectionSort(int arr[], int size)
{
	int first, temp;

	for (int i = size - 1; i > 0; i--)
	{
		first = 0;
		for (int j = 1; j <= i; j++)
		{
			if (arr[j] > arr[first])
			{ 
				first = j;
			}
			temp = arr[first];
			arr[first] = arr[i];
			arr[i] = temp;
		}
	}
}

void quickSort(int arr[], int left, int right)
{
	int i = left;
	int j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];
	while (i <= j)
	{
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j)
		{
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	}
	if (left < j)
		quickSort(arr, left, j);
	if (right > i)
		quickSort(arr, i, right);
}

double getTime(clock_t time1, clock_t time2)
{
	double ticks = time1 - time2;
	return (ticks * 10) / CLOCKS_PER_SEC;
}

void genData(int arr[], int size)
{
	const unsigned seed = static_cast<unsigned>(time(NULL));
	srand(seed);
	for (int i = 0; i < size; i++)
	{
		arr[i] = (rand() % 100) + 1;
	}
}

namespace firstNums
{
	int num1 = 10;
	int num2 = 20;
}

namespace secondNums
{
	int num1 = 1;
	int num2 = 2;
}

namespace minMax
{
	int min(int num1, int num2)
	{
		if (num1 < num2)
			return num1;
		else
			return num2;
	}
	int max(int num1, int num2)
	{
		if (num1 > num2)
			return num1;
		else
			return num2;
	}
}

namespace People
{
	class Person
	{
	private:
		string name;
		string sex;
	public:
		Person(string n, string s)
		{
			name = n;
			sex = s;
		}
		string get()
		{
			return name + ", " + sex;
		}
	};
}
*/

/* Constants, auto, and decltype
auto AutoFunctionFromReturn(int parameter) -> int
{
	return parameter;
}

template <typename T>
auto AutoFunctionFromParameter(T parameter) -> decltype(parameter) // decltype specifically used with auto to assist type inference
{
	return parameter;
}

// I note the similarity of using : to access class members during construction

class MyClass
{
private:
	size_t m_Member;
public:
	constexpr MyClass(int parameter)
		: m_Member{ static_cast<size_t>(parameter) }
	{
		// Default constructor,  m_Member is initialized from parameter
	}
	constexpr size_t GetValue() const
	{
		return m_Member;
	}
};

constexpr size_t ArraySizeFunction(int parameter)
{
	return parameter;
}
*/

/*
class MyClass
{
private:
	vector<string> m_String{
		"This is a pretty long string that"
		" must be copy constructed into"
		" copyConstructed!"s
	};

	int m_Value{ 1 };

public:
	MyClass() = default;
	MyClass(const MyClass& rhs) = default;
	MyClass(MyClass&& rhs) = default;

	int GetValue() const
	{
		return m_Value;
	}
};
*/

/*
using MyUniquePointer = unique_ptr<MyClass>;
auto PassUniquePointer(MyUniquePointer ptr)
{
cout << "In function name: " << ptr->GetName() << endl;
return ptr;
}
*/

/*
class MyClass
{
public:
	MyClass() = default;
	MyClass(const char* name, uint32_t value)
		: m_Value{ value }
	{
		const uint32_t length = strlen(name) + 1; // Add space for null terminator
		m_Name = new char[length] {};
		strcpy(m_Name, name);
		//TODO: 
	}
	~MyClass()
	{
		if (m_Name != nullptr)
		{
			delete m_Name;
			m_Name = nullptr;
		}
	}
	//~MyClass() = default;
	// MyClass(const MyClass& other) = delete; // Tell the compiler you do not want copy available
	// MyClass& operator=(const MyClass& other) = delete; // Tell the compiler you do not want assign available

	// Copy lvalue constructor
	MyClass(const MyClass& other)
	{
		const uint32_t length = strlen(other.m_Name) + 1;
		m_Name = new char[length] {};
		strcpy(m_Name, other.m_Name);
		m_Value = other.m_Value;
	}
	// Copy lvalue assign operator
	MyClass& operator=(const MyClass& other)
	{
		if (m_Name != nullptr)
		{
			delete m_Name;
		}

		const uint32_t length = strlen(other.m_Name) + 1;
		m_Name = new char[length] {};
		strcpy(m_Name, other.m_Name);
		m_Value = other.m_Value;

		return *this;
	}
	// Move rvalue constructor
	MyClass(MyClass&& other)
	{
		m_Name = other.m_Name;
		other.m_Name = nullptr;
		m_Value = other.m_Value;
	}
	// Move rvalue assign operator
	MyClass& operator=(MyClass&& other)
	{
		if (m_Name != nullptr)
		{
			delete m_Name;
		}

		m_Name = other.m_Name;
		other.m_Name = nullptr;
		m_Value = other.m_Value;

		return *this;
	}

	// Note the use of char* as a string to illustrate moving objects
	char* GetName() { return m_Name; }
	uint32_t GetValue() const { return m_Value; } // Versus moving stack values

	// m_Name can only be changed through construction
	void SetValue(uint32_t value) { m_Value = value; }

private:
	char*				m_Name{};
	uint32_t			m_Value{};
};
*/

/*
class Printable
{
public:
	virtual void Print() = 0;
};

class Vehicle
{
public:
	Vehicle() : m_NumberOfWheels{ 0 }
	{

	}
	Vehicle(uint32_t numberOfWheels)
		: m_NumberOfWheels{ numberOfWheels }
	{

	}
	virtual void Print() = 0;
	//void  Print() { cout << "Vehicle Wheels: " << GetNumberOfWheels() << endl; }
	//virtual uint32_t GetNumberOfWheels() const = 0; // Pure virtual declaration
	virtual uint32_t GetNumberOfWheels() const
	{
		return m_NumberOfWheels;
	}

protected:
	uint32_t m_NumberOfWheels{};
};

class Bike : public Vehicle, public Printable
{
public:
	Bike() : Vehicle{ 2 }
	{

	}
	uint32_t GetNumberOfWheels() const override { return 2; }
	void Print() override { cout << "Bike Wheels: " << GetNumberOfWheels() << endl; }
};

class Car : public Vehicle, public Printable
{
public:
	Car() : Vehicle{ 4 }
	{
		//NOTE: In this sample, we initialize the protected value to 4, but also return 4 literally only for exemplary purposes demonstrating the override mechanisms
	}
	uint32_t GetNumberOfWheels() const final { return 4; }
	void Print() override { cout << "Car Wheels: " << GetNumberOfWheels() << endl; }
};

class Ford : public Car
{
public:
	Ford() : Car{}
	{

	}
	//uint32_t GetNumberOfWheels() const override { return 4; } // Cannot override final method in Car
	void Print() override { cout << "Ford Wheels: " << GetNumberOfWheels() << endl; }
};
*/

/*
//using namespace Test;
Test::MyTest::MyTest(unsigned number)
	: pImpl{ make_unique<Impl>(number) }
{
}

class Test::MyTest::Impl
{
private:
	unsigned m_number;
public:
	Impl(unsigned number);

	unsigned GetNumber();
};

Test::MyTest::Impl::Impl(unsigned number)
	: m_number{ number }
{
}

unsigned Test::MyTest::Impl::GetNumber()
{
	return m_number;
}
*/




int main()
{
	



	/*
	auto test = make_shared<MyTest>(100);
	
	auto impl = test->GetImplPtr();

	auto num = impl->GetNumber();

	cout << num << endl;
	*/

	/*
	// Use a pointer to handle virtual, override, final. polymorphism and multiple inheritance
	Vehicle* pointer{};

	//Vehicle myVehicle{};
	//pointer = &myVehicle;
	//pointer->Print(); // This cannot be called because it is pure virtual

	Bike myBike{};
	pointer = &myBike;
	pointer->Print();

	Car myCar{};
	pointer = &myCar;
	pointer->Print();

	Ford myFord{};
	pointer = &myFord;
	pointer->Print();

	vector<int> myInts;
	cout << myInts.size() << endl;
	cout << myInts.capacity() << endl;
	myInts.reserve(16);
	cout << myInts.capacity() << endl;
	myInts.shrink_to_fit();
	cout << myInts.capacity() << endl;
	*/

	/*
	vector<uint32_t> myVector;
	for (char i = 0; i < 100; ++i)
	{
		myVector.push_back(i);
	}
	// Using automatically generated begin to end iterator (foreach)
	for (auto&& number : myVector)
	{
		cout << number << " ";
	}
	cout << endl;
	*/

	/*
	// Demonstrating move constructor exercise, note the line that would cause a crash after move() function is called
	MyClass myAssignedCar;
	{
		MyClass myCar{ "myCar", 4 };
		cout << "Vehicle name: " << myCar.GetName() << endl;

		myAssignedCar = move(myCar);
		// cout << "Vehicle name: " << myCar.GetName() << endl; // This line would result in a crash
		cout << "Vehicle name: " << myAssignedCar.GetName() << endl;
	}
	cout << "Vehicle name: " << myAssignedCar.GetName() << endl;
	*/

	/* Instantiating a class and instantiating 
	MyClass myClass1{};
	myClass1.SetValue(1);
	auto myValu1e = myClass1.GetValue();

	auto myClass2{ make_unique<MyClass>() };
	myClass2.SetValue(2);
	myValue2 = myClass2.GetValue();
	*/

	/* Simple example of instantiating with a managed pointer
	// make_unique is the call that creates the pointer, handles new and delete
	unique_ptr<MyClass> uniquePointer{ make_unique<MyClass>("MyClass", 10) };
	cout << "Name: " << uniquePointer->GetName() << endl;
	cout << "Value: " << uniquePointer->GetValue() << endl;
	*/

	/*
	auto uniquePointer = make_unique<MyClass>("MyClass", 10);
	auto newUniquePointer = PassUniquePointer(move(uniquePointer));

	// This if check will fail, demonstrating that the original reference is no longer valid
	if (uniquePointer)
	{
		// We never see this text in a run of the application (expected behavior)
		cout << "First object name: " << uniquePointer->GetName() << endl;
	}

	cout << "Second object name: " << newUniquePointer->GetName() << endl;

	auto moveUniquePointer = move(newUniquePointer);
	cout << "Third object name: " << moveUniquePointer->GetName() << endl;
	*/

	/* Fancy copy/move construction pattern
	using MyVector = vector<MyClass>;
	constexpr unsigned ITERATIONS{ 100000U };

	MyVector copyConstructed(ITERATIONS);
	int value{ 0 };

	auto copyStartTime = high_resolution_clock::now();
	for (unsigned i = 0; i < ITERATIONS; ++i)
	{
		MyClass myClass;
		copyConstructed.push_back(myClass);
		value = myClass.GetValue();
	}
	auto copyEndTime = high_resolution_clock::now();
	cout << "Value: " << value << endl;
	auto copyDuration = duration_cast<milliseconds>(copyEndTime - copyStartTime);
	cout << "Copy time: " << copyDuration.count() << "ms" << endl;

	MyVector moveConstructed(ITERATIONS);

	auto moveStartTime = high_resolution_clock::now();
	for (unsigned i = 0; i < ITERATIONS; ++i)
	{
		MyClass myClass;
		moveConstructed.push_back(move(myClass));
		value = myClass.GetValue();
	}
	auto moveEndTime = high_resolution_clock::now();
	cout << "Value: " << value << endl;
	auto moveDuration = duration_cast<milliseconds>(moveEndTime - moveStartTime);
	cout << "Move time: " << moveDuration.count() << "ms" << endl;
	*/

	/* Simple time and thread pause sample
	auto startTime = system_clock::now();
	this_thread::sleep_for(5s);
	auto endTime = system_clock::now();
	
	auto timeTaken = duration_cast<milliseconds>(endTime - startTime);
	cout << timeTaken.count() << endl;
	*/

	/* Universal initialization with type narrowing through static_cast
	int number{ 0 };
	char another{ static_cast<char>(512) };
	double bigNumber{ 1.0 };
	float littleNumber{ static_cast<float>(bigNumber) };
	// Above, we've demonstrated intentionally "narrowing" the variable's size using static_cast<type>
	// I personally note that Uniform Initialization makes sense now as I've seen it in many places, just didn't understand it was a "thing"
	// A formal type for uniform initialization
	//initializer_list<int> initList{ 2, 10 };
	//MyVector vectorC(initList);
	*/

	/* Vector initialization
	using MyVector = vector<int>;
	MyVector vectorA(2); // Size
	cout << "Size: " << vectorA.size() << endl;
	cout << "[0]: " << vectorA[0] << endl;
	cout << "[1]: " << vectorA[1] << endl;
	MyVector vectorB(2, 10); // Size, Initial Value
	cout << "Size: " << vectorB.size() << endl;
	cout << "[0]: " << vectorB[0] << endl;
	cout << "[1]: " << vectorB[1] << endl;
	*/

	/* Auto type deduction
	// Auto type deduction with typeid showing internal type symbol
	auto variableA = 1; // int
	cout << "Type of variable: " << typeid(variableA).name() << endl;

	auto variableB = 1.0f; // float
	cout << "Type of variable: " << typeid(variableB).name() << endl;

	auto variableC = static_cast<long long>(1); // __int64
	cout << "Type of variable: " << typeid(variableC).name() << endl;

	// Mixing auto with uniform initialization produces unexpected results
	auto variableD{ 1 }; // int
	cout << "Type of variable: " << typeid(variableD).name() << endl;
	*/

	/* Constants and auto typing
	auto valueA = AutoFunctionFromReturn(1);
	cout << valueA << " " << typeid(valueA).name() << endl;
	auto valueB = AutoFunctionFromParameter(2.0f);
	cout << valueB << " " << typeid(valueB).name() << endl;

	constexpr size_t ARRAY_SIZE_A{ 5 };
	array<uint32_t, ARRAY_SIZE_A> myArrayA{ 1, 2, 3, 4, 5 };

	constexpr size_t ARRAY_SIZE_B{ ArraySizeFunction(5) }; // Not sure what the improvment of "constexpr" versus "const" is?
	array<uint32_t, ARRAY_SIZE_B> myArrayB{ 6, 7, 8, 9, 10 };

	constexpr size_t ARRAY_SIZE_C{ MyClass{ 5 }.GetValue() };
	array<uint32_t, ARRAY_SIZE_C> myArrayC{ 11, 12, 13, 14, 15 };
	
	// foreach
	for (auto&& number : myArrayA)
	{
		cout << number << endl;
	}
	*/

	/*
	cout << "num1 in firstNums: " << firstNums::num1 << endl;
	cout << "num1 in secondNums: " << secondNums::num1 << endl;
	cout << "num2 in firstNums: " << firstNums::num2 << endl;
	cout << "num2 in secondNums: " << secondNums::num2 << endl;
	{
		using namespace firstNums;
		cout << "num1 in firstNums: " << num1 << endl;
		cout << "num2 in firstNums: " << num2 << endl;
	}
	{
		using namespace secondNums;
		cout << "num1 in secondNums: " << num1 << endl;
		cout << "num2 in secondNums: " << num2 << endl;
	}
	using namespace minMax;
	using namespace People;

	int a, b;
	cout << "Enter a number: ";
	cin >> a;
	cout << "Enter another number: ";
	cin >> b;
	cout << min(a, b) << endl;
	cout << max(a, b) << endl;
	cout << endl;
	Person you("Jane Doe", "F");
	cout << you.get() << endl;
	*/

	/*
	const int size = 10000;
	int numbers[size];
	genData(numbers, size);
	clock_t begin = clock();
	insertionSort(numbers, size);
	clock_t end = clock();
	cout << "Insertion Sort: " << getTime(end, begin) << "ms" << endl;

	genData(numbers, size);
	begin = clock();
	selectionSort(numbers, size);
	end = clock();
	cout << "Selection Sort: " << getTime(end, begin) << "ms" << endl;

	genData(numbers, size);
	begin = clock();
	quickSort(numbers, 0, size);
	end = clock();
	cout << "Quick Sort: " << getTime(end, begin) << "ms" << endl;
	*/
	
	return 0;
}
