#include "MainConsole.h"

#include "screen_controller.h"


MainConsole::MainConsole()
	:AConsole("MAIN_CONSOLE"), isConsoleEnabled(false)
{

	this->h_console = ScreenController::getInstance()->getConsoleHandle();
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(this->h_console, &csbi);

	this->header_frame_size = DEFAULT_HEADER_SIZE;
	this->max_width = csbi.srWindow.Right;
	this->max_height = csbi.srWindow.Bottom;
	this->scroll_offset = 0;
	this->scrollOffset2 = 0;
	hitCommandDone = false;
	isAnimationGo = true;
	std::thread t1(&MainConsole::getInDaRobotShinji, this);
	t1.detach();
	origSize = 0;
	int outputLimit = 0;
	inputCountPrompt = 1;

	for (int i = 0; i < ARR_SIZE; ++i) {
		output_list[i] = '\0';

	}
}
void MainConsole::onEnabled() {
}

void MainConsole::getInDaRobotShinji() {

	while (checkAnimationGo()) {
		this->ConsoleFill(0, DEFAULT_HEADER_SIZE, hidr_frames[hidr_curInd]);
		hidr_curInd = (hidr_curInd + 1) % 8;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}

}


void MainConsole::display() {
	if (!checkAnimationGo()) {
		setAnimationGo(true);
		std::thread t1(&MainConsole::getInDaRobotShinji, this);
		t1.detach();
	}


	this->ConsoleFillHeader();


	if (cli_list.size()>39)
	{
		this->ConsoleFillCliList();
		this->ConsoleUpdateInput(ScreenController::getInstance()->getInput());
	}
	else{
		this->ConsoleUpdateInput(ScreenController::getInstance()->getInput());
		this->ConsoleFillCliList();
	}
	ConsoleOut();
}

void MainConsole::process() {
}



int MainConsole::GetMaxHeight() const { return max_height; }
int MainConsole::GetMaxWidth() const { return max_width; }



vector<string> MainConsole::GetCliList() const { return this->cli_list; }


void MainConsole::ConsoleFillCliList() {


	if (cli_list.size()>39)
	{

		ConsoleFill(this->header_frame_size + 3, this->cli_list.size()-scrollOffset2, this->cli_list);

	}
	else
	{
		ConsoleFill(this->header_frame_size + 3, this->cli_list.size(), this->cli_list);

	}

}

void MainConsole::ConsoleFillHeader() {

	this->ConsoleFill(DEFAULT_HEADER_SIZE, str(YELLOW) + "Welcome to CSOPESY Command Line");
	this->ConsoleFill(DEFAULT_HEADER_SIZE + 1, str(YELLOW) + "'exit' to quit, 'clear' to clear the screen");

}
void MainConsole::ConsoleUpdateInput(string input) {

	if (this->cli_list.size() > 39) {

		this->ConsoleFill(this->header_frame_size + 3 + cli_list.size() - scrollOffset2, str(YELLOW) + "Enter input : " + str(GREEN) + input + str(RESET));
		
	}
	else
	{

		this->ConsoleFill(this->header_frame_size + 2, str(YELLOW) + "Enter input : " + str(GREEN) + input + str(RESET));

	}

}


void MainConsole::ConsoleOut() {


	//int start = this->active_line_count - VISIBLE_LINES_MAX > 0 ? this->active_line_count - VISIBLE_LINES_MAX : 0;
	for (int i = 0; i <= VISIBLE_LINES_MAX; i++) {
		this->MoveCursorTo({ 0, (short)i });
		this->ClearCurrentLine();
	}




	if (active_line_count < VISIBLE_LINES_MAX)
	{
		scroll_offset = 0;
	}


	if (active_line_count > VISIBLE_LINES_MAX) {
		this->scroll_offset = active_line_count - (VISIBLE_LINES_MAX);
	}

	for (int i = 0; i < VISIBLE_LINES_MAX; i++) {
		int actual_index = i + this->scroll_offset;

		this->MoveCursorTo({ 0, (short)i });

		cout << output_list[actual_index] << flush;
		

	}


}
void MainConsole::ScrollUp() {
	if (this->scroll_offset > 0) {
		ClearCurrentLine();
		this->scroll_offset--;
		this->active_line_count--;
		scrollOffset2++;
	}


}

void MainConsole::ScrollDown() {
	// Determine the maximum scrollable offset
	// This ensures we don't scroll past the end of our content
	
	if (this->scroll_offset < cli_list.size()-6) {
		this->scroll_offset++;
		if (scrollOffset2>0)
			scrollOffset2--;
	}
}

std::string MainConsole::limitStringEnd(string input, int limit)
{

	if (input.empty())
	{
		return "";
	}
	if (input.size() < limit)
	{
		return input;
	}

	std::string truncated_string = input.substr(0, limit - 4);
	truncated_string += " ...";
	return truncated_string;
}

std::string MainConsole::limitStringStart(std::string input, int limit)
{

	if (input.empty()) {
		return "";
	}

	if (input.size() <= limit) {
		return input;
	}


	std::string truncated_string = "...";
	truncated_string += input.substr(input.size() - (limit - 3));

	return truncated_string;
}

void MainConsole::drawNvidiaLine(int mode)
{
	std::stringstream ss; // Use a stringstream for each line
	ss.str("");

	switch (mode)
	{
	case 0:
		ss << "+"+ std::string(77, '-') +"+";
		ConsoleListPush(ss.str(), 1, 0);
		break;
	case 1:
		ss << "|" + std::string(77, '-') + "|";
		ConsoleListPush(ss.str(), 1, 0);
		break;
	case 2:
		ss << "|" + std::string(31, '=') + "+" + std::string(22, '=') + "+" + ::string(22, '=') + "|";
		ConsoleListPush(ss.str(), 1, 0);
		break;
	case 3:
		ss << "+" + std::string(31, '-') + "+" + std::string(22, '-') + "+" + ::string(22, '-') + "+";
		ConsoleListPush(ss.str(), 1, 0);
		break;
	case 4:
		ss << "|" + std::string(77, '=') + "|";
		ConsoleListPush(ss.str(), 1, 0);
		break;
	}


}

void MainConsole::displayNvidiaSmiOutput() {
	// Clear previous content if this is a full redraw
	std::stringstream ss; // Use a stringstream for each line

	

	// --- Header Section ---
	ss.str("");
	ss << get_current_timestamp();
	ConsoleListPush("", 1, 0);
	ConsoleListPush(ss.str(), 1, 0);


	// --- NVIDIA-SMI Header ---

	drawNvidiaLine(0);
	ss.str("");

	// Char count except nulls from const chars: 19 + 11 + 22 + 6 + 18 + 3 + 1 = 80
	//
	ss << "| NVIDIA-SMI 551.86" << std::setw(9) 
		<< "" << "Driver Version: 551.86"
		<< std::setw(8) << "" << "CUDA Version: 12.4" << std::setw(3) << "|";
	ConsoleListPush(ss.str(), 1, 0);

	// --- GPU Info Header ---
	drawNvidiaLine(1);
	ss.str("");
	ss << std::left
		<< std::setw(23) << "| GPU  Name"
		<< std::setw(7) << "TCC/Coom |"
		<< std::setw(15) << " Bus-Id"
		<< std::setw(7) << "Disp.A" << "| "
		<< std::setw(9) << "Volatile"
		<< std::setw(8) << "Uncorr."
		<< std::setw(4) << "ECC" << "|";
	ConsoleListPush(ss.str(), 1, 0);

	// Values of above properties
	ss.str("");
	ss << std::left << "| "
		<< std::setw(5) << "Fan"
		<< std::setw(6) << "Temp"
		<< std::setw(6) << "Perf"
		<< std::setw(13) << "Pwr:Usage/Cap" << "|"
		<< std::setw(9) << ""
		<< std::setw(13) << "Memory-Usage" << "| "

		<< std::setw(9) << "GPU-Util"
		<< std::right
		<< std::setw(11) << "Compute M." << " |";
	ConsoleListPush(ss.str(), 1, 0);

	ss.str("");
	ss << std::left << "| "
		<< std::setw(30) << " " << "|"
		<< std::setw(22) << " " << "| "
		<< std::setw(9) << " "
		<< std::right
		<< std::setw(11) << "MIG M." << " |";
	ConsoleListPush(ss.str(), 1, 0);

	drawNvidiaLine(2);
	// --- GPU Info Data (NVIDIA GeForce GTX 1080) ---
	ss.str("");
	ss << std::right << "| "
		<< std::setw(3) << "0" << " "
		<< std::setw(19) << limitStringEnd("NVIDIA GeForce GTX 1080", 18) << " "
		<< std::left
		<< std::setw(6) << "WDDM" << "| "
		<< std::right
		<< std::setw(16) << "00000000:26:00.0" << " "
		<< std::setw(3) << "On" << " | "
		<< std::setw(7) << " " << " "
		<< std::setw(12) << "N/A" << " |";
	ConsoleListPush(ss.str(), 1, 0);


	ss.str("");
	ss << std::left << "| "
		<< std::setw(3) << "28%" << "   "
		<< std::setw(5) << "37C" << "  "
		<< std::setw(4) << "P8" << " "
		<< std::right
		<< std::setw(4) << "13W" << " /"
		<< std::setw(5) << "N/A" << " | "
		<< std::setw(9) << "701MiB" << " /"
		<< std::setw(9) << "8192MiB" << " | "
		<< std::setw(7) << "3%" << " "
		<< std::setw(12) << "Default" << " |";
	ConsoleListPush(ss.str(), 1, 0);



	ss.str("");
	ss << std::left << "| "
		<< std::setw(29) << " " << " | "
		<< std::setw(20) << " " << " | "
		<< std::right
		<< std::setw(7) << " " << " "
		<< std::setw(12) << "N/A" << " |";
	ConsoleListPush(ss.str(), 1, 0);

	drawNvidiaLine(3);


	ss.str("");
	ConsoleListPush(ss.str(), 1, 0);

	// --- Processes Header ---
	drawNvidiaLine(0);
	ss.str("");
	ss << "| " << std::left
		<< std::setw(75) << "Processes:" << " |";
	ConsoleListPush(ss.str(), 1, 0);

	ss.str("");
	ss << std::left << "|  "
		<< std::setw(6) << "GPU"
		<< std::setw(5) << "GI"
		<< std::setw(5) << "CI"
		<< std::right
		<< std::setw(8) << "PID" << "  "
		<< std::setw(5) << "Type" << "   "
		<< std::left
		<< std::setw(28) << "Process name" << "  "
		<< std::setw(11) << "GPU Memory" << "|";
	ConsoleListPush(ss.str(), 1, 0);
	ss.str("");
	ss << std::left << "|  "
		<< std::right
		<< std::setw(64) << " "
		<< std::left
		<< std::setw(11) << "Usage" << "|";;
	ConsoleListPush(ss.str(), 1, 0);

	// --- Processes Data ---
	drawNvidiaLine(4);
	for (const auto& s : ScreenController::getInstance()->getScreens()) {

		ss.str("");
		ss << std::left << "|  "
			<< std::right
			<< std::setw(3) << "0" << "   "
			<< std::setw(3) << "GI" << "  "
			<< std::setw(3) << "CI" << " "
			<< std::right
			<< std::setw(9) << s->getPid() << "   "
			<< std::setw(4) << "C+G" << "   "
			<< std::left
			<< std::setw(28) << limitStringStart(s->getProcessName(), 28) << "    "
			<< std::setw(9) << "N/A" << "|";
		ConsoleListPush(ss.str(), 1, 0);
	}
	drawNvidiaLine(0);
}



void MainConsole::ParseSkeys(int& action, int& sKeys) {

	//if (action == 10) {
	//	this->scroll_offset = 0;
	//	this->ConsoleOut();

	//}
	//else {
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

	//}

}


void MainConsole::ParseAction(string& input, int& action)
{

	bool main_displyed = false;
	int origSize = cli_list.size();

	if (!isConsoleEnabled) {
		if (action == 0)
		{
			isConsoleEnabled = true;
		}
		else {
			action = -2;

		}
	}
	switch (action) {
		case 0:	// Initializing
			ConsoleListPush("initialize" + default_msg, 1, 0);
			main_displyed = true;

			break;

		case 1:	// exit
			cout << "Goodbye and Thank you!" << endl;



			ScreenController::getInstance()->setCommandQuit(true);
			main_displyed = true;
			break;


		case 2: // screen -s
			isAnimationGo = false;
			if (ScreenController::getInstance()->getMatch().size() > 1) {
				std::string tempVal = ScreenController::getInstance()->getMatch()[1].str();
				std::shared_ptr<Screen> checkScreen = ScreenController::getInstance()->createScreen(tempVal);

				
				if (ScreenController::getInstance()->isScreenHere(tempVal)) {
					//ConsoleListPush("Swapped to screen " + tempVal, 0, 1);
					main_displyed = true;

				}
				else if (checkScreen){
					//ConsoleListPush("Created screen " + tempVal, 0, 1);
					main_displyed = true;

				}
				else
				{
					ConsoleListPush("Error: screen " + tempVal + " not created", 1, 0);
					main_displyed = true;

				}

			}
			else
			{
				ConsoleListPush("Error: Please provide a name for the screen", 1, 0);
				main_displyed = true;

			}

			break;


		case 3: // screen -ls
			ConsoleCliListFlush();
			displayCliAllProcess();

			break;

		case 4: // scheduler-start
			ScreenController::getInstance()->startScheduler();

			break;

		case 5: // scheduler-stop
			ScreenController::getInstance()->stopScheduler();

			break;

		case 6: // report-util
			saveReportUtil();
			ConsoleListPush("SUCCESS: Saved file to csopesy-log.txt" + default_msg, 1, 0);
			main_displyed = true;

			break;

		case 7: // marquee
			isAnimationGo = false;

			ScreenController::getInstance()->swapToMarquee();
			ConsoleFlush(0);
			MoveCursorTo({ 0, 0 });
			std::cout << "*************************************" << endl << std::flush;
			std::cout << "* Displaying a marquee Console      *" << endl << std::flush;
			std::cout << "**********************************************************************************************************************" << std::flush;

			main_displyed = true;

			break;

		case 8: // clear

			this->ConsoleCliListFlush();
			inputCountPrompt = 1;
			main_displyed = true;
			break;

		case 9: //nvidia-smi
			ConsoleCliListFlush();
			this->displayNvidiaSmiOutput();
			main_displyed = true;
			break;
		case 10: // screen -r
			isAnimationGo = false;

			if (ScreenController::getInstance()->getMatch().size() > 1) {
				std::string tempVal = ScreenController::getInstance()->getMatch()[1].str();
				std::shared_ptr<Screen> checkScreen = ScreenController::getInstance()->findScreen(tempVal);

				if (checkScreen) {
					ScreenController::getInstance()->swapScreen(tempVal);
				}
				else
				{
					ConsoleListPush("Error: process " + tempVal + " not found", 1, 0);
					main_displyed = true;

				}

			}
			else
			{
				ConsoleListPush("Error: Please provide a name for the screen", 1, 0);
				main_displyed = true;

			}

			break;
		case 11: // d for debug mode:
			ScreenController::getInstance()->debugLogs();
			break;
		case -1: // Invalid command

			ConsoleListPush("Invalid command. Please try again.",1, 0);
			break;
		case -2:
			ConsoleListPush("ERROR: Please type and enter 'initialize' first", 1, 0);
			break;

		default:
			ConsoleListPush("ERROR: Unknown action code encountered.", 1, 0);
			main_displyed = true;
			break;

	}

	if (main_displyed)
	{
		hitCommandDone = true;

	}
	else
	{
		hitCommandDone = false;
	}


}
bool MainConsole::checkAnimationGo()
{
	std::lock_guard<std::mutex> lock(mtx);
	return isAnimationGo;
}
void MainConsole::setAnimationGo(bool go)
{
	std::lock_guard<std::mutex> lock(mtx);
	isAnimationGo = go;	

}

string MainConsole::getProcessNameAndInfo() {
	return "";
}
void MainConsole::saveReportUtil()
{






	vector<string> terminated_list;
	vector<string> running_list;
	vector<string> ready_list;


	int runningCores = 0;
	for (auto& screen_ptr : ScreenController::getInstance()->getScreens()) {

		Process& p = *(screen_ptr->getAttachedProcess());
		if (p.getState() == ProcessState::FINISHED) {
			terminated_list.push_back(p.getProcessNameAndInfo());
		}
		else if (p.getState() == ProcessState::RUNNING) {
			running_list.push_back(p.getProcessNameAndInfo());
			runningCores++;

		}

	}


	double core_utilization_percentage = static_cast<double>(runningCores) / CPU::getMaxCores() * 100.0; // Example calculated value
	int core_utilization = static_cast<int>(core_utilization_percentage);



	std::ofstream reportFile("csopesy-log.txt");
	if (!reportFile.is_open()) {
		std::cerr << "Failed to open process_report.txt for writing.\n";
		return;
	}

	reportFile << "CPU utilization: " + std::to_string(core_utilization) + "%";
	reportFile << "Cores used: " + std::to_string(runningCores);
	reportFile << "Cores available: " + to_string(CPU::getMaxCores() - runningCores);

	reportFile << "";
	// Write report
	reportFile << "================= PROCESS REPORT =================\n\n";

	reportFile << "Running Processes:\n";
	for (const auto& s : running_list) {
		reportFile << "  - " << s << "\n";
	}

	reportFile << "\nFinished Processes:\n";
	for (const auto& s : terminated_list) {
		reportFile << "  - " << s << "\n";
	}



	reportFile << "\n==================================================\n";
	reportFile.close();

}

void MainConsole::displayCliAllProcess()
{
	ConsoleCliListFlush();


	
	vector<string> terminated_list;
	vector<string> running_list;
	vector<string> ready_list;

	int runningCores = 0;
	for (auto& screen_ptr : ScreenController::getInstance()->getScreens()) {

		Process p = *(screen_ptr->getAttachedProcess());
		if (p.getState() == ProcessState::FINISHED) {
			terminated_list.push_back(p.getProcessNameAndInfo());
		}
		else if (p.getState() == ProcessState::RUNNING) {
			running_list.push_back(p.getProcessNameAndInfo());
			runningCores++;

		}

	}


	double core_utilization_percentage = static_cast<double>(runningCores) / CPU::getMaxCores() * 100.0; // Example calculated value
	int core_utilization = static_cast<int>(core_utilization_percentage);

	ConsoleListPush("CPU utilization: " + std::to_string(core_utilization) + "%", 1, 0);
	ConsoleListPush("Cores used: " + std::to_string(runningCores), 1, 0);
	ConsoleListPush("Cores available: " + to_string(CPU::getMaxCores()- runningCores), 1, 0);

	ConsoleListPush("", 1, 0);

	ConsoleListPush("================= PROCESS REPORT ================= ", 1, 0);

	ConsoleListPush("Running Processes:", 1, 0);
	for(const auto& s : running_list) {
		ConsoleListPush(s, 1, 0);
	}
	ConsoleListPush("Finished Processes:", 1, 0);
	for(const auto& s : terminated_list) {
		ConsoleListPush(s, 1, 0);
	}


	drawNvidiaLine(0);

}


void MainConsole::startAnimation() {
	isAnimationGo = true;
}

std::string MainConsole::get_current_timestamp() {
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	std::tm local_tm;
	localtime_s(&local_tm, &now_c); 

	std::stringstream ss;
	ss << std::put_time(&local_tm, "%a %b %d %H:%M:%S %Y");
	return ss.str();
}
