#include <iostream>
#include <iomanip>
#include <thread>
#include <future>
#include <chrono>
#include <memory>
#include <functional>
#include <map>
#include <vector>
#include <cassert>

class ThreadController
{
	std::thread m_thread;
	std::function<void(int, int)> m_callback;
public:
	ThreadController(std::future<void> future_obj);
	~ThreadController();

	void BindCallback(std::function<void(int, int)> fn);
	void InvokeCallback(int x, int y);
};

struct ThreadFunctor
{
	void operator()(ThreadController* controller, std::future<void> future_obj)
	{
		bool run_loop = true;
		while (run_loop)
		{
			std::wcout << L"InvokeCallback(1, 1)" << std::endl;
			controller->InvokeCallback(1, 1);
			if (future_obj.wait_for(std::chrono::seconds(1)) != std::future_status::timeout)
			{
				run_loop = false;
			}
		}
	}
};

ThreadController::ThreadController(std::future<void> future_obj)
{
	m_thread = std::thread{ ThreadFunctor(), this, std::move(future_obj) };
}

ThreadController::~ThreadController()
{
	if (m_thread.joinable())
		m_thread.join();
}

void ThreadController::BindCallback(std::function<void(int, int)> fn)
{
	m_callback = fn;
}

void ThreadController::InvokeCallback(int x, int y)
{
	std::invoke(m_callback, x, y);
}

void do_work(int x, int y)
{
	std::wcout << L"do_work(1, 1)" << std::endl;
}

int wmain()
{
	std::promise<void> exit_signal;
	auto tc = std::make_unique<ThreadController>(std::move(exit_signal.get_future()));
	tc->BindCallback([](int x, int y) { do_work(x, y); });
	std::this_thread::sleep_for(std::chrono::seconds(10));
	exit_signal.set_value();
	tc.reset();
}
