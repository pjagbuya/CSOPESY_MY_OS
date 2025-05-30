#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <vector>
#include <chrono>
#include <ctime>     // For time_t, std::tm, and related functions like localtime
#include <iomanip>   // For std::put_time
#include <sstream>   // For std::stringstream
#include "console.h"

extern vector<string> ascii_art;

using str = std::string;

class Screen:public Console
{

public:
	Screen() = delete;
	Screen(string name) {


		this->h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(this->h_console, &csbi);

		this->max_width = csbi.srWindow.Right;
		this->max_height = csbi.srWindow.Bottom;
		this->scroll_offset = 0;
		for (int i = 0; i < ARR_SIZE; ++i) {
			output_list[i] = '\0';
		}
		



		auto now = std::chrono::system_clock::now();
		time_t now_c = std::chrono::system_clock::to_time_t(now);
		std::tm tm_struct;
		localtime_s(&tm_struct, &now_c);
		std::stringstream ss;

		this->display_buffer = {};
		ss << std::put_time(&tm_struct, "%m/%d/%Y, %I:%M:%S %p");
		this->creation_timestamp = ss.str();
		this->name = name;
		this->process_name = "OMG get in the robot shinji process";
		this->current_line_instruction = 0;
		this->total_lines_instruction = 100;

		this->UpdateDisplayBuffer();
		this->setHeaderFrameSize(ascii_art.size() + this->display_buffer.size());


	}

	void ConsoleFillHeader() override{
		this->ConsoleFill(0, ascii_art.size(), ascii_art);
		this->ConsoleFill(ascii_art.size(), this->display_buffer.size(), this->display_buffer);
		this->ConsoleFill(this->header_frame_size, str(YELLOW) + "Welcome to CSOPESY Command Line");
		this->ConsoleFill(this->header_frame_size + 1, str(YELLOW) + "'exit' to quit, 'clear' to clear the screen");
	}

	string getName() {
		return name;
	}

	void UpdateDisplayBuffer() {
		display_buffer.clear();
		display_buffer.push_back("--- Screen: " + this->name + " ---");
		display_buffer.push_back("Process Name: " + process_name);
		display_buffer.push_back("Instruction: " + std::to_string(current_line_instruction) + " / " + std::to_string(total_lines_instruction));
		display_buffer.push_back("Created: " + creation_timestamp);
		display_buffer.push_back("------------------------");


	}

	string getProcessNameAndInfo() {
		return "Name: " + name + "| Instruction: " + std::to_string(current_line_instruction) + " / " + std::to_string(total_lines_instruction);
	}

private:
	string name;
	string process_name;
	int current_line_instruction;
	int total_lines_instruction;
	string creation_timestamp;
	std::vector<std::string> display_buffer;

};

