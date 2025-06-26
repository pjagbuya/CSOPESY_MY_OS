#pragma once  
#include <vector>  
#include <string>  
#include <unordered_map> // Include the header for unordered_map  
#include <memory>
#include "ICommand.h"
#include "cpu.h"
enum ProcessState
{
	READY,
	RUNNING,
	WAITING,
	FINISHED
};
class Process  
{
public:

	Process() = delete;

	~Process();

	Process(int pid, std::string name, std::shared_ptr<std::vector<std::string>> out);
	void addCommand(std::shared_ptr<ICommand> command);
	void executeCurrentCommand();  
	void setProcessCommands(std::vector<std::shared_ptr<ICommand>> commands);
	void moveToNextLine();  

	bool isFinished() const;  
	int getRemainingTime() const;  
	int getCommandCounter() const;  
	int getLinesOfCode() const;  
	int getPid() const;  
	int getCPUCoreID() const;
	ProcessState getState() const;
	std::string getName() const;
	std::string getCurrMsgLog() const;
	std::string getTimeStamp();
	std::vector<std::string> getPrintLog();
	int getVariableCounter() const;


	std::unordered_map<std::string, uint16_t> getSymbolTable();
	std::unordered_map<std::string, uint16_t> getForLoopTable();

	void updateSymbolTable(std::string varname, uint16_t value);
	void updateForLoopTable(std::string varname, uint16_t value);
	void setProcessState(ProcessState state);
	void setCPUCoreID(int id);



	void setCpuCoreID(int id);


	void ConsoleLogPush();


	void updateProcess();
	void resetLoggingLimit();


	void setCurrMsgLog(std::string msg);
	void sendPrintOut(std::string msg);
	int readAtForLoopTable(std::string msg);


private:
	int pid;  
	std::string name;  
	int remainingTime; 
	int cpuCoreID;  
	std::unordered_map<std::string, uint16_t> symbolTable;
	std::unordered_map<std::string, uint16_t> forLoopTable;
	std::shared_ptr<std::vector<std::string>> process_output_list;

	std::unordered_map<int, std::vector<std::string>> coreLogs;
	int variableCounter;
	int loggingLimit;
	std::mutex mtx;


	ProcessState state;

	uint64_t delay;
	size_t instruction_index;
	uint64_t current_line_instruction;  
	uint64_t total_lines_instruction;



	std::string currMsgLog;
	std::vector<std::string> printLog;
	std::vector<std::shared_ptr<ICommand>> commandList; // Execution times for each command  

	std::vector<int> commandExecutionTimes; // Execution times for each command  
	std::vector<int> commandLineNumbers; // Line numbers for each command  

};
