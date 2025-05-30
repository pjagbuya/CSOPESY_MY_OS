#pragma once
#include <vector>
#include <string>
#include <chrono>
#include <ctime>     // For time_t, std::tm, and related functions like localtime
#include <iomanip>   // For std::put_time
#include <sstream>   // For std::stringstream

#include "console.h"
#include "screen.h"
#include "input_manager.h"



#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"


#define DEFAULT_SIZE 10

using stdstr = std::string;

class ScreenController
{
public:
	ScreenController() {
		this->main_screen = std::make_shared<Console>();
		this->backup_main_screen = main_screen;
		this->inputMan = InputMan();
		this->input = "";
		this->action = -999;
		this->sKeys = -1;
		this->screens = {};
		this->isInScreen = false;
		isCmdQuit = false;
		isCmdDone = false;
		isCmdClear = false;

	}


	std::shared_ptr<Screen> findScreen(std::string& name) {

		for (auto& screen_ptr : this->screens) {
			if (screen_ptr->getName() == name) {
				return screen_ptr; // Return the shared_ptr itself
			}
		}
		return nullptr;
	}

	void displayCliAllProcess() {
		this->main_screen->ConsoleCliListFlush();

		for (auto& screen_ptr : this->screens) {
			
			this->main_screen->ConsoleListPush(screen_ptr->getProcessNameAndInfo(), 0 , 1);
			
		}
		
	}


	bool isScreenHere(std::string& name) {
		std::shared_ptr<Screen> tempScreen;
		tempScreen= findScreen(name);

		if (tempScreen == nullptr) {
			return false;
		}
		else {
			return true;
		}
	}

	std::shared_ptr<Screen> createScreen(std::string& name) {
		if (isScreenHere(name)) {
			this->main_screen->ConsoleFlush(0);
			swapScreen(name);
			return findScreen(name);
		}
		else {

			std::shared_ptr<Screen> newScreen = std::make_shared<Screen>(name);
			screens.push_back(newScreen);
			this->main_screen->ConsoleFlush(0);
			swapScreen(name);

			return newScreen;
		}
	}

	void swapScreen(std::string name) {

		std::shared_ptr<Screen> foundScreen = findScreen(name);
		if (foundScreen) {
			this->main_screen = foundScreen;
		}
		else {

		}

		

	}
	std::string executeInput(int& action, bool& isCmdDone, bool& isCmdQuit) {
		switch (action) {
		case 0:	// Initializing
			isCmdDone = true;
			return "initialize" + default_msg;
		case 1:	// exit
			if (isInScreen) {
				this->backToMain();
				isCmdDone = true;
				isInScreen = false;
				return "exiting screen";
			}
			else {
				isCmdDone = true;
				isCmdQuit = true;
				return "Goodbye and Thank you!";
			}

		case 2: // screen -s
			isCmdDone = true;

			if (this->match.size() > 1) {
				std::string tempVal = this->match[1].str();
				if (isScreenHere(tempVal)) {
					return "Swapped to screen " + tempVal;
				}
				else {
					this->createScreen(tempVal);
					isInScreen = true;
					return "Created screen " + tempVal;
				}

			}
			return "Error: screen "+ this->match[1].str() + " not created";
		case 3: // screen -ls
			isCmdDone = true;
			this->displayCliAllProcess();
			return "screen -ls" + default_msg;
		case 4: // scheduler-start
			isCmdDone = true;
			return "scheduler-start" + default_msg;
		case 5: // scheduler-stop
			isCmdDone = true;
			return "scheduler-stop" + default_msg;
		case 6: // report-util
			isCmdDone = true;
			return "report-util" + default_msg;
		case 7: // ping
			isCmdDone = true;
			return "pong";
		case 8: // clear
			isCmdDone = true;
			this->main_screen->ConsoleCliListFlush();
			return "clear" + default_msg;

		case -1: // Invalid command
			isCmdDone = true;
			return "Invalid command. Please try again.";
		default:
			isCmdDone = false;
			return "Unknown action code encountered.";
		}


	}
	void callInputListener() {
		std::string outputListener;

		// Modifies and clears input
		inputMan.Input(input, sKeys, isCmdDone);

		// Modifies this action and isCmdDone
		inputMan.CLI_Comms(this->input, this->action, this->isCmdDone, match);
		outputListener = this->executeInput(this->action, this->isCmdDone, this->isCmdQuit);
		

		this->main_screen->ConsoleFillHeader();
		this->main_screen->ConsoleUpdateInput(input);

		if (this->isCmdDone) {
			this->main_screen->ConsoleListPush(outputListener, 1, 0);
			this->isCmdDone = false;
			this->action = -999;
		}
		this->main_screen->ConsoleFillCliList();

		this->main_screen->ConsoleOut();
	}

	void backToMain() {
		this->main_screen = this->backup_main_screen;
	}

	bool isCommandQuit() {
		return isCmdQuit;
	}


private:
	
	std::shared_ptr<Console> main_screen;
	std::shared_ptr<Console> backup_main_screen;
	std::string default_msg = " command recognized. Doing something";
	std::smatch match;
	InputMan inputMan;
	std::vector<std::shared_ptr<Screen>> screens;
	int sKeys;
	int action;
	std::string input;


	bool isInScreen;
	bool isCmdQuit;
	bool isCmdDone;
	bool isCmdClear;


};

