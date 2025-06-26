#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include "CPUCore.h"
#include "process.h"

using namespace std;

class Scheduler;
class CPUCore;
class Process;

class CPU {
private:
	

	static std::mutex mtx;
	static std::vector<std::shared_ptr<CPUCore>> CPUCores;
	static int syncCheck;
	static int max_core;
	static uint64_t quantum_cycles;
	static uint64_t batch_process_freq;
	static uint64_t delays_per_exec;
	static std::string algoName;
	static uint64_t min_ins;
	static uint64_t max_ins;



public:
	CPU(); // Constructor remains simple

	void initialize(int num_cpu,
		const std::string scheduler_algo,
		uint64_t quantum_cycles,
		uint64_t batch_process_freq,
		uint64_t delays_per_exec,
		uint64_t min_ins,
		uint64_t max_ins); // You might need these for process generation
	
	
	static void setToCore(std::shared_ptr<Process>p, int id);
	static void removeFromCore(int coreID);
	static void runCores();

	static bool checkSyncCheck();
	static void incSyncCheck(int coreID);
	static int getMaxCores();
	static uint64_t getQuantumCycles();
	static uint64_t getBatchProcessFreq();
	static uint64_t getDelays();
	static std::string getAlgoname();

	static std::shared_ptr<CPUCore> getCore(); // can return nullptr if no core is available
	static std::vector<std::shared_ptr<CPUCore>> getAllCores();
	//static int CPUCores();
	static uint64_t getMinIns();
	static uint64_t getMaxIns();

	static void stopAllCores();

	void CpuReport();



};