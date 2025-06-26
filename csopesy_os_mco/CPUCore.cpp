#include "CPUCore.h"

CPUCore::CPUCore(int id)
{
    this->core_id = id;
    this->busy = false;
    this->ready = false;
}


CPUCore::~CPUCore()
{

}

void CPUCore::run() {
    while (!m_stopFlag) {

        std::unique_lock<std::mutex> lock(mtx);
        cv.wait_for(lock, std::chrono::milliseconds(1000), [&] {
            return ready.load() || m_stopFlag;
            });
        if (m_stopFlag) break;
        if (curr_p && ready.load()) {
            sleep(std::chrono::milliseconds(CPU::getDelays()));

            this->curr_p->executeCurrentCommand();
            this->curr_p->ConsoleLogPush();
            this->curr_p->moveToNextLine();
            if (this->curr_p->isFinished()) {
                stopCore();
            }

        }
        ready.store(false);

        if(endBarrier||!m_stopFlag)
            endBarrier->arrive_and_wait();


    }
}
bool CPUCore::isBusy() const
{

    return this->busy.load();
}

void CPUCore::setBusy(bool busy)
{
    this->busy.store(busy);
}

int CPUCore::getCoreId() const
{
    return core_id;
}

void CPUCore::setCoreId(int id)
{
    core_id = id;
}


void CPUCore::setCurrentProcess(std::shared_ptr<Process> p)
{
    std::unique_lock<std::mutex> lock(mtx);

	this->curr_p = p;
}

std::shared_ptr<Process> CPUCore::getCurrentProcess()
{
    std::unique_lock<std::mutex> lock(mtx);
	return this->curr_p;
}

void CPUCore::setBarriers(std::shared_ptr<std::barrier<>> startBarrier, std::shared_ptr<std::barrier<>> endBarrier)
{
    this->startBarrier = startBarrier;
	this->endBarrier = endBarrier;
}

void CPUCore::setReady()
{

    ready.store(true);
    
    cv.notify_one();

}


void CPUCore::setBarrierCount(int count)
{
	barrierCount = count;
}

int CPUCore::getBarrierCount()
{
    return barrierCount;
}

void CPUCore::stopCore()
{
    setBusy(false);

}



void CPUCore::stopPersistentThread()
{
    this->m_stopFlag.store(true);
    setReady();
}

