#pragma once  
#include <atomic> // Include the correct header for std::atomic  
#include <memory>
#include <barrier>   // C++20 for synchronization
#include <chrono>
#include "threadablejoin.h"  
#include "process.h"  
class Process;
class CPUCore : public ThreadableJ  
{  
public:  
    CPUCore(int id);  
    ~CPUCore();  

    void run() override;  

    bool isBusy() const;  
    void setBusy(bool busy);  
    int getCoreId() const;
    void setCoreId(int id);
    void setCurrentProcess(std::shared_ptr<Process> p);  
    std::shared_ptr<Process> getCurrentProcess();
	void setBarriers(std::shared_ptr<std::barrier<>> startBarrier, std::shared_ptr<std::barrier<>> endBarrier);
	void setReady();
	void setBarrierCount(int count);
    int getBarrierCount();
    void stopCore();  
    void stopPersistentThread();
    bool isLeave();
private:  
    int core_id;  
    std::atomic<bool> busy; // Use std::atomic instead of atomic  
    std::mutex mtx;
    std::mutex m_currProcessMutex;
    std::shared_ptr<Process> curr_p;  
    std::atomic<bool> ready;
    std::condition_variable cv;
    std::atomic<bool> leave;
    int pid;


	int barrierCount = 0; 
    std::shared_ptr<std::barrier<>> startBarrier; // Barrier to synchronize start of all cores
    std::shared_ptr<std::barrier<>> endBarrier;   // Barrier to synchronize end of all cores
};
