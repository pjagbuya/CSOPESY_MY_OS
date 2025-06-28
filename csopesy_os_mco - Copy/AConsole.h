#pragma once
#include <string>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <mutex>

#define ARR_SIZE 2000

class AConsole
{
public:
	AConsole(std::string name);
	~AConsole() = default;

	std::string getName();
	virtual void onEnabled() = 0;
	virtual void display() = 0;
	virtual void process() = 0;

	virtual void ParseAction(std::string& input, int& action) = 0;
	virtual void ParseSkeys(int& action, int& sKeys) = 0;
	virtual std::string getProcessNameAndInfo() = 0;


	void MoveCursorTo(COORD coord);
	void ClearCurrentLine();
	void ConsoleFlush(int index);
	void ConsoleFill(int index, int count, std::vector<std::string> list);
	void ConsoleFill(int index, std::string input);
	void ConsoleListPush(std::string input, int mode, int i);
	void ConsoleCliListFlush();



	int getActiveLineCount() const;


	HANDLE h_console;
	std::string name;
	int scroll_offset;
	int header_frame_size;
	int active_line_count;
	std::vector<std::string> cli_list;
	std::mutex mtx;

	std::array<std::string, ARR_SIZE> output_list;

	friend class ScreenController;
};

