#include <iostream>
#include <memory>
#include <vector>
#include <functional>

class Subject
{
	std::vector<std::function<void(int, int)>> m_callbacks;
public:
	Subject() = default;

	void BindCallback(const std::function<void(int, int)>& observer)
	{
		m_callbacks.emplace_back(observer);
	}

	void InvokeCallback(int x, int y)
	{
		for (auto element : m_callbacks)
		{
			std::invoke(element, x, y);
		}
	}
};

class Observer
{
	std::shared_ptr<Subject> m_subject;
public:
	Observer(std::shared_ptr<Subject> subject)
		: m_subject{ subject }
	{
		m_subject->BindCallback([this](int x, int y) { OnEvent(x, y); });
	}

	void OnEvent(int x, int y) { std::wcout << L"OnEvent(" << x << L", " << y << L")" << std::endl; };
};

int wmain(int argc, wchar_t argv[])
{
	auto sub = std::make_shared<Subject>();

	auto doma = std::make_unique<Observer>(sub);
	auto dome = std::make_unique<Observer>(sub);
	auto domi = std::make_unique<Observer>(sub);
	auto domo = std::make_unique<Observer>(sub);
	auto domu = std::make_unique<Observer>(sub);

	sub->InvokeCallback(10, 20);

	return 0;
}
