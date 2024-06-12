#include <iostream>
#include <algorithm>
#include <vector>

void PrintFunction(int x)
{
	std::cout << x << std::endl;
}

struct PrintFunctor
{
	void operator()(int x) const
	{
		std::cout << x << std::endl;
	}
};

// By default a lambda is const like in the Functor, override with mutable keyword
auto mutableLambda = [](int x) mutable { std::cout << x << std::endl; };


int main()
{
	int x = 1;
	int y = 2;

	std::cout << x << " " << y << std::endl;

	auto myLambda1 = [&x, &y]() { ++x; ++y; };
	myLambda1();
	
	std::cout << x << " " << y << std::endl;

	auto myLambda2 = [x, y]() mutable { std::cout << ++x << " " << ++y << std::endl; };
	myLambda2();

	std::cout << x << " " << y << std::endl;

	[&]() { ++x; ++y; }();

	std::cout << x << " " << y << std::endl;



	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);

	for (int element : v)
	{
		PrintFunction(element);
	}

	std::for_each(v.begin(), v.end(), PrintFunctor());

	struct {
		void operator()(int x) const
		{
			std::cout << x << std::endl;
		}
	} localPrintFunctor;

	std::for_each(v.begin(), v.end(), localPrintFunctor);

	std::for_each(v.begin(), v.end(), [](int x) { std::cout << x << std::endl; });
}
