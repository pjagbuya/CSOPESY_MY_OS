#include "process.h"




Process::Process(const Process& other)
    : pid(other.pid),
    name(other.name),
    remainingTime(other.remainingTime),
    current_line_instruction(other.current_line_instruction),
    creation_time_stamp(other.creation_time_stamp),
    total_lines_instruction(other.total_lines_instruction),
    cpuCoreID(other.cpuCoreID),
    symbolTable(other.symbolTable), 
    forLoopTable(other.forLoopTable), 
    process_output_list(other.process_output_list), 
    coreLogs(other.coreLogs), 
    variableCounter(other.variableCounter),
    loggingLimit(other.loggingLimit),

    processInfo(other.processInfo),
    state(other.state),
    delay(other.delay),
    instruction_index(other.instruction_index),
    commandList(other.commandList), 
    commandExecutionTimes(other.commandExecutionTimes),
    commandLineNumbers(other.commandLineNumbers),
    currMsgLog(other.currMsgLog),
    printLog(other.printLog)
{

}
Process::~Process()
{
    this->symbolTable.clear();
}
// Constructor
Process::Process(int pid, std::string name, std::shared_ptr<std::vector<std::string>> out, std::string time)
    : pid(pid), name(name), remainingTime(0), current_line_instruction(0), creation_time_stamp(time),
    total_lines_instruction(CPU::getMinIns()), cpuCoreID(-1)
{
    this->updateForLoopTable("FORLOOP_INTERRUPTED_1", 0);
    process_output_list = out;
	state = READY;
    loggingLimit = 1;
    currMsgLog = "EMPTY";
    instruction_index = 0;
    this->variableCounter = 0;
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

    if(this && readAtForLoopTable("INTERRUPT")==0) current_line_instruction++;

}

// True if all commands are done
bool Process::isFinished() {
    if (this)
        return current_line_instruction >= total_lines_instruction;
    else return true;
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
ProcessState Process::getState() {
    std::lock_guard<std::mutex> lock(mtx);

    return this->state;
}
std::string Process::getCurrMsgLog() {
    std::lock_guard<std::mutex> lock(mtxMsg);

    return currMsgLog;
}
void Process::setProcessState(ProcessState state)
{
    std::lock_guard<std::mutex> lock(mtx);

	this->state = state;
}
void Process::setCPUCoreID(int id)
{
    std::lock_guard<std::mutex> lock(mtx);

    this->cpuCoreID = id;
}
std::string Process::getName() const {

    return name;
}


void Process::setCpuCoreID(int id)
{
    std::lock_guard<std::mutex> lock(mtx);

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
    if (this) {
        // Access the vector for the current CPUCoreID
        std::vector<std::string>& logs_for_core = coreLogs[cpuCoreID]; // Access by [] creates if not exists

        // If the vector already has 3 elements, remove the oldest one (the first element)
        if (logs_for_core.size() >= 3) {
            logs_for_core.erase(logs_for_core.begin());
        }

        // Add the new log message to the back
        logs_for_core.push_back("(" + getTimeStamp() + ")" + " Core:" + std::to_string(this->cpuCoreID) + " " + this->getCurrMsgLog());
    }
}

std::vector<std::string> Process::getPrintLog()
{

    printLog.clear();

    for (int i = 0; i < CPU::getMaxCores(); i++) {
        if (coreLogs.count(i)) { // Check if key exists
            const std::vector<std::string>& current_core_logs = coreLogs.at(i);

            // Determine the starting index to get the last 3 elements
            // This is the key part for "most recent three" or "last three".
            // If the vector has fewer than 3 elements, start from index 0.
            // Otherwise, start from size - 3.
            for (const std::string& log_entry : current_core_logs) {
                printLog.push_back(log_entry);
            }



        }
    }
    return printLog;
}

int Process::getVariableCounter() const
{
    return variableCounter;
}

void Process::incVariableCounter()
{
	this->variableCounter++;
}

void Process::updateProcess() {

    this->total_lines_instruction = this->commandList.size();
    this->variableCounter = symbolTable.size();
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
    //process_output_list->push_back(msg);
}

int Process::readAtForLoopTable(std::string msg)
{
    std::lock_guard<std::mutex> lock(mtx);


    return forLoopTable[msg];
}

string Process::getProcessNameAndInfo() {
    if (this->isFinished()) {
        return "process: " + name + +"\t(" + creation_time_stamp + ") " "| Finished\t" + " |Instruction: " + std::to_string(this->getCommandCounter()) + " / " + std::to_string(this->getLinesOfCode());

    }
    else if (this->getCommandCounter() == 0 || this->getCPUCoreID() == -1) {
        return "process: " + name + +"\t(" + creation_time_stamp + ") " "| IDLE\t" + " |Instruction: " + std::to_string(this->getCommandCounter()) + " / " + std::to_string(this->getLinesOfCode());

    }
    else {
        return "process: " + name + +"\t(" + creation_time_stamp + ") " "| Core: " + std::to_string(this->getCPUCoreID()) + "\t |Instruction: " + std::to_string(this->getCommandCounter()) + " / " + std::to_string(this->getLinesOfCode());

    }
}