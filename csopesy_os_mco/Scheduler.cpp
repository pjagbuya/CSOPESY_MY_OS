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

		schedulerExecuteAlgrithm();

		if (!getIsStopScheduling()) {
			

			for (int i = 0; i < CPU::getBatchProcessFreq(); i++) {
				int currCount = Screen::getProcessCount();
				std::string processName;
				if (currCount < 10) {
					processName = "process_0" + std::to_string(currCount);
				}
				else {
					processName = "process_" + std::to_string(currCount);

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


		}
		
		ScreenController::getInstance()->notifyCleanUpLoop();

		tick();

		//std::this_thread::sleep_for(std::chrono::milliseconds(CPU_TICKS_MS));
	}

}

// Stop the scheduler
void Scheduler::schedulerStop()
{

	setIsStopScheduling(true);

}

void Scheduler::schedulerDestroy()
{
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

	CommandType type = static_cast<CommandType>(rand() % 6);
	std::shared_ptr<Process> tempProcess = processList.at(pid);


	switch (type) {
	case PRINT: {
		int caseType = rand() % 2;
		std::unordered_map<std::string, uint16_t> symbolTable = tempProcess->getSymbolTable();
		std::string chosenVariable;
		if (caseType == 1 && symbolTable.size() > 0) {
			std::vector<std::string> keys;
			for (const auto& pair : symbolTable) {
				keys.push_back(pair.first);
			}



			size_t randomIndex = schedulerRandomizer(0, keys.size() - 1);

			chosenVariable = keys[randomIndex];

			return std::make_shared<PrintCommand>(pid, chosenVariable);

		}

		return std::make_shared<PrintCommand>(pid, "Hello World!");

	}
	case DECLARE: {
		std::string varname = "var" + std::to_string(tempProcess->getVariableCounter());
		return std::make_shared<DeclareCommand>(pid, varname, 10 + rand() % MAXNUMBER);
	}
	case ADD: {
		int caseType = rand() % 4;
		std::unordered_map<std::string, uint16_t> symbolTable = tempProcess->getSymbolTable();
		std::string chosenVariable;
		std::vector<std::string> keys;
		if (symbolTable.size() == 0) {
			std::string varname = "var" + std::to_string(tempProcess->getVariableCounter());
			return std::make_shared<DeclareCommand>(pid, varname, 10 + rand() % MAXNUMBER);
		}
		for (const auto& pair : symbolTable) {
			keys.push_back(pair.first);


		}
		if (caseType==0) {

			size_t randomIndex = schedulerRandomizer(0, keys.size() - 1);
			std::string chosenVariable1 = keys[randomIndex];

			randomIndex = schedulerRandomizer(0, keys.size() - 1);
			std::string chosenVariable2 = keys[randomIndex];


			std::string varname = "var" + std::to_string(tempProcess->getVariableCounter());

			return std::make_shared<AddCommand>(pid, varname, symbolTable[chosenVariable1], symbolTable[chosenVariable2]);


		}
		else if (caseType==1){
			size_t randomIndex = schedulerRandomizer(0, keys.size() - 1);
			std::string chosenVariable2 = keys[randomIndex];

			std::string varname = "var" + std::to_string(tempProcess->getVariableCounter());

			return std::make_shared<AddCommand>(pid, varname, rand() % MAXNUMBER, symbolTable[chosenVariable2]);

		}
		else if (caseType == 2) {
			size_t randomIndex = schedulerRandomizer(0, keys.size() - 1);
			std::string chosenVariable1 = keys[randomIndex];

			std::string varname = "var" + std::to_string(tempProcess->getVariableCounter());

			return std::make_shared<AddCommand>(pid, varname, symbolTable[chosenVariable1], rand() % MAXNUMBER);
		}
		else {
			std::string varname = "var" + std::to_string(tempProcess->getVariableCounter());

			return std::make_shared<AddCommand>(pid, varname, rand() % MAXNUMBER, rand() % MAXNUMBER);

		}
	}
	case SUBTRACT:
	{
		int caseType = rand() % 4;
		std::unordered_map<std::string, uint16_t> symbolTable = tempProcess->getSymbolTable();
		std::string chosenVariable;
		std::vector<std::string> keys;
		for (const auto& pair : symbolTable) {
			keys.push_back(pair.first);


		}
		if (symbolTable.size() == 0) {
			std::string varname = "var" + std::to_string(tempProcess->getVariableCounter());
			return std::make_shared<DeclareCommand>(pid, varname, 10 + rand() % MAXNUMBER);
		}
		if (caseType == 0) {

			size_t randomIndex = schedulerRandomizer(0, keys.size() - 1);
			std::string chosenVariable1 = keys[randomIndex];

			randomIndex = schedulerRandomizer(0, keys.size() - 1);
			std::string chosenVariable2 = keys[randomIndex];


			std::string varname = "var" + std::to_string(tempProcess->getVariableCounter());

			uint16_t bigger = symbolTable[chosenVariable1];
			uint16_t smaller = symbolTable[chosenVariable2];
			if(bigger > smaller) {
				return std::make_shared<SubtractCommand>(pid, varname, bigger, smaller);

			}
			else {
				return std::make_shared<SubtractCommand>(pid, varname, smaller, bigger);

			}



		}
		else if (caseType == 1) {
			size_t randomIndex = schedulerRandomizer(0, keys.size() - 1);
			std::string chosenVariable2 = keys[randomIndex];

			std::string varname = "var" + std::to_string(tempProcess->getVariableCounter());

			uint16_t bigger = rand() % MAXNUMBER;
			uint16_t smaller = symbolTable[chosenVariable2];
			if (bigger > smaller) {
				return std::make_shared<SubtractCommand>(pid, varname, bigger, smaller);

			}
			else {

				return std::make_shared<SubtractCommand>(pid, varname, smaller, bigger);

			}


		}
		else if (caseType == 2) {

			size_t randomIndex = schedulerRandomizer(0, keys.size() - 1);
			std::string chosenVariable1 = keys[randomIndex];

			std::string varname = "var" + std::to_string(tempProcess->getVariableCounter());
			uint16_t bigger = symbolTable[chosenVariable1];
			uint16_t smaller = rand() % MAXNUMBER;
			if (bigger > smaller) {
				return std::make_shared<SubtractCommand>(pid, varname, bigger, smaller);

			}
			else {

				return std::make_shared<SubtractCommand>(pid, varname, smaller, bigger);

			}
		}
		else {
			uint16_t bigger = rand() % MAXNUMBER;
			uint16_t smaller = rand() % MAXNUMBER;

			std::string varname = "var" + std::to_string(tempProcess->getVariableCounter());

			if (bigger > smaller) {
				return std::make_shared<SubtractCommand>(pid, varname, bigger, smaller);

			}
			else {

				return std::make_shared<SubtractCommand>(pid, varname, smaller, bigger);

			}
		}
	}
	case SLEEP:
		return std::make_shared<SleepCommand>(pid, rand() % 100);
	case FOR:
		if (layer < 3) {
			
			int forCounterRand = 1 + rand() % MAX_FOR_COUNTER;
			auto loop = std::make_shared<ForCommand>(pid, forCounterRand, layer + 1);
			
			int num_comms = rand() % COMMANDS_PER_FOR;
			for (int i = 0; i < num_comms; ++i) {

				// Each command is ran by forCounterRand times
				if(currCount + forCounterRand > maxCount){
					break;
				}

				loop->addCommand(generateRandomCommand(pid, layer + 1, currCount, maxCount));
				currCount += forCounterRand;

			}
			return loop;
		}
		break;
	default:
		break;
	}

	return nullptr;

}

void Scheduler::addProcess(std::shared_ptr<Process>& p)
{
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