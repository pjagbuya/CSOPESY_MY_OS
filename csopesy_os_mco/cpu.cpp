#include "cpu.h"
std::vector<std::shared_ptr<CPUCore>> CPU::CPUCores;

int CPU::max_core = 0;
uint64_t CPU::quantum_cycles = 0;
uint64_t CPU::batch_process_freq = 0;
uint64_t CPU::delays_per_exec = 0;
std::string CPU::algoName = "";

uint64_t CPU::min_ins = 0;
uint64_t CPU::max_ins = 0;
int CPU::syncCheck = 0;
int CPU::cores_used = 0;
int CPU::cores_available = 0;
int CPU::core_utilization = 0;


CPU::CPU()
{

}

void CPU::initialize(int num_cpu, const std::string scheduler_algo, uint64_t quantum_cycles, uint64_t batch_process_freq, uint64_t delays_per_exec, uint64_t min_ins, uint64_t max_ins)
{
	this->max_core = num_cpu;
	this->algoName = scheduler_algo;
	this->quantum_cycles = quantum_cycles;
	this->batch_process_freq = batch_process_freq;
	this->delays_per_exec = delays_per_exec;
	this->min_ins = min_ins;
    this->max_ins = max_ins;
    this->syncCheck = 0;



    CPUCores.clear();

    auto startBarrier = std::make_shared<std::barrier<>>(num_cpu);
    auto endBarrier = std::make_shared<std::barrier<>>(num_cpu);
    this->max_core = num_cpu;

    for (int i = 0; i < num_cpu; ++i) {
        CPUCores.push_back(std::make_shared<CPUCore>(i));

        CPUCores[i]->setBarriers(startBarrier, endBarrier);
        CPUCores[i]->setBarrierCount(num_cpu);
        CPUCores[i]->start();


    }




}
void CPU::setToCore(std::shared_ptr<Process> p, int id)
{

    std::shared_ptr<CPUCore> temp = getCore();

    CPUCores.at(id)->setCurrentProcess(p);


}
void CPU::removeFromCore(int coreID)
{
    CPUCores.at(coreID)->stopCore();
}

void CPU::runCores()
{
    for (const auto& core : CPUCores) {
        if (core->isBusy()) {
            core->start();
        }
	}
}

void CPU::catchCoreStatus()
{
    cores_used = 0;
    cores_available = 0;
    core_utilization = 0;
    for(const auto& core : CPUCores) {
        if (core->isBusy()) {
            cores_used++;
        }
        else {
            cores_available++;
        }
	}
    double core_utilization_percentage = static_cast<double>(cores_used) / max_core * 100.0; // Example calculated value
    core_utilization = static_cast<int>(core_utilization_percentage);
}

bool CPU::checkSyncCheck()
{
    return (syncCheck = max_core);
}
void CPU::incSyncCheck(int coreID)
{

}
int CPU::getMaxCores() {
    return max_core;
}

uint64_t CPU::getQuantumCycles() {
    return quantum_cycles;
}

uint64_t CPU::getBatchProcessFreq() {
    return batch_process_freq;
}

uint64_t CPU::getDelays() {
    return delays_per_exec;
}

std::string CPU::getAlgoname() {
    return algoName;
}







std::shared_ptr<CPUCore> CPU::getCore()
{
    for (const auto& core : CPUCores) {
        if(!core->isBusy()) return core;
    }
    return nullptr;
}

std::vector<std::shared_ptr<CPUCore>> CPU::getAllCores()
{
    return CPUCores;
}


uint64_t CPU::getMinIns()
{
	return min_ins;
}
uint64_t CPU::getMaxIns()
{
    return max_ins;
}
void CPU::stopAllCores()
{
    int i = 0;
    for (const auto& core : CPUCores) {

        core->stopCore();
        core->stopPersistentThread();
    }
    for (const auto& core : CPUCores) {
        core->setReady();

        core->join();
    }


}


std::vector<std::string> CPU::CpuReport()
{
    catchCoreStatus();
    vector<std::string> temp;
    temp.push_back("CPU utilization: " + std::to_string(core_utilization) + "%");
    temp.push_back("Cores used: " + std::to_string(cores_used));
    temp.push_back("Cores used: " + std::to_string(cores_available));

    return temp;
}