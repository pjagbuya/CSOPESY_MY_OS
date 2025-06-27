#include "process.h"
#include <random>
#include <chrono>
Process::~Process()
{
    this->symbolTable.clear();
}
// Constructor
Process::Process(int pid, std::string name, std::shared_ptr<std::vector<std::string>> out)
    : pid(pid), name(name), remainingTime(0), current_line_instruction(0),
    total_lines_instruction(CPU::getMinIns()), cpuCoreID(-1)
{
    this->updateForLoopTable("FORLOOP_INTERRUPTED_1", 0);

    process_output_list = out;
	state = READY;
    loggingLimit = 1;
    currMsgLog = "EMPTY";
    instruction_index = 0;
}

// Add a command placeholder (not implemented due to missing ICommand list)
void Process::addCommand(std::shared_ptr<ICommand> command) {
	if (command != nullptr) {
		commandList.push_back(command);
        
	}

}

// Executes the "current command" — stubbed logic
void Process::executeCurrentCommand() {
	std::lock_guard<std::mutex> lock(mtxProcess);
    if(this->readAtForLoopTable("INTERRUPT") == 0)
        this->commandList.at(current_line_instruction)->execute();

}

void Process::setProcessCommands(std::vector<std::shared_ptr<ICommand>> commands)
{


	this->commandList = commands;
	
}

// Moves to the next command
void Process::moveToNextLine() {

    if(this) current_line_instruction++;

}

// True if all commands are done
bool Process::isFinished() const {
    return current_line_instruction >= total_lines_instruction || commandList.empty();
}

int Process::getRemainingTime() const {
    return remainingTime;
}

int Process::getCommandCounter() const {
    return current_line_instruction;
}

int Process::getLinesOfCode() const {
    return total_lines_instruction;
}

int Process::getPid() const {
    return pid;
}

int Process::getCPUCoreID() const {
    return cpuCoreID;
}

std::unordered_map<std::string, uint16_t> Process::getSymbolTable()
{
    std::lock_guard<std::mutex> lock(mtx);

	return symbolTable;
}

std::unordered_map<std::string, uint16_t> Process::getForLoopTable()
{
    std::lock_guard<std::mutex> lock(mtx);

    return forLoopTable;
}

void Process::updateSymbolTable(std::string varname, uint16_t value)
{
    if (this) {
        std::lock_guard<std::mutex> lock(mtx);
        if (symbolTable.find(varname) == symbolTable.end()) {
            symbolTable[varname] = value;
            variableCounter++; 
        }
        else {
            symbolTable[varname] = value;
        }
    }



}

void Process::updateForLoopTable(std::string varname, uint16_t value)
{
    if (this) {
        std::lock_guard<std::mutex> lock(mtx);

        forLoopTable[varname] = value;
    }

}

// Dummy placeholder since enum is private in header
ProcessState Process::getState() const {
    // You might want to store `state` in a private member (commented out in your header)
    return this->state;
}
std::string Process::getCurrMsgLog() {
    std::lock_guard<std::mutex> lock(mtxMsg);

    return currMsgLog;
}
void Process::setProcessState(ProcessState state)
{
	this->state = state;
}
void Process::setCPUCoreID(int id)
{
    this->cpuCoreID = id;
}
std::string Process::getName() const {
    return name;
}


void Process::setCpuCoreID(int id)
{
    this->cpuCoreID = id;
}

std::string Process::getTimeStamp()
{
    auto now = std::chrono::system_clock::now();
    time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm_struct;
    localtime_s(&tm_struct, &now_c);

    std::stringstream ss;

    ss << std::put_time(&tm_struct, "%m/%d/%Y, %I:%M:%S %p");

    return ss.str();
}

void Process::ConsoleLogPush()
{
    if(this)
        coreLogs[cpuCoreID].push_back("(" + getTimeStamp() + ")" + " Core:" + std::to_string(this->cpuCoreID) + " " + this->getCurrMsgLog());

}

std::vector<std::string> Process::getPrintLog()
{

    for (int i = 0; i < CPU::getMaxCores(); i++) {

        int logTrack = 0;

        if (coreLogs.find(i) != coreLogs.end()) { // Ensure the coreLogs map contains the key
            for (size_t j =0 ; j< coreLogs.at(i).size(); j++) {
                printLog.push_back(coreLogs.at(i).at(j)); // Use `at()` for safe access
                
                logTrack++;
                if (loggingLimit == logTrack && i == CPU::getMaxCores()-1) {
					loggingLimit++;
                }
            }
        }
    }
    return printLog;
}

int Process::getVariableCounter() const
{
    return variableCounter;
}

void Process::updateProcess() {

    this->total_lines_instruction = this->commandList.size();

}

void Process::resetLoggingLimit()
{

	loggingLimit = 1;

}

void Process::setCurrMsgLog(std::string msg)
{
    std::lock_guard<std::mutex> lock(mtxMsg);

    currMsgLog = msg;
}

void Process::sendPrintOut(std::string msg)
{
    process_output_list->push_back(msg);
}

int Process::readAtForLoopTable(std::string msg)
{
    std::lock_guard<std::mutex> lock(mtx);


    return forLoopTable[msg];
}

