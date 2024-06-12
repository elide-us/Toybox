#include <iostream>
#include <iomanip>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <random>
#include <chrono>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d11.h>

std::mutex gDisplay;

struct ThreadID
{
	std::thread::id Id;
	ThreadID(std::thread::id id) : Id{ id } {}
};

std::wostream& operator<<(std::wostream& out, const ThreadID thread) { return out << L"[" << std::setfill(L'0') << std::setw(8) << thread.Id << L"] "; }

template<typename Iterator, typename T>
struct AccumulateBlock
{
	void operator()(Iterator first, Iterator last, T& result)
	{
		gDisplay.lock();
		std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Thread accumulating block beginning..." << std::endl;
		gDisplay.unlock();
		QueryPerformanceCounter(&start);
		result = std::accumulate(first, last, result);
		QueryPerformanceCounter(&end);
		time = end.QuadPart - start.QuadPart;
		gDisplay.lock();
		std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Duration (ticks): " << time << std::endl;
		gDisplay.unlock();
	}
private:
	long long time{ 0ll };
	LARGE_INTEGER start{ };
	LARGE_INTEGER end{ };
};

constexpr auto DEFAULT_THREADS{ 4ull };

template<typename Iterator, typename T>
T ParallelAccumulate(Iterator first, Iterator last, T initial)
{
	auto const length = std::distance(first, last);
	if (!length)
		return initial;

	auto const min_per_thread{ 25ull };
	auto const max_threads = (length + min_per_thread - 1) / min_per_thread;

	auto const hardware_threads = std::thread::hardware_concurrency();
	std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Detected " << hardware_threads << L" physical threads,";
	auto const num_threads = std::min(hardware_threads != 0 ? hardware_threads : DEFAULT_THREADS, max_threads);
	std::wcout << L" using " << num_threads - 1 << L" threads..." << std::endl;

	auto const block_size = length / num_threads;

	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1); // Subtract one from num_threads to account for main thread

	Iterator block_start = first;
	for (auto i = 0ull; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread{ AccumulateBlock<Iterator, T>(), block_start, block_end, std::ref(results[i]) };
		block_start = block_end;
	}
	
	for (auto& entry : threads)
	{
		gDisplay.lock();
		std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << entry.get_id() << L"] Thread accumulating block finished..." << std::endl;
		gDisplay.unlock();
		entry.join();
	}

	gDisplay.lock();
	std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Thread accumulating remainder block beginning..." << std::endl;
	gDisplay.unlock();
	AccumulateBlock<Iterator, T>()(block_start, last, results[num_threads - 1]);
	gDisplay.lock();
	std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Thread accumulating remainder block finished..." << std::endl;
	gDisplay.unlock();

	return std::accumulate(results.begin(), results.end(), initial);
}

template<typename Iterator, typename T>
T SerialAccumulate(Iterator first, Iterator last, T initial)
{
	return std::accumulate(first, last, initial);
}

int GetRandomRange(int min, int max)
{
	static std::mt19937 engine{ std::random_device{}() };
	std::uniform_int_distribution dist{ min, max };

	return dist(engine);
}

 const std::vector<int> GenerateNumbers()
{
	 auto time = 0ll;
	 LARGE_INTEGER start;
	 LARGE_INTEGER end;

	time = 0ll;
	gDisplay.lock();
	std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Generating 100,000,000 random numbers between 1 and 100..." << std::endl;
	gDisplay.unlock();
	QueryPerformanceCounter(&start);
	std::vector<int> collection(100000000);
	for (auto& entry : collection)
	{
		entry = GetRandomRange(1, 100);
	}
	QueryPerformanceCounter(&end);
	time = end.QuadPart - start.QuadPart;
	gDisplay.lock();
	std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Duration (ticks): " << time << std::endl;
	gDisplay.unlock();

	return collection;
}

void DoSerialAccumulate(std::vector<int>& boogie)
{
	auto time = 0ll;
	LARGE_INTEGER start;
	LARGE_INTEGER end;

	time = 0ll;
	gDisplay.lock();
	std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Beginning serial accumulation..." << std::endl;
	gDisplay.unlock();
	QueryPerformanceCounter(&start);
	auto result_serial = SerialAccumulate<std::vector<int>::iterator, int>(boogie.begin(), boogie.end(), 0);
	QueryPerformanceCounter(&end);
	time = end.QuadPart - start.QuadPart;
	gDisplay.lock();
	std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Duration (ticks): " << time << std::endl;
	std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Result: " << result_serial << std::endl;
	std::wcout << std::endl;
	gDisplay.unlock();
}

int wmain(int argc, wchar_t argv[])
{
	int nCount = 2;
	bool bIsTrue = true;
	while (bIsTrue)
	{

		for (auto i = 0; i < nCount; ++i)
		{
			auto boogie = GenerateNumbers();


			auto time = 0ll;
			LARGE_INTEGER start;
			LARGE_INTEGER end;

			time = 0ll;
			gDisplay.lock();
			std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Beginning threaded accumulation..." << std::endl;
			gDisplay.unlock();
			QueryPerformanceCounter(&start);
			auto result_parallel = ParallelAccumulate<std::vector<int>::iterator, int>(boogie.begin(), boogie.end(), 0);
			// ParallelAccumulate should instead of joining when complete, sleep until an event is sent
			// ParallelAccumulate will need to be more complicated, first it will spawn threads and
			// then it will put them to sleep. We'll need globals for signaling and thread management.
			// We'll want the program logic to return to the main thread after the worker threads are
			// spawned so that we can execute the Serial Accumulate in this thread while the other seven
			// threads are processing their blocks in parallel. We will have to join the threads before
			// leaving the main logic loop, which will execute a fixed number of times, generating numbers
			// and then timing the results of the accumulate functions
			QueryPerformanceCounter(&end);
			time = end.QuadPart - start.QuadPart;
			gDisplay.lock();
			std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Duration (ticks): " << time << std::endl;
			std::wcout << L"[" << std::setfill(L'0') << std::setw(8) << std::this_thread::get_id() << L"] Result: " << result_parallel << std::endl;
			gDisplay.unlock();

			DoSerialAccumulate(boogie);
		}

		bIsTrue = false;
	}

	return 0;
}


//struct ThreadID
//{
//	ThreadID(std::thread::id id) : _id{ id } {}
//	std::thread::id _id;
//};
//std::wostream& operator<<(std::wostream& out, const ThreadID thread) { return out << std::setfill(L'0') << std::setw(8) << thread._id; }
//
//int main()
//{
//	ThreadID ID{ std::this_thread::get_id() };
//	std::wcout << ID;
//}



//void DoWork(unsigned duration)
//{
//	gDiplay.lock();
//	std::wcout << L"[" << std::this_thread::get_id() << L"] Sleeping for duration: " << duration << std::endl;
//	gDiplay.unlock();
//
//	std::this_thread::sleep_for(std::chrono::seconds(duration));
//
//	gDiplay.lock();
//	std::wcout << L"[" << std::this_thread::get_id() << L"] Duration " << duration << " thread joining..." << std::endl;
//	gDiplay.unlock();
//}
//
//class BackgroundTask
//{
//public:
//	void operator()(unsigned duration = 1) const
//	{
//		DoWork(duration);
//	}
//};

// Goes inside main() function...
//std::vector<std::thread> threads;
//for (auto i = 1u; i <= std::thread::hardware_concurrency(); ++i)
//{
//	threads.emplace_back(DoWork, i);
//}
//for (auto& thread : threads)
//{
//	thread.join();
//}
//// Create named functor thread
//BackgroundTaskFunctor btf;
//std::thread t{ btf, 1 };
//// Create unnamed functor thread
//std::thread u{ BackgroundTaskFunctor(), 2 };
//// Call function with Lambda
//std::thread v{ [](auto duration = 1u) { DoWork(duration); } };
//// Call function directly with argument
//std::thread w{ DoWork, 2 };
//t.join();
//u.join();
//v.join();
//w.join();

// A class to wrap a thread reference so that it will be joined by dtor
class ThreadGuard
{
	std::thread& mThread;
public:
	explicit ThreadGuard(std::thread& t)
		: mThread(t)
	{}
	~ThreadGuard()
	{
		if (mThread.joinable()) mThread.join();
	}
	ThreadGuard(ThreadGuard const&) = delete; // No copy ctor
	ThreadGuard& operator=(ThreadGuard const&) = delete; // No copy assign
};

// A class that holds a thread and joins it on dtor
class ScopedThread
{
	std::thread mThread;
public:
	explicit ScopedThread(std::thread t)
		: mThread(std::move(t))
	{
		if (!mThread.joinable()) throw std::logic_error("No thread");
	}
	~ScopedThread()
	{
		mThread.join();
	}
	ScopedThread(ScopedThread const&) = delete; // No copy ctor
	ScopedThread& operator=(ScopedThread const&) = delete; // No copy assign
};

// class JoiningThread
// {
//     std::thread mThread;
// public:
//     JoiningThread() noexcept = default;
//     template<typename Callable, typename ... Args>
//     explicit JoiningThread(Callable&& func, Args&& ... args)
//         : mThread(std::forward<Callable>(func), std::forward<Args>(args) ...)
//     {}
//     explicit JoiningThread(std::thread t) noexcept
//         : mThread(std::move(t))
//     {}
//     JoiningThread(JoiningThread&& rhs) noexcept
//         : mThread(std::move(rhs))
//     {}
//     JoiningThread& operator=(JoiningThread&& rhs) noexcept
//     {
//         if (Joinable()) Join();
//         mThread = std::move(rhs.mThread);
//         return *this;
//     }
//     JoiningThread& operator=(std::thread rhs) noexcept
//     {
//         if (Joinable()) Join();
//         mThread = std::move(rhs);
//         return *this;
//     }
//     ~JoiningThread() noexcept
//     {
//         if (Joinable()) Join();
//     }
//     JoiningThread(JoiningThread const &) = delete;
//     JoiningThread& operator=(JoiningThread const &) = delete;
//     void Swap(JoiningThread& rhs) noexcept
//     {
//         mThread.swap(rhs.mThread);
//     }
//     std::thread::id GetThreadId() const noexcept
//     {
//         return mThread.get_id();
//     }
//     bool Joinable() const noexcept
//     {
//         return mThread.joinable();
//     }
//     void Join()
//     {
//         mThread.join();
//     }
//     void Detach()
//     {
//         mThread.detach();
//     }
//     std::thread& as_thread() noexcept
//     {
//         return mThread;
//     }
//     const std::thread& as_thread() const noexcept
//     {
//         return mThread;
//     }
// };
