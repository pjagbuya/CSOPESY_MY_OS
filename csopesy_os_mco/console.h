#pragma once

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <vector>

using str = std::string;

using namespace std;

#define ARR_SIZE 100
#define VISIBLE_LINES_MAX 39
#define MAIN_START_FLUSH 34
#define ASCII_FRAME_SIZE 30


#define RESET "\033[0m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"


extern const std::array<std::vector<std::string>, 8> hidr_frames;
extern int hidr_curInd;
class Console
{




public:

	Console()
	{
		this->h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(this->h_console, &csbi);


		this->header_frame_size = 30;
		this->max_width = csbi.srWindow.Right;
		this->max_height = csbi.srWindow.Bottom;
		this->scroll_offset = 0;
		for (int i = 0; i < ARR_SIZE; ++i) {
			output_list[i] = '\0';
		}
	}

	int GetMaxHeight() const { return max_height; }
	int GetMaxWidth() const { return max_width; }
	
	int getActiveLineCount() const {
		return active_line_count;
	}


	vector<string> GetCliList() const{ return this->cli_list;  }

	void setCliList(vector<string>& list) {
		int currSize = this->cli_list.size();
		currSize = this->active_line_count - currSize;
		this->cli_list.clear();
		this->cli_list = list;
		this->active_line_count = currSize + cli_list.size();
		ConsoleFill(currSize, this->cli_list.size(), cli_list);
		
	}
	void setHeaderFrameSize(int new_size) {
		this->header_frame_size = new_size;
	}

	void ConsoleFillCliList() {
		ConsoleFill(this->header_frame_size+3, this->cli_list.size(), this->cli_list);
	}

	
	void MoveCursorTo(COORD coord)
	{
		SetConsoleCursorPosition(this->h_console, coord);

	}

	void ClearCurrentLine() {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		DWORD count_written;

		GetConsoleScreenBufferInfo(this->h_console, &csbi);
		COORD line_start = { 0, csbi.dwCursorPosition.Y };
		FillConsoleOutputCharacter(this->h_console, ' ', csbi.dwSize.X, line_start, &count_written);
	}


	void ConsoleFlush(int index) {
		for (int i = index; i < ARR_SIZE; i++) {

			this->MoveCursorTo({ 0, (short)i });
			this->ClearCurrentLine();
			this->output_list[i] = '\0'; // deleted all space strings
			this->scroll_offset = 0;
		}
	}

	void ConsoleCliListFlush() {
		int currSize = this->cli_list.size();
		currSize = this->active_line_count - currSize;
		this->cli_list.clear();
		ConsoleFlush(currSize);
	}

	virtual void ConsoleFillHeader() {
		
		this->ConsoleFill(0, this->header_frame_size, hidr_frames[hidr_curInd]);
		this->ConsoleFill(this->header_frame_size, str(YELLOW) + "Welcome to CSOPESY Command Line");
		this->ConsoleFill(this->header_frame_size + 1, str(YELLOW) + "'exit' to quit, 'clear' to clear the screen");

	}
	void ConsoleUpdateInput(string& input) {
		this->ConsoleFill(this->header_frame_size + 2, str(YELLOW) + to_string(this->getActiveLineCount()) + " Enter input : " + str(GREEN) + input + str(RESET));
	
	}
	void ConsoleFill(int index, int count, vector<string> list)
	{

		for (int i = 0; i < count; i++) {

			this->output_list[index + i] = list[i];
			if (index + i >= active_line_count) {
				active_line_count = index + i + 1;
			}

		}

	}

	void ConsoleFill(int index, string input) {

		this->output_list[index] = input;
		if (index >= active_line_count) {
			active_line_count = index + 1;
		}

	}

	void ConsoleOut() {
		for (int i = 0; i <= this->active_line_count; i++) {
			this->MoveCursorTo({ 0, (short)i });
			this->ClearCurrentLine();
		}
		if (active_line_count > VISIBLE_LINES_MAX + this->scroll_offset) {
			this->scroll_offset += active_line_count - (VISIBLE_LINES_MAX +this->scroll_offset);
		}

		for (int i = 0; i < this->active_line_count; i++) {
			


			int actual_index = i + this->scroll_offset;
			this->MoveCursorTo({ 0, (short)i });
			cout << output_list[actual_index] << flush;

		}

		// Updates the looping animation frames
		hidr_curInd = (hidr_curInd + 1) % 8;
	}
	void ScrollUp() {
		if (this->scroll_offset > 0) {
			this->scroll_offset--;
			this->ConsoleOut(); // Redraw console after scrolling
		}
	}

	void ScrollDown() {
		// Determine the maximum scrollable offset
		// This ensures we don't scroll past the end of our content
		int max_scrollable_offset = 0;
		for (int i = ARR_SIZE - 1; i >= 0; --i) {
			if (!this->output_list[i].empty()) {
				max_scrollable_offset = i - (this->max_height); // Calculate based on content
				break;
			}
		}
	}

	void ConsoleListPush(string input, int mode, int i) {


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


	void ParseSkeys(int& action, int& sKeys) {

		if (action == 8) {
			this->scroll_offset = 0;
			ConsoleCliListFlush();
			this->ConsoleOut();
			
		}
		else {
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

	}

	protected:
		int max_width;
		int max_height;
		int scroll_offset;
		int active_line_count;
		int header_frame_size;

		array<string, ARR_SIZE> output_list;
		vector<string> header;
		vector<string> cli_list;
		HANDLE h_console;
		COORD buffer_size;
		SMALL_RECT window_rect;

};





