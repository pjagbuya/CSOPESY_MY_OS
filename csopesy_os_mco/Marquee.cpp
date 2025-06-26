#include "Marquee.h"

#include "screen_controller.h"


Marquee::Marquee()
	:AConsole("MARQUEE"), x(0), y(3), dx(1), dy(1)
{

	this->h_console = ScreenController::getInstance()->getConsoleHandle();
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(this->h_console, &csbi);


	this->max_width = csbi.srWindow.Right;
	this->max_height = csbi.srWindow.Bottom;
	this->scroll_offset = 0;
	this->scrollOffset2 = 0;
	this->header_frame_size = MAXMARQUEE_HEIGHT;
	this->bouncingText = "Lorem Ipsum";
	origSize = 0;

	for (int i = 0; i < ARR_SIZE; ++i) {
		output_list[i] = '\0';

	}
	this->g_exitFlag = false;

}
void Marquee::onEnabled() {

	startUpdateThread();
}



void Marquee::moveConsoleScroll()
{
	if (scroll_offset <= 32)
	{
		SetConsoleCursorPosition(this->h_console, { 0, (short)(0) });
	}
	else
	{
		SetConsoleCursorPosition(this->h_console, { 0, (short)(scroll_offset + VISIBLE_LINES_MAX) });
	}
}
void Marquee::display() {


	this->ConsoleFillHeader();
	this->ConsoleFillCliList();

	ConsoleOut();

}

void Marquee::process() {
}



int Marquee::GetMaxHeight() const { return max_height; }
int Marquee::GetMaxWidth() const { return max_width; }



vector<string> Marquee::GetCliList() const { return this->cli_list; }



void Marquee::ConsoleFillCliList() {



	ConsoleFill(this->header_frame_size + 2, this->cli_list.size(), this->cli_list);


}
void Marquee::updateMarquee() {
	while (!g_exitFlag) {
		x += dx;
		y += dy;

		// Bounce at edges
		if (x <= 0 || x + bouncingText.size() >= MAXMARQUEE_WIDTH) 
			dx = dx * -1;
		if (y <= 3 || y >= MAXMARQUEE_HEIGHT - 2) 
			dy = dy * -1;


		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

void Marquee::startUpdateThread() {  
	this->g_exitFlag = false;
    this->t1 = std::thread(&Marquee::updateMarquee, this);  
}

void Marquee::stopUpdateThread() {
	if (t1.joinable())
		t1.join();      // Wait for thread to exit
}

void Marquee::ConsoleFillHeader() {


	string temp = "";

	for (int i = 3; i < y; ++i)
		ConsoleFill(i, "");

	for (int i = 0; i < x; ++i)
		temp += " ";
	ConsoleFill(y-1, "");
	ConsoleFill(y, "");
	ConsoleFill(y, temp + "Lorem Ipsum");
	ConsoleFill(y + 1, "");


	MoveCursorTo({ 0, (short)this->header_frame_size });
	this->ConsoleFill(this->header_frame_size, str(YELLOW) + "Enter input : " + str(GREEN) + ScreenController::getInstance()->getInput() + str(RESET));



}
void Marquee::ConsoleUpdateInput(string input) {


	std::cout << "Prompt: " << ScreenController::getInstance()->getInput() << std::flush;




}


void Marquee::ConsoleOut() {


	//int start = this->active_line_count - VISIBLE_LINES_MAX > 0 ? this->active_line_count - VISIBLE_LINES_MAX : 0;
	for (int i = 3; i <= VISIBLE_LINES_MAX + scroll_offset; i++) {
		this->MoveCursorTo({ 0, (short)i });
		this->ClearCurrentLine();
	}


	if (active_line_count < VISIBLE_LINES_MAX)
	{
		scroll_offset = 0;
	}


	if (active_line_count > VISIBLE_LINES_MAX + this->scroll_offset) {
		this->scroll_offset += active_line_count - (VISIBLE_LINES_MAX + this->scroll_offset);
	}

	for (int i = 0; i < this->active_line_count; i++) {
		int actual_index = i + this->scroll_offset;
		if (actual_index < active_line_count)
		{
			this->MoveCursorTo({ 0, (short)i });
			cout << output_list[actual_index] << flush;
		}

	}


	hidr_curInd = (hidr_curInd + 1) % 8;
}
void Marquee::ScrollUp() {
	if (this->scroll_offset > 0) {
		ClearCurrentLine();
		this->scroll_offset--;
		this->active_line_count--;
		if (scrollOffset2 < active_line_count)
			scrollOffset2++;
		moveConsoleScroll();
	}

}

void Marquee::ScrollDown() {
	// Determine the maximum scrollable offset
	// This ensures we don't scroll past the end of our content

	if (this->scroll_offset < cli_list.size() - 6) {
		this->scroll_offset++;
		if (scrollOffset2 > 0)
			scrollOffset2--;
		moveConsoleScroll();
	}
}




void Marquee::ParseSkeys(int& action, int& sKeys) {


	switch (sKeys) {
	case 72:
		ScrollUp();
		break;
	case 80:
		ScrollDown();
		break;
	default:
		break;
	}


}


void Marquee::ParseAction(string& input, int& action)
{

	bool main_displyed = false;
	int origSize = cli_list.size();
	switch (action) {
	case 0:	// Initializing
		ConsoleListPush("initialize" + default_msg, 1, 0);
		main_displyed = true;
		break;

	case 1:	// exit
		ConsoleFlush(0);
		this->g_exitFlag = true;
		stopUpdateThread();
		ScreenController::getInstance()->backToMain();
		break;

	case 2: // screen -s
		ConsoleListPush("screen -s" + default_msg, 1, 0);
		break;


	case 3: // screen -ls
		ConsoleListPush("scheduler-start" + default_msg, 1, 0);

		break;

	case 4: // scheduler-start
		ConsoleListPush("scheduler-start" + default_msg, 1, 0);
		main_displyed = true;

		break;

	case 5: // scheduler-stop
		ConsoleListPush("scheduler-stop" + default_msg, 1, 0);
		main_displyed = true;

		break;

	case 6: // report-util
		ConsoleListPush("report-util" + default_msg, 1, 0);
		main_displyed = true;

		break;


	case 8: // clear
		this->ConsoleCliListFlush();
		main_displyed = true;
		break;

	case -1: // Invalid command

		ConsoleListPush("Invalid command. Please try again.", 1, 0);
		break;
	default:
		ConsoleListPush("Unknown action code encountered.", 1, 0);
		main_displyed = true;
		break;

	}


}
string Marquee::getProcessNameAndInfo() { return ""; }





