#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

// // A class to wrap a thread reference so that it will be joined by dtor
class ThreadGuard
{
private:
    std::thread& mThread;
public:
    explicit ThreadGuard(std::thread& t)
        : mThread(t)
    {}
    ~ThreadGuard()
    {
        if (mThread.joinable()) mThread.join();
    }
    ThreadGuard(ThreadGuard const &) = delete;
    ThreadGuard& operator=(ThreadGuard const &) = delete;
};

// A class that holds a thread and joins it on dtor
class ScopedThread
{
private:
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
    ScopedThread(ScopedThread const &) = delete; // Copy ctor
    ScopedThread& operator=(ScopedThread const &) = delete; // Copy assign
};

std::mutex gDiplay;

void DoWork(unsigned duration)
{
    gDiplay.lock();
    std::wcout << L"[" << std::this_thread::get_id() << L"] Sleeping for duration: " << duration << std::endl;
    gDiplay.unlock();

    std::this_thread::sleep_for(std::chrono::seconds(duration));

    gDiplay.lock();
    std::wcout << L"[" << std::this_thread::get_id() << L"] Duration " << duration << " thread joining..." << std::endl;
    gDiplay.unlock();
}

class BackgroundTask
{
public:
    void operator()(unsigned duration = 1) const
    {
        DoWork(duration);
    }
};

#include <algorithm>

int wmain(int argc, wchar_t argv[])
{
    std::vector<std::thread> threads;
    
    for (unsigned i = 1; i <= std::thread::hardware_concurrency(); ++i)
    {
        threads.emplace_back(DoWork, i);
    }

    for (auto & thread : threads)
    {
        thread.join();
    }

    // Create named thread class
    BackgroundTask bt;
    std::thread t{ bt, 2 };

    // Create unnamed thread class
    std::thread u{ BackgroundTask(), 2 };

    // Call function directly with argument
    std::thread v{ DoWork, 1 };

    // Call function with Lambda
    std::thread w{ [](unsigned duration = 1){
        DoWork(duration);
    }};

    // Will join after v, w
    t.join();
    u.join();

    // Will join before t, u
    v.join();
    w.join();

    return 0;
}
















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
