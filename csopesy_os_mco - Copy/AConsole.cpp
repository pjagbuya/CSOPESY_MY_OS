#include "AConsole.h"
#include "screen_controller.h"

AConsole::AConsole(std::string name)
	:name(name),
	 scroll_offset(0),
	active_line_count(0)
{
	h_console = ScreenController::getInstance()->getConsoleHandle();
}

std::string AConsole::getName()
{
	return name;
}


void AConsole::MoveCursorTo(COORD coord)
{
	SetConsoleCursorPosition(h_console, coord);

}

void AConsole::ClearCurrentLine() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count_written;

	GetConsoleScreenBufferInfo(h_console, &csbi);
	COORD line_start = { 0, csbi.dwCursorPosition.Y };
	FillConsoleOutputCharacter(h_console, ' ', csbi.dwSize.X, line_start, &count_written);
}


void AConsole::ConsoleFlush(int index) {
	for (int i = index; i < ARR_SIZE; i++) {

		this->MoveCursorTo({ 0, (short)i });
		this->ClearCurrentLine();
		this->output_list[i] = '\0'; // deleted all space strings
	}


}


void AConsole::ConsoleFill(int index, int count, std::vector<std::string> list)
{

	
	std::lock_guard<std::mutex> lock(mtx);



	for (int i = 0; i < count; i++) {
		if (i >= list.size() || index+i >= output_list.size()) {
			break;
		}

		this->output_list[index + i] = list[i];
		if (index + i >= active_line_count ) {
			active_line_count += 1;

		}

	}

}

void AConsole::ConsoleFill(int index, std::string input) {
	std::lock_guard<std::mutex> lock(mtx);

	this->output_list[index] = input;
	if (index >= active_line_count ) {
		active_line_count = index + 1;
	}

}


void AConsole::ConsoleListPush(string input, int mode, int i) {

	std::lock_guard<std::mutex> lock(mtx);

	if (this->cli_list.size() == 0) {
		this->cli_list.emplace_back(input);

	}
	else {
		if (mode == 0) {
			if (this->cli_list.size() > i) {
				this->cli_list[i] = input;
			}
			else {
				this->cli_list.emplace_back(input);

			}
		}
		else {

			this->cli_list.emplace_back(input);

		}
	}

}

int AConsole::getActiveLineCount() const
{
	return active_line_count;
}



void AConsole::ConsoleCliListFlush() {

	ConsoleFlush(MAIN_START_FLUSH);
	//int currSize = this->cli_list.size();
	//currSize = this->active_line_count - currSize;
	this->scroll_offset = 0;
	this->cli_list.clear();
	active_line_count = MAIN_START_FLUSH;
	this->header_frame_size = MAIN_START_FLUSH-1;

}