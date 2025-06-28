#pragma once

#include <queue>
#include <atomic>
#include <memory>
#include <iostream>
#include "threadable.h"
#include "process.h"
#include "screen_controller.h"
#include "AddCommand.h"
#include "SubtractCommand.h"
#include "DeclareCommand.h"
#include "SleepCommand.h"
#include "ForCommand.h"
#include "PrintCommand.h"
#include "Algorithm.h"



#define CPU_TICKS_MS 10
#define COMMANDS_PER_FOR 10
#define MAX_FOR_COUNTER 10
#define MAXNUMBER 10
#define MAXSLEEPMILLISECONDS 200


class CPU;
class Screen;
class Scheduler : public Threadable
{
public:
	Scheduler();
	~Scheduler();

	void schedulerSetupScreenController(std::vector<std::shared_ptr<Screen>>& screensList,
										std::vector<std::shared_ptr<Process>>& processList); // sets up connections of lists from screen controller to scheduler
	void schedulerStart(); // starts only the ticks and thread
	void schedulerStop(); // stops the process of randomizing and creating processes
	void schedulerDestroy(); // Destroys actual running thread and ticks
	void schedulerStartRandomize();

	void clearQueue(std::queue<std::shared_ptr<Process>>& queue);

	void schedulerExecuteAlgrithm(); // starts the scheduling algorithm, should be called after the CPU is initialized
	size_t schedulerRandomizer(int min, int max);


	std::shared_ptr<ICommand> generateRandomCommand(int pid, int layer, uint64_t currCount, uint64_t maxCount);
	void addProcess(std::shared_ptr<Process>& p);
	void removeProcess(int pid);
	void run() override;


	void initializeCPU(int num_cpu, const std::string& scheduler_algo, uint64_t quantum_cycles, uint64_t batch_process_freq, uint64_t delays_per_exec, uint64_t min_ins, uint64_t max_ins);
	std::shared_ptr<CPU> getCpu();
	int getCpuCycle() const;
	std::string getMsgLog() const;
	std::vector<std::shared_ptr<Screen>> getScreenList() const;
	void popTerminatedProcesses();
	std::shared_ptr<Process> frontTerminatedProcesses();

	bool isThereTerminatedProcesses();
	void resetIsThereTerminatedProcesses();

	bool getIsOn();
	bool getIsStopScheduling();

	void setIsOn(bool var);
	void setIsStopScheduling(bool var);


	void tick();
	std::queue<std::shared_ptr<Process>> getReadyQueue();
	std::queue<std::shared_ptr<Process>> getTerminatedProcesses();
	std::queue<std::shared_ptr<Process>> getRunningQueue();


private:
	Algorithm algo;
	std::mutex mtx;
	int cpu_cycle; // init
	std::shared_ptr<CPU> cpu; 
	uint64_t delay;
	uint64_t max_ins;
	uint64_t min_ins;

	std::queue<std::shared_ptr<Process>> readyQueue;
	std::queue<std::shared_ptr<Process>> terminatedProcesses;

	std::atomic<bool> isOn; 
	std::atomic<bool> isStopScheduling;


	std::vector<std::shared_ptr<Screen>> screensList;
	std::vector<std::shared_ptr<Process>> processList;



	std::string msgLog;
	friend class ScreenController;

};

