#pragma once
#include "AConsole.h"
#include <thread>
#include <mutex>
#include <vector>

using namespace std;

#define MAXMARQUEE_HEIGHT 30
#define MAXMARQUEE_WIDTH 120

class Marquee : public AConsole
{



public:

	Marquee();


	int GetMaxHeight() const;
	int GetMaxWidth() const;
	vector<string> GetCliList() const;



	void updateMarquee();
	void ConsoleFillCliList();
	void moveConsoleScroll();



	void ConsoleUpdateInput(string input);
	void ConsoleFillHeader();

	void startUpdateThread();
	void stopUpdateThread();

	void ConsoleOut();

	void ScrollUp();
	void ScrollDown();



	void onEnabled() override;
	void display() override;
	void process() override;

	void displayNvidiaSmiOutput();

	void ParseSkeys(int& action, int& sKeys);
	void ParseAction(string& input, int& action);

	string getProcessNameAndInfo();


private:




	std::string bouncingText;
	int max_width;
	int max_height;
	int scroll_offset;
	int header_frame_size;

	int origSize;
	int scrollOffset2;
	std::atomic<int> x, y;
	std::atomic<int> dx, dy;

	std::thread t1;
	std::atomic<bool> g_exitFlag;

	COORD buffer_size;
	SMALL_RECT window_rect;

	std::string default_msg = " command recognized. Doing something";



};

