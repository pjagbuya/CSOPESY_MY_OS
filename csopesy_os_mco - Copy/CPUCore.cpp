#include "CPUCore.h"

CPUCore::CPUCore(int id)
{
    this->core_id = id;
    this->busy = false;
    this->ready = false;
    this->leave.store(false);
}


CPUCore::~CPUCore()
{
}

void CPUCore::run() {
    while (!m_stopFlag) {

        std::unique_lock<std::mutex> lock(mtx);
        cv.wait_for(lock, std::chrono::milliseconds(1000), [&] {
            return ready.load() || m_stopFlag.load();
            });

        if(m_stopFlag.load())
            break;

        std::unique_lock<std::mutex> process_exec_lock(m_currProcessMutex);
        if (curr_p && ready.load()) {
            if (!curr_p && curr_p->isFinished()) {
                stopCore();
            }
            else {
                curr_p->executeCurrentCommand();
                curr_p->ConsoleLogPush();
                curr_p->moveToNextLine();
            }



        }

        process_exec_lock.unlock();
        ready.store(false);

        if(endBarrier)
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
    std::unique_lock<std::mutex> lock(m_currProcessMutex);
    if (p == nullptr) {
        if (this->curr_p) { 
            this->curr_p->updateSymbolTable("INTERRUPT", 1);
        }
        this->curr_p.reset();
    }
    else {
        this->curr_p = p;

    }

}

std::shared_ptr<Process> CPUCore::getCurrentProcess()
{
    std::unique_lock<std::mutex> lock(m_currProcessMutex);

	return this->curr_p;
}

void CPUCore::setBarriers(std::shared_ptr<std::barrier<>> startBarrier, std::shared_ptr<std::barrier<>> endBarrier)
{
    this->startBarrier = startBarrier;
	this->endBarrier = endBarrier;
}

void CPUCore::setReady()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready.store(true);
    }

    
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

    std::unique_lock<std::mutex> lock(m_currProcessMutex);
    if (curr_p) {
        this->curr_p->updateSymbolTable("INTERRUPT", 1);
        this->curr_p.reset();
    }
    lock.unlock();
    this->m_stopFlag.store(true);
    setReady();
}

bool CPUCore::isLeave()
{
    return leave.load();
}

