#include "screen_controller.h"

#include "MainConsole.h"
#include "Marquee.h"


ScreenController* ScreenController::sharedInstance = nullptr;
ScreenController* ScreenController::getInstance()
{
	return sharedInstance;
}

void ScreenController::initialize()  
{  
	sharedInstance = new ScreenController();  
	sharedInstance->readConfigFileAndInitializeCPU("config.txt");  

}

void ScreenController::destroy()
{
	delete sharedInstance;
}



ScreenController::ScreenController() {
	isSchedulerOn = false;
	sharedInstance = this; // Set early!
	exec_delay = 10; // Default execution delay
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	inputMan = InputMan();
	input = "";
	action = -999;
	sKeys = -1;
	screens = {};
	isInScreen = false;
	isCmdQuit = false;
	isCmdDone = false;
	isCmdClear = false;
	isThreadedMarquee = false;
	main_screen = std::make_shared<MainConsole>();
	backup_main_screen = main_screen;
	marquee = std::make_shared<Marquee>();
	scheduler = std::make_shared<Scheduler>();
	scheduler->start();
	scheduler->schedulerSetupScreenController(screens, processList);

}


// Ckleans up main_screen and backup main Screen
// Call deconstructors for INputMan
// Process table is cleared
// Cleanup thread is called to stop 
//
ScreenController::~ScreenController() {
	ScreenController::main_screen.reset();
	ScreenController::backup_main_screen.reset();
	ScreenController::inputMan.~InputMan();
	stopCleanupThread = true;
	cleanupCv.notify_one();
	if (cleanupThread.joinable()) {
		cleanupThread.join();
	}

	ScreenController::screens = {};
	process_table.clear();
	scheduler->schedulerDestroy();


}

// Read file for parameters
// Clean up the processes that are finished on the screen so they won't be accessible anymore.
void ScreenController::readConfigFileAndInitializeCPU(const std::string& filename)
{
	std::ifstream file(filename);

	if (!file.is_open()) {
		throw std::runtime_error("Failed to open config file");
	}

	std::string line;
	int num_cpu = 1;
	std::string scheduler_algo = "fcfs";
	uint64_t  quantum_cycles = 1;
	uint64_t  batch_process_freq = 1;
	uint64_t  delays_per_exec = 0;
	uint64_t  min_ins = 1;
	uint64_t  max_ins = 1;

	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#') continue;

		std::istringstream iss(line);
		std::string key;
		std::string value;

		iss >> key >> value;
		if (!value.empty() && value.front() == '"' && value.back() == '"') {
			value = value.substr(1, value.size() - 2);
		}
		if (key == "num-cpu") {
			num_cpu = std::stoi(value);
		}
		else if (key == "scheduler") {
			scheduler_algo = value;
		}
		else if (key == "quantum-cycles") {
			quantum_cycles = std::stoull(value);
		}
		else if (key == "batch-process-freq") {
			batch_process_freq = std::stoull(value);
		}
		else if (key == "delays-per-exec") {
			delays_per_exec = std::stoull(value);
			this->exec_delay = delays_per_exec; 
		}
		else if (key == "min-ins") {
			min_ins = std::stoull(value);
		}
		else if (key == "max-ins") {
			max_ins = std::stoull(value);
		}
	}

	if (min_ins > max_ins) {
		throw std::invalid_argument("min-ins must be <= max-ins");
	}


	this->scheduler->initializeCPU(num_cpu, scheduler_algo, quantum_cycles,
		batch_process_freq, delays_per_exec,
		min_ins, max_ins);
	cleanupThread = std::thread(&ScreenController::cleanUploop, this);

}

std::shared_ptr<Screen> ScreenController::findScreen(std::string& name) {


	
	for (auto& screen_ptr : ScreenController::screens) {
		if (screen_ptr->getName() == name) {
			return screen_ptr; // Return the shared_ptr itself
		}
	}
	return nullptr;
}



bool ScreenController::isScreenHere(std::string& name) {
	std::shared_ptr<Screen> tempScreen;
	tempScreen = ScreenController::getInstance()->findScreen(name);

	if (tempScreen == nullptr) {
		return false;
	}
	else {
		

		return true;
	}
}

std::shared_ptr<Screen> ScreenController::createScreen(std::string& name) {

	std::lock_guard<std::mutex> lock(mtx);
	if (isScreenHere(name)) {
		swapScreen(name);
		return findScreen(name);
	}
	else {
		Screen::incProcessCount();

		std::shared_ptr<Screen> newScreen = std::make_shared<Screen>(name);

		processList.push_back(newScreen->getAttachedProcess());
		screens.push_back(newScreen);
		swapScreen(newScreen);
		Screen::incProcessCount();
		return newScreen;
	}
}

void ScreenController::addScreen(std::shared_ptr<Screen> temp)
{
	std::lock_guard<std::mutex> lock(mtx);
	screens.push_back(temp);

}


void ScreenController::swapScreen(std::shared_ptr<Screen> newScreen) {
	if (newScreen) {
		main_screen->ConsoleFlush(0);
		main_screen = newScreen;
	}
	else {
		std::cerr << "Error, screen name '" + newScreen->getName() + "' NOT FOUND. Please try a different name." << endl;
		return;
	}



}
void ScreenController::swapScreen(std::string name) {

	main_screen->ConsoleFlush(0);
	std::shared_ptr<AConsole> foundScreen = findScreen(name);
	if (foundScreen) {
		main_screen = foundScreen;
	}
	else {
		std::cerr << "Error, screen name '" + name + "' NOT FOUND. Please try a different name." << endl;
		return;
	}



}


HANDLE ScreenController::getConsoleHandle()
{
	return getInstance()->consoleHandle;
}
void ScreenController::doThreadedMarquee() {
	action = -999;
	sKeys = -1;

	// Modifies and clears input
	inputMan.Input(input, sKeys, isCmdDone);

	// Modifies this action and isCmdDone

	if (isCmdDone)
	{
		inputMan.CLI_Comms(input, action, isCmdDone, match);
		main_screen->ParseAction(input, action);

	}
	main_screen->ParseSkeys(action, sKeys);

	main_screen->display();
	Sleep(20);

}
void ScreenController::callInputListener() {
	action = -999;
	sKeys = -1;

	// Modifies and clears input
	inputMan.Input(input, sKeys, isCmdDone);

	// Modifies this action and isCmdDone
	
	if (isCmdDone)
	{
		inputMan.CLI_Comms(input, action, isCmdDone, match);
		main_screen->ParseAction(input, action);

	}
	main_screen->ParseSkeys(action, sKeys);

	main_screen->display();
	
	Sleep(10); 

}
void ScreenController::swapToMarquee() {
	marquee->onEnabled();
	main_screen = marquee;
}
void ScreenController::backToMain() {
	main_screen = backup_main_screen;

}

bool ScreenController::isCommandQuit() {
	return isCmdQuit;
}

bool ScreenController::isCommandDone() {
	return isCmdDone;
}


bool ScreenController::isCommandClear() {
	return isCmdClear;
}

void ScreenController::setCommandQuit(bool val) { 
	isCmdQuit = true;
	
	scheduler->schedulerStop();
	scheduler->schedulerDestroy();


	//screens.clear();
	//processList.clear();
	//process_table.clear();

};
void ScreenController::setCommandDone(bool val) { isCmdDone = true; };
void ScreenController::setCommandClear(bool val) { isCmdClear = true; }

std::string ScreenController::getDebugLogs()
{
	
	std::string maxCoresStr = "DEBUG: " + scheduler->getMsgLog();
	return maxCoresStr;
	
}

void ScreenController::startScheduler()
{
	processList.clear();
	screens.clear();
	process_table.clear();
	scheduler->schedulerStartRandomize();
}

void ScreenController::stopScheduler()
{
	scheduler->schedulerStop();
}

std::vector<std::shared_ptr<Process>> ScreenController::getProcessList()
{
	return processList;
}
void ScreenController::setMessageAtScreen(int pid, std::string msgLog)
{
	std::shared_ptr<Screen> screenTemp = screens.at(pid);
	std::shared_ptr<Process> processTemp = processList.at(pid);

	screenTemp->ConsoleCliListFlush();
	screenTemp->ConsoleListPush("Process name: " + screenTemp->getName(), 1, 0);
	screenTemp->ConsoleListPush("ID: "+ std::to_string(pid) + screenTemp->getName(), 1, 0);
	screenTemp->ConsoleListPush("Logs" + std::to_string(pid) + screenTemp->getName(), 1, 0);


	

}
shared_ptr<Process> ScreenController::getProcessAt(int pid)
{
	return process_table[pid];
}
void ScreenController::setProcessAt(int pid, shared_ptr<Process> p)
{
	process_table[pid] = p;
}
void ScreenController::notifyCleanUpLoop()
{
	cleanupCv.notify_one();

}
void ScreenController::cleanUploop()
{
	while (true) {
		std::unique_lock<std::mutex> lock(mtx);

		// Wait for a notification or for the stop signal.
		// The cleanupCv.wait will atomically release the lock and wait,
		// then re-acquire the lock when notified or when the predicate is true.
		cleanupCv.wait(lock, [this] { return stopCleanupThread || scheduler->isThereTerminatedProcesses(); });


		if (stopCleanupThread || screens.empty()) {
			break; // Exit the loop
		}

		// Perform cleanup
		std::erase_if(screens, [](const std::shared_ptr<Screen>& screen) {
			if (screen && screen->shouldBeDestroyed() && !screen->isScreenUsed()) {
				return true;
			}
			return false;
			});

		scheduler->resetIsThereTerminatedProcesses();

	}

}
void ScreenController::debugLogs()
{  
    std::string maxCoresStr = "DEBUG: " + std::to_string(scheduler->getCpu()->getMaxIns()) + " " + scheduler->getCpu()->getAlgoname();
    this->main_screen->ConsoleListPush(maxCoresStr, 1, 0);  
}


std::smatch ScreenController::getMatch()
{
	return ScreenController::match;
}


std::string ScreenController::getInput()
{
	return input;
}

uint64_t ScreenController::getExecDelay()
{
	return this->exec_delay;
}




std::vector<std::shared_ptr<Screen>> ScreenController::getScreens()
{
	return scheduler->getScreenList();
}

