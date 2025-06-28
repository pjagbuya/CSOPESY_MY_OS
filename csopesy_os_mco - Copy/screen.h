#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <chrono>
#include <ctime>     // For time_t, std::tm, and related functions like localtime
#include <iomanip>   // For std::put_time
#include <sstream>   // For std::stringstream
#include <random>

#include "AConsole.h"
#include "process.h"


extern std::vector<std::string> ascii_art;


#define VISIBLE_LINES_MAX 40
using str = std::string;


class Process;
class Screen:public AConsole
{

public:
	Screen() = delete;
	Screen(std::string name);

	std::string getProcessName();

	void UpdateDisplayBuffer();
	void displayProcessSmi();

	void onEnabled() override;
	void display() override;
	void process() override;

	void ConsoleUpdateInput(std::string input);
	void ConsoleFillCliList();
	void ConsoleOut();
	void ScrollUp();
	void ScrollDown();
	void ParseSkeys(int& action, int& sKeys);
	void ParseAction(std::string& input, int& action) ;
	std::string getProcessNameAndInfo() ;
	unsigned short getPid() const;

	
	std::shared_ptr<Process> getAttachedProcess();

	static int getProcessCount();
	static void setProcessCount(int count);
	static void incProcessCount();
	static void decProcessCount();

	bool shouldBeDestroyed() const;
	bool isScreenUsed() const;

private:
	std::string name;
	std::string process_name;


	int pid;
	int scroll_offset2;
	int indexInput;
	bool isUsingScreen;
	std::string creation_timestamp;
	static int process_count;


	std::vector<std::string> display_buffer;
	std::vector<std::string> process_buffer;
	std::vector<std::string> logs_buffer;


	std::shared_ptr<std::vector<std::string>> process_output_list;

	std::shared_ptr<Process> attachedProcess;
	std::string default_msg = " command recognized. Doing something";
	std::string debug_msg = "";



	void ConsoleFillHeader();

};

