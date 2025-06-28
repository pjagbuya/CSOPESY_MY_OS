#pragma once

#include <thread>
class Threadable
{
public:

	Threadable();
	~Threadable();

	void start();
	static void sleep(std::chrono::milliseconds ms);  



	virtual void run() = 0;
protected:
    // This atomic flag is used to signal the `run()` loop to terminate
    std::atomic<bool> m_stopFlag;
    std::thread m_thread;

private:



};
