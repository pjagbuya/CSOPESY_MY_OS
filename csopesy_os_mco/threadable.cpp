#include "threadable.h"


Threadable::Threadable() : m_stopFlag(false)
{
}

Threadable::~Threadable()
{
    //if (m_thread.joinable()) {

    //    m_stopFlag.store(true, std::memory_order_relaxed);
    //    m_thread.join();
    //}
}

void Threadable::start() {
    m_stopFlag.store(false);
	std::thread(&Threadable::run, this).detach();
}

void Threadable::sleep(std::chrono::milliseconds ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
//void Threadable::join() {
//    if (m_thread.joinable()) {
//        m_thread.join();
//    }
//}
//void Threadable::stop() {
//    m_stopFlag.store(true, std::memory_order_relaxed);
//
//}
