#include "ForCommand.h"



// Layer = 0 topmost for loop
// Layer = 2 innermost loop
// Counter for how many times each command is repeated
// Command index as commands ith time
ForCommand::ForCommand(int pid, int counter, int layer)
	: ICommand(pid, CommandType::FOR)
{
	this->pid = pid;
	this->counter = counter;
	this->layer = layer;
	this->maxCommands = 0;


	this->currCounterVar = "FORLOOP_COUNT_" + std::to_string(layer + 1);
	this->currCommandIndexVar = "FORLOOP_COMMANDSINDEX_" + std::to_string(layer + 1);
	
	
	std::shared_ptr<Process> processTemp = process_table[pid];

	processTemp->updateForLoopTable(currCounterVar, 0);
	processTemp->updateForLoopTable(currCommandIndexVar, 0);

}

void ForCommand::addCommand(std::shared_ptr<ICommand> command) {
	if (command != nullptr) {
		commandList.push_back(command);
		maxCommands += 1;
	}

}

void ForCommand::execute() const {
	std::shared_ptr<Process> processTemp = process_table[pid];
	std::unordered_map<std::string, uint16_t> symbolTable = processTemp->getForLoopTable();
	uint16_t currCounter = symbolTable[currCounterVar];
	uint16_t currCommandIndex = symbolTable[currCommandIndexVar];
	processTemp->ConsoleLogPush();
	int core_id = processTemp->getCPUCoreID();

	if (currCounter >= counter) {

		if(layer==0)
			processTemp->updateForLoopTable("FORLOOP_INTERRUPTED_1", 0);
		return;

	}


	if (currCommandIndex < maxCommands) {
		if (commandList.empty()) { 
			
		}
		else {
			if(commandList.at(currCommandIndex))
				commandList.at(currCommandIndex)->execute();

		}
		currCommandIndex++;
		processTemp->updateForLoopTable(currCommandIndexVar, currCommandIndex);
	}
	if (currCommandIndex >= maxCommands) {
	
		currCommandIndex = 0; 
		processTemp->updateForLoopTable(currCommandIndexVar, currCommandIndex); 
		currCounter++; 
		processTemp->updateForLoopTable(currCounterVar, currCounter); 

	}
	if (layer == 0 && currCounter < counter)
		processTemp->updateForLoopTable("FORLOOP_INTERRUPTED_1", 1);






}