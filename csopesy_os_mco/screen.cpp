#include "screen.h"


#include "screen_controller.h"
#include "process.h"
int Screen::process_count = 0;
Screen::Screen(string name) :AConsole("SCHEDULING_CONSOLE") {

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(ScreenController::getInstance()->getConsoleHandle(), &csbi);
	this->scroll_offset = 0;
	for (int i = 0; i < ARR_SIZE; ++i) {
		output_list[i] = '\0';
	}
	process_output_list = std::make_shared<vector<std::string>>();
	this->process_name = name;

	auto now = std::chrono::system_clock::now();
	time_t now_c = std::chrono::system_clock::to_time_t(now);
	std::tm tm_struct;
	localtime_s(&tm_struct, &now_c);

	
	std::stringstream ss;


	
	this->pid = process_count;
	this->display_buffer = {};
	ss << std::put_time(&tm_struct, "%m/%d/%Y, %I:%M:%S %p");
	this->creation_timestamp = ss.str();
	attachedProcess = std::make_shared<Process>(process_count, this->process_name, process_output_list, creation_timestamp);


	this->name = "BASE_SCREEN";
	this->process_name = name;

	this->isUsingScreen = false;
	this->UpdateDisplayBuffer();
	this->header_frame_size=ascii_art.size() + this->display_buffer.size();
	incProcessCount();
	process_buffer.push_back("");
	process_buffer.push_back("Process name: " + this->getProcessName());
	process_buffer.push_back("ID: " + std::to_string(pid));
	process_buffer.push_back("Logs:");
	debug_msg = "DEFAULT";
	indexInput = this->header_frame_size+2;
	scroll_offset2=0;
}
void Screen::onEnabled() {
}

void Screen::display() {
	this->isUsingScreen = true;

	this->UpdateDisplayBuffer();
	this->ConsoleFillHeader();
	this->ConsoleUpdateInput(ScreenController::getInstance()->getInput());
	this->ConsoleFillCliList();

	ConsoleOut();
}

void Screen::process() {
}


void Screen::ConsoleFillHeader() {
	this->ConsoleFill(0, ascii_art.size(), ascii_art);
	this->ConsoleFill(ascii_art.size(), this->display_buffer.size(), this->display_buffer);
	this->ConsoleFill(this->header_frame_size, str(YELLOW) + "Welcome to CSOPESY Command Line");
	this->ConsoleFill(this->header_frame_size + 1, str(YELLOW) + "'exit' to quit, 'clear' to clear the screen");
}

string Screen::getProcessName() {
	return process_name;
}

void Screen::UpdateDisplayBuffer() {
	display_buffer.clear();
	display_buffer.push_back("--- Process Name: " + process_name + " ---");
	display_buffer.push_back("Instruction: " + std::to_string(attachedProcess->getCommandCounter()) + " / " + std::to_string(attachedProcess->getLinesOfCode()) );
	
	if (attachedProcess->getCPUCoreID() != -1) {
		display_buffer.push_back("(" + creation_timestamp + ") Core: " + std::to_string(attachedProcess->getCPUCoreID()) + " \"" + attachedProcess->getCurrMsgLog() + "\"");

	}
	else {
		display_buffer.push_back("(" + creation_timestamp + ") Core: IDLE" + " \"" + attachedProcess->getCurrMsgLog() + "\"");

	}

	display_buffer.push_back("------------------------");


}

void Screen::displayProcessSmi()
{
	this->cli_list.clear();

	logs_buffer.clear();

	attachedProcess->ConsoleLogPush();

	vector<std::string> temp = attachedProcess->getPrintLog();

	
	for (string& strvar : temp) {
		logs_buffer.push_back(strvar);
	}

	if (attachedProcess->isFinished()) {
		logs_buffer.push_back("");
		logs_buffer.push_back("Finished!");

	}
	else {
		logs_buffer.push_back("");
		logs_buffer.push_back("Current instruction line: " + std::to_string(attachedProcess->getCommandCounter()));
		logs_buffer.push_back("Lines of code: " + std::to_string(attachedProcess->getLinesOfCode()));

	}

	for (string& strvar : process_buffer) {
		ConsoleListPush(strvar, 1, 0);
	}
	debug_msg = "T4";

	for (string& strvar : logs_buffer) {
		ConsoleListPush(strvar, 1, 0);
	}

}

string Screen::getProcessNameAndInfo() {
	if (attachedProcess->isFinished()) {
		return "process: " + process_name + +"\t(" + creation_timestamp + ") " "| Finished\t" + " |Instruction: " + std::to_string(attachedProcess->getCommandCounter()) + " / " + std::to_string(attachedProcess->getLinesOfCode());

	}
	else if (attachedProcess->getCommandCounter() == 0 || attachedProcess->getCPUCoreID() == -1) {
		return "process: " + process_name + +"\t(" + creation_timestamp + ") " "| IDLE\t"+ " |Instruction: " + std::to_string(attachedProcess->getCommandCounter()) + " / " + std::to_string(attachedProcess->getLinesOfCode());

	}
	else {
		return "process: " + process_name + +"\t(" + creation_timestamp + ") " "| Core: " + std::to_string(attachedProcess->getCPUCoreID()) + "\t |Instruction: " + std::to_string(attachedProcess->getCommandCounter()) + " / " + std::to_string(attachedProcess->getLinesOfCode());

	}
}


void Screen::ConsoleUpdateInput(string input) {
	//this->ConsoleFill(this->indexInput, str(YELLOW) + "root:>\\ " + "DEBUG: "+ debug_msg + " | " + str(GREEN) + input + str(RESET));
	this->ConsoleFill(this->indexInput, str(YELLOW) + "root:>\\ " + str(GREEN) + input + str(RESET));

}
void Screen::ConsoleFillCliList() {
	debug_msg = "Filled Cli_list " + std::to_string(cli_list.size());

	int maxVisibility = VISIBLE_LINES_MAX;
	int header_space = this->header_frame_size + 3 + this->cli_list.size()+ process_output_list->size();
	
	// Total output of both cli_list and process_output_list exceed visibility on screen
	// Just compress other print commands into repeats x times
	if ( header_space > VISIBLE_LINES_MAX || this->header_frame_size + 3 + process_output_list->size() > VISIBLE_LINES_MAX) {

		int diff = process_output_list->size() + this->header_frame_size + 3 - VISIBLE_LINES_MAX;
		ConsoleFill(this->header_frame_size + 3, VISIBLE_LINES_MAX-(this->header_frame_size + 3), *(this->process_output_list));
		ConsoleFill(VISIBLE_LINES_MAX, "Repeats " + std::to_string(diff) + " more times");

	}
	else {

		ConsoleFill(this->header_frame_size + 3, cli_list.size(), this->cli_list);
		ConsoleFill(this->header_frame_size + 3+ cli_list.size(), process_output_list->size(), *(this->process_output_list));

	}

}
void Screen::ConsoleOut() {
	for (int i = 0; i <= this->active_line_count; i++) {
		this->MoveCursorTo({ 0, (short)i });
		this->ClearCurrentLine();
	}
	//if (active_line_count > VISIBLE_LINES_MAX + this->scroll_offset) {
	//	this->scroll_offset += active_line_count - (VISIBLE_LINES_MAX + this->scroll_offset);
	//}

	for (int i = 0; i <= VISIBLE_LINES_MAX; i++) {

		int actual_index = i + this->scroll_offset;
		this->MoveCursorTo({ 0, (short)i });
		cout << output_list[actual_index] << flush;

	}


}

void Screen::ParseAction(string& input, int& action)
{

	debug_msg = "parsing action";

	switch (action) {
	case 12:	// Displaying process-smi
		debug_msg = "detected";

		displayProcessSmi();
		debug_msg = "finished smi";
		break;
	case 1:	// exit
		this->isUsingScreen = false;
		ScreenController::getInstance()->backToMain();
		break;

	case 7: // ping

		ConsoleListPush("pong", 1, 0);
		break;

	case 8: // clear

		this->ConsoleCliListFlush();
		(*(this->process_output_list)).clear();
		break;

	case -1: // Invalid command

		ConsoleListPush("Invalid command. Please try again.", 1, 0);
		break;

	default:
		ConsoleListPush("Invalid command. Please try again.", 1, 0);
		break;


	}





}

void Screen::ScrollUp() {
	if (this->scroll_offset > 0) {
		this->scroll_offset--;
		this->ConsoleOut(); // Redraw MainConsole after scrolling
	}
}

void Screen::ScrollDown() {
	// Determine the maximum scrollable offset
	// This ensures we don't scroll past the end of our content
	int max_scrollable_offset = 0;
	for (int i = ARR_SIZE - 1; i >= 0; --i) {
		if (!this->output_list[i].empty()) {
			max_scrollable_offset = i - (this->active_line_count); // Calculate based on content
			break;
		}
	}
}
void Screen::ParseSkeys(int& action, int& sKeys) {

	if (action == 10) {
		this->scroll_offset = 0;
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
unsigned short Screen::getPid() const
{
	return this->pid;
}

std::shared_ptr<Process> Screen::getAttachedProcess()
{
	return attachedProcess;
}

int Screen::getProcessCount()
{
	return Screen::process_count;

}
void Screen::setProcessCount(int num) {
	Screen::process_count = num;
}

void Screen::incProcessCount() {
	Screen::process_count++;
}

void Screen::decProcessCount() {
	Screen::process_count--;
}

bool Screen::shouldBeDestroyed() const
{
	
	return attachedProcess && attachedProcess->isFinished();
	
}

bool Screen::isScreenUsed() const
{
	return isUsingScreen;
}
