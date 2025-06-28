#include "Scheduler.h"



// Constructor
Scheduler::Scheduler()
	: cpu_cycle(0), isOn(false), isStopScheduling(true)
{
	cpu = std::make_shared<CPU>();

}

void Scheduler::clearQueue(std::queue<std::shared_ptr<Process>>& queue) {  
    while (!queue.empty()) {  
        queue.pop();  
    }  
}

void Scheduler::schedulerExecuteAlgrithm()
{
	algo.executeAlgorithm();
}

size_t Scheduler::schedulerRandomizer(int min, int max)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_int_distribution<size_t> distribution(min, max);

	size_t randomIndex = distribution(generator);


	return randomIndex;
}

Scheduler::~Scheduler() {  
    clearQueue(readyQueue);  
	clearQueue(terminatedProcesses);

	setIsOn(false);
    screensList.clear();  
    processList.clear();  
}



void Scheduler::run()  {
	isOn = true;
	schedulerStart();
}

void Scheduler::initializeCPU(int num_cpu, const std::string& scheduler_algo, uint64_t quantum_cycles, uint64_t batch_process_freq, uint64_t delays_per_exec, uint64_t min_ins, uint64_t max_ins)
{
	cpu->initialize(num_cpu, scheduler_algo, quantum_cycles, batch_process_freq, delays_per_exec, min_ins, max_ins);

	algo.setAlgoType(CPU::getAlgoname());
	algo.initialize(this->readyQueue, this->terminatedProcesses);


}

std::shared_ptr<CPU> Scheduler::getCpu()
{
	return this->cpu;
}

int Scheduler::getCpuCycle() const
{
	return cpu_cycle;
}

std::string Scheduler::getMsgLog() const
{
	return msgLog;
}

std::vector<std::shared_ptr<Screen>> Scheduler::getScreenList() const
{
	return screensList;
}

std::queue<std::shared_ptr<Process>> Scheduler::getTerminatedProcesses()
{
	
	return this->algo.getTerminatedProcesses();
}
void Scheduler::popTerminatedProcesses() {
	algo.popTerminatedProcesses();
}
std::shared_ptr<Process> Scheduler::frontTerminatedProcesses() {
	return terminatedProcesses.front();
}

bool Scheduler::isThereTerminatedProcesses()
{
	return algo.hasTerminatedProcesses();
}

void Scheduler::resetIsThereTerminatedProcesses()
{
	algo.resetHasTerminatedProcesses();
}

bool Scheduler::getIsOn()
{
	std::lock_guard<std::mutex> lock(mtx);

	return isOn;
}

bool Scheduler::getIsStopScheduling()
{
	std::lock_guard<std::mutex> lock(mtx);

	return isStopScheduling;
}


void Scheduler::schedulerSetupScreenController(std::vector<std::shared_ptr<Screen>>& screensList, std::vector<std::shared_ptr<Process>>& processList)
{
	this->screensList = screensList;
	this->processList = processList;
}

void Scheduler::schedulerStart()
{


	setIsOn(true);
	Screen::setProcessCount(0);
	processList.clear();
	screensList.clear();
	while (getIsOn()) {

        uint64_t delay = CPU::getDelays();
		if(delay == 0)
			schedulerExecuteAlgrithm();
		else if (cpu_cycle % delay == 0)
			schedulerExecuteAlgrithm();


		uint64_t delay2 = CPU::getBatchProcessFreq();

		if (delay2 != 0 && !getIsStopScheduling() && cpu_cycle%delay2==0) {
			

			int currCount = Screen::getProcessCount();
			std::string processName;
			if (currCount < 10) {
				processName = "proc_0" + std::to_string(currCount);
			}
			else {
				processName = "proc_0" + std::to_string(currCount);

			}

			std::shared_ptr<Screen> newScreen = std::make_shared<Screen>(processName);

			std::shared_ptr<Process> p = newScreen->getAttachedProcess();



			processList.push_back(p);

			screensList.push_back(newScreen);


			ScreenController::getInstance()->addScreen(newScreen);

			ScreenController::getInstance()->setProcessAt(p->getPid(), p);


			addProcess(p);

			p->updateProcess();

			algo.pushToReadyQueue(p);



		}
		
		ScreenController::getInstance()->notifyCleanUpLoop();
		tick();
		Sleep(100); 
	}

}

// Stop the scheduler
void Scheduler::schedulerStop()
{

	setIsStopScheduling(true);

}

void Scheduler::schedulerDestroy()
{
	CPU::stopAllCores();

	setIsStopScheduling(true);
	setIsOn(false);
	clearQueue(readyQueue);
	clearQueue(terminatedProcesses);

}

void Scheduler::schedulerStartRandomize()
{

	setIsStopScheduling(false);
}

std::shared_ptr<ICommand> Scheduler::generateRandomCommand(int pid, int layer, uint64_t currCount, uint64_t maxCount)
{	
	if (!getIsOn() || layer > 3 || currCount >= maxCount) return nullptr;

	std::shared_ptr<Process> tempProcess = processList.at(pid);
	int type= (currCount % 2);

	switch (type) {
	case 0: {
		std::unordered_map<std::string, uint16_t> symbolTable = tempProcess->getSymbolTable();
		return std::make_shared<PrintCommand>(pid, "x");

	}

	case 1: {

		std::unordered_map<std::string, uint16_t> symbolTable = tempProcess->getSymbolTable();

		return std::make_shared<AddCommand>(pid, "x", "x", rand() % MAXNUMBER+1);

		
	}

	default:
		break;
	}

	return nullptr;

}

void Scheduler::addProcess(std::shared_ptr<Process>& p)
{
	std::lock_guard<std::mutex> lock(mtx);
	
	uint64_t maxProcesses = CPU::getMaxIns();
	uint64_t minProcesses = CPU::getMinIns();


	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(minProcesses, maxProcesses);

	uint64_t instructionCount = dist(gen);

	for(int i = 0; i < instructionCount; ++i) {

		auto cmd = generateRandomCommand(p->getPid(), 0, i, instructionCount);

		if (cmd) {
			p->addCommand(cmd);

		}

	}


}

void Scheduler::pushToReadyQueue(std::shared_ptr<Process> p) {
	algo.pushToReadyQueue(p);
}

void Scheduler::removeProcess(int pid)
{


}

void Scheduler::setIsOn(bool var)
{
	std::lock_guard<std::mutex> lock(mtx);
	isOn = var;
}

void Scheduler::setIsStopScheduling(bool var)
{
	std::lock_guard<std::mutex> lock(mtx);
	isStopScheduling = var;
}

void Scheduler::tick()
{
	cpu_cycle++;

}

std::queue<std::shared_ptr<Process>> Scheduler::getReadyQueue()
{
	return algo.getReadyQueue();
}

std::queue<std::shared_ptr<Process>> Scheduler::getRunningQueue()
{
	return algo.getRunningQueue();
}
