#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <ctime>     // For time_t, std::tm, and related functions like localtime
#include <iomanip>   // For std::put_time
#include <sstream>   // For std::stringstream
#include <mutex>
#include <unordered_map>

#include "screen.h"
#include "AConsole.h"
#include "input_manager.h"
#include "Scheduler.h"
#include "MainConsole.h"
#include "process.h"
#include <fstream>

#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"
#define DEFAULT_PROCESS_COUNT 20


#define DEFAULT_SIZE 10



//std::unordered_map<std::string, std::shared_ptr<AConsole>> ConsoleTable;


const std::string MAIN_CONSOLE = "MAIN_CONSOLE";
const std::string MARQUEE_CONSOLE = "MARQUEE_CONSOLE";
const std::string SCHEDULING_CONSOLE = "SCHEDULING_CONSOLE";
const std::string MEMORY_CONSOLE = "MEMORY_CONSOLE";

using stdstr = std::string;
class Scheduler;


class ScreenController
{
public:

	typedef std::unordered_map<std::string, std::shared_ptr<AConsole>> ConsoleTable;


	static ScreenController* getInstance();
	static void initialize();
	static void destroy();
	

	void readConfigFileAndInitializeCPU(const std::string& filename);
	std::shared_ptr<Screen> findScreen(std::string& name);


	void swapScreen(std::shared_ptr<Screen> newScreen);
	void swapScreen(std::string name);



	void callInputListener();

	void backToMain();


	// Creates a screen, swaps if exists by default
	std::shared_ptr<Screen> createScreen(std::string& name);
	void addScreen(std::shared_ptr<Screen> screen);


	std::smatch getMatch();

	HANDLE getConsoleHandle();
	std::string getInput();
	uint64_t getExecDelay();


	void swapToMarquee();
	void doThreadedMarquee();

	bool isCommandQuit();
	bool isCommandDone();
	bool isCommandClear();

	bool isScreenHere(std::string& name);
	

	void setCommandQuit(bool val);
	void setCommandDone(bool val);
	void setCommandClear(bool val);
	std::string getDebugLogs();

	void startScheduler();
	void stopScheduler();
	std::vector<std::shared_ptr<Screen>> getScreens();

	std::vector<std::shared_ptr<Process>> getProcessList();
	std::unordered_map<int, std::shared_ptr<Process>> getProcessTable();

	shared_ptr<Process> getProcessAt(int pid);
	void setProcessAt(int pid, shared_ptr<Process>);


	mutex getMtx();
	void notifyCleanUpLoop();
	void cleanUploop();


	void debugLogs();




	std::queue<Process> getReadyQueue();
	std::queue<Process> getTerminatedProcesses();
	std::queue<Process> getRunningQueue();
	std::shared_ptr<ICommand> generateRandomCommand(std::shared_ptr<Process> p);


private:
	ScreenController();
	~ScreenController();
	ScreenController(ScreenController const&) {};
	ScreenController& operator=(ScreenController const&) {};
	static ScreenController* sharedInstance;


	ConsoleTable consoleTable;
	std::shared_ptr<AConsole> currentConsole;
	std::shared_ptr<AConsole> previousConsole;

	HANDLE consoleHandle;

	std::shared_ptr<AConsole> main_screen;
	std::shared_ptr<AConsole> marquee;
	std::shared_ptr<AConsole> backup_main_screen;

	InputMan inputMan;
	std::vector<std::shared_ptr<Screen>> screens;
	std::vector<std::shared_ptr<Process>> processList;



	int sKeys;
	int action;
	uint64_t exec_delay;

	std::mutex mtx;

	
	std::shared_ptr<Scheduler> scheduler;


	std::string input;
	std::smatch match;

	bool isInScreen;
	bool isCmdQuit;
	bool isCmdDone;
	bool isCmdClear;
	bool isThreadedMarquee;


	atomic<bool> isSchedulerOn;


	std::condition_variable cleanupCv;
	bool stopCleanupThread = false;
	std::thread cleanupThread;



};

