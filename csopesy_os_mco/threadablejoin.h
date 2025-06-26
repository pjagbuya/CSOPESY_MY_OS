#pragma once

#include <thread>
#include <atomic>
#include <chrono>
#include <stdexcept>
class ThreadableJ
{

public:
    ThreadableJ();
    ~ThreadableJ();

    void start();
    static void sleep(std::chrono::milliseconds ms);
	void join();


    virtual void run() = 0;
protected:
    std::atomic<bool> m_stopFlag;
    std::thread m_thread;

private:

};

