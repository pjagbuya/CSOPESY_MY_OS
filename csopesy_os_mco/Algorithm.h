#pragma once
#include <queue>
#include <memory>
#include "CPUCore.h"
#include "process.h"
#include "cpu.h"

// TODOS: uninitialized private variables from functions with scheduler
class CPUCore;
class Process;
enum AlgoType
{
	RR,// Round Robin
	FCFS,// First Come First Serve
};
class Algorithm
{


public:
	Algorithm();
	void setAlgoType(std::string algoName);
	void initialize(std::queue<std::shared_ptr<Process>>& readyQueue,	
	                std::queue<std::shared_ptr<Process>>& terminatedProcesses);
	void pushToReadyQueue(std::shared_ptr<Process> p);
	std::queue<std::shared_ptr<Process>> getTerminatedProcesses();
	void popTerminatedProcesses();
	std::shared_ptr<Process> frontTerminatedProcesses();


	bool hasTerminatedProcesses() { return isThereTerminated; };
	void resetHasTerminatedProcesses() { isThereTerminated = false; }
	void executeAlgorithm();
	void runFCFSAlgo();

	void runRRAlgo();

private:
	AlgoType algo;
	std::queue<std::shared_ptr<Process>> readyQueue;
	std::queue<std::shared_ptr<Process>> terminatedProcesses;

	bool isThereTerminated;

	int maxQC;
	std::vector<std::shared_ptr<CPUCore>> CPUCores;

	std::array<uint64_t, 128> quantum_cycles;

};

