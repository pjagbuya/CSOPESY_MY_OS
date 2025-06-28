#pragma once

#include <iostream>
#include <conio.h>

#include <vector>
#include <mutex>
#include "AConsole.h"

using str = std::string;

using namespace std;

#define VISIBLE_LINES_MAX 40
#define MAIN_START_FLUSH 33
#define ASCII_FRAME_SIZE 30
#define DEFAULT_HEADER_SIZE 30


#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"


extern const std::array<std::vector<std::string>, 8> hidr_frames;
extern int hidr_curInd;
class MainConsole : public AConsole
{




public:

	MainConsole();


	int GetMaxHeight() const;
	int GetMaxWidth() const;
	vector<string> GetCliList() const;




	void ConsoleFillCliList();



	void ConsoleUpdateInput(string input);
	void getInDaRobotShinji();
	void startAnimation();


	void ConsoleFillHeader();


	void ConsoleOut();
	void ScrollUp();

	void ScrollDown();

	void displayCliAllProcess();


	void onEnabled() override;
	void display() override;
	void process() override;

	void displayNvidiaSmiOutput();

	void ParseSkeys(int& action, int& sKeys);
	void ParseAction(string& input, int& action);
	bool checkAnimationGo();
	void setAnimationGo(bool go);

	string getProcessNameAndInfo();
	void saveReportUtil();

private:
	std::string get_current_timestamp();
	void drawNvidiaLine(int mode);
	int inputCountPrompt;
	std::string limitStringEnd(string input, int limit);
	std::string limitStringStart(std::string input, int limit);

	int max_width;
	int max_height;
	int scroll_offset;
	int header_frame_size;
	int outputLimit;
	int origSize;
	int scrollOffset2;

	std::mutex cout_mutex;
	bool isConsoleEnabled;



	bool hitCommandDone;
	std::atomic<bool> isAnimationGo;

	COORD buffer_size;
	SMALL_RECT window_rect;

	std::string default_msg = " command recognized. Doing something";

	


};





