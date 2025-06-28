#include "Algorithm.h"  

Algorithm::Algorithm() {  
    isThereTerminated = false;
}

void Algorithm::setAlgoType(std::string algoName)
{
    if (algoName == "FCFS" || algoName == "fcfs")
    {
        this->algo = FCFS;
    }
    else if(algoName =="RR" || algoName == "rr") {
		this->algo = RR;
    }
    else {
        this->algo = FCFS;
    }

}

void Algorithm::initialize(std::queue<std::shared_ptr<Process>>& readyQueue, std::queue<std::shared_ptr<Process>>& terminatedProcesses)
{
	CPUCores = CPU::getAllCores();
    int coresSize = CPUCores.size();
    for (size_t i = 0; i < coresSize; ++i) {
        quantum_cycles[i] = 0;
    }
    this->maxQC = CPU::getQuantumCycles();


    this->readyQueue = readyQueue;
    this->terminatedProcesses = terminatedProcesses;

}

void Algorithm::pushToReadyQueue(std::shared_ptr<Process> p)
{
    readyQueue.push(p);
}

std::queue<std::shared_ptr<Process>> Algorithm::getTerminatedProcesses()
{
    return terminatedProcesses;
}

void Algorithm::popTerminatedProcesses()
{
    terminatedProcesses.pop();
}

std::shared_ptr<Process> Algorithm::frontTerminatedProcesses()
{
    return terminatedProcesses.front();
}



void Algorithm::executeAlgorithm() {
    switch (algo) {
        case RR:
			runRRAlgo();
            break;
        case::FCFS:
            runFCFSAlgo();
            break;
    }

}

void Algorithm::runFCFSAlgo()
{

    for (size_t i = 0; i < CPU::getMaxCores(); ++i) {
        if (CPUCores.empty()) {
            break;
        }


        if (!CPUCores[i]->isBusy() && !readyQueue.empty()) {
            shared_ptr<Process> process = readyQueue.front();
            readyQueue.pop();
            shared_ptr<Process> processInCore = CPUCores[i]->getCurrentProcess();
            if (processInCore != nullptr) {
				isThereTerminated = true;
                processInCore->setProcessState(ProcessState::FINISHED);
                processInCore->setCpuCoreID(-1);
                terminatedProcesses.push(processInCore);
                CPUCores[i]->stopCore();

            }
            process->setCPUCoreID(i);
            process->setProcessState(ProcessState::RUNNING);
            CPUCores[i]->setCurrentProcess(process);
            CPUCores[i]->setBusy(true);

        } 
        // This case is for wala ng new process dumating but theres one last process in the CPU Core
        else{
            std::shared_ptr<Process> currentProcess = CPUCores[i]->getCurrentProcess();
            if (currentProcess!= nullptr && currentProcess->isFinished()) {
                isThereTerminated = true;
                terminatedProcesses.push(currentProcess);
                currentProcess->setCPUCoreID(-1);
                currentProcess->setProcessState(ProcessState::FINISHED);
                CPUCores[i]->stopCore();
                CPUCores[i]->setCurrentProcess(nullptr);

            }
        }

        CPUCores[i]->setReady();
    }

    //int newSyncNum = coresWithProcess.size();

    //std::shared_ptr<std::barrier<>> startBarrier = std::make_shared<std::barrier<>>(newSyncNum);
    //std::shared_ptr<std::barrier<>> endBarrier = std::make_shared<std::barrier<>>(newSyncNum);

    //for (int& id : coresWithProcess) {

    //    CPUCores[id]->setBarriers(startBarrier, endBarrier);
    //    CPUCores[id]->setBarriers(startBarrier, endBarrier);



    //}


    //for (int& id : coresWithProcess) {

    //    CPUCores[id]->join();

    //}
    //startBarrier = std::make_shared<std::barrier<>>(0);
    //endBarrier = std::make_shared<std::barrier<>>(0);
    //for (int& id : coresWithProcess) {
    //    CPUCores[id]->setBarriers(startBarrier, endBarrier);

    //}
}

void Algorithm::runRRAlgo()
{

    for (size_t i = 0; i < CPUCores.size(); i++) {
        //shared_ptr<CPUCore>& core = ;

        if (!CPUCores[i]) {
            break;
        }

        if (CPUCores[i]->isBusy() ) {

            auto process = CPUCores[i]->getCurrentProcess();
            if (process->isFinished()) {
                terminatedProcesses.push(process);
                process->setCPUCoreID(-1);
                process->setProcessState(ProcessState::FINISHED);
                
                isThereTerminated = true;

                quantum_cycles[i] = 0;
                CPUCores[i]->stopCore();
                CPUCores[i]->setBusy(false);
                CPUCores[i]->setCurrentProcess(nullptr);



            }
            else if (quantum_cycles[i] >= this->maxQC) {
                readyQueue.push(process); // preempt
                process->setCPUCoreID(-1);
                process->setProcessState(ProcessState::WAITING);
                CPUCores[i]->stopCore();
                quantum_cycles[i] = 0;
                CPUCores[i]->setBusy(false);
                CPUCores[i]->setCurrentProcess(nullptr);
            }
            else {
                quantum_cycles[i]++;

            }
        }
        if (!CPUCores[i]->isBusy() && !readyQueue.empty()) {
            shared_ptr<Process> process = readyQueue.front();
            readyQueue.pop();


            process->setCPUCoreID(i);
            process->setProcessState(ProcessState::RUNNING);
            CPUCores[i]->setCurrentProcess(process);
            CPUCores[i]->setBusy(true);


            quantum_cycles[i] = 0;

        }

        CPUCores[i]->setReady();

    }

    

    //startBarrier = std::make_shared<std::barrier<>>(0);
    //endBarrier = std::make_shared<std::barrier<>>(0);
    //for (int& id : coresWithProcess) {
    //    CPUCores[id]->setBarriers(startBarrier, endBarrier);

    //}
}

void Algorithm::clearQueues()
{
    while (!this->terminatedProcesses.empty()) {
        this->terminatedProcesses.pop();
    }
    while (!this->readyQueue.empty()) {
        this->readyQueue.pop();
    }
    while (!this->runningQueue.empty()) {
        this->runningQueue.pop();
    }
}

void Algorithm::clearRunning()
{
    while (!this->runningQueue.empty()) {
        this->runningQueue.pop();
    }
}

std::queue<std::shared_ptr<Process>> Algorithm::getReadyQueue()
{
    return readyQueue;
}

std::queue<std::shared_ptr<Process>> Algorithm::getRunningQueue()
{
    return runningQueue;
}
