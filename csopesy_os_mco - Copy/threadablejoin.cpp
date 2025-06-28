#include "threadablejoin.h"
ThreadableJ::ThreadableJ() : m_stopFlag(false)
{
}

ThreadableJ::~ThreadableJ()
{

}

void ThreadableJ::start() {
	this->m_stopFlag = false;

    m_thread = std::thread(&ThreadableJ::run, this);
}

void ThreadableJ::sleep(std::chrono::milliseconds ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void ThreadableJ::join() {
    if (m_thread.joinable()) {
        this->m_stopFlag = true;
        this->m_thread.join();
        
    }
}
