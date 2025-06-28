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
	processTemp->updateForLoopTable("INTERRUPT", 0);


}

void ForCommand::addCommand(std::shared_ptr<ICommand> command) {
	if (command != nullptr) {
		commandList.push_back(command);
		maxCommands += 1;
	}

}

void ForCommand::execute() const {
	std::shared_ptr<Process> processTemp = process_table[pid];
	uint16_t currCounter = processTemp->readAtForLoopTable(currCounterVar);
	uint16_t currCommandIndex = processTemp->readAtForLoopTable(currCommandIndexVar);
	std::string tempMsg = "FOR\t Nest level " + std::to_string(layer) + "| Counter progress: " + std::to_string(currCounter)+"/" +std::to_string(counter);
	processTemp->setCurrMsgLog(tempMsg);
	int core_id = processTemp->getCPUCoreID();
	int interrupt = processTemp->readAtForLoopTable("INTERRUPT");

	//if (layer == 0) {
	//	processTemp->updateForLoopTable("INTERRUPT", 0);
	//}

	while (currCounter < counter && interrupt==0) {
		if (currCounter >= counter) {
			processTemp->updateForLoopTable("INTERRUPT", 0);
			break;

		}


		if (currCommandIndex < maxCommands) {
			if (commandList.empty()) {

			}
			else {
				if (commandList.at(currCommandIndex))
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

		interrupt = processTemp->readAtForLoopTable("INTERRUPT");

	}






}