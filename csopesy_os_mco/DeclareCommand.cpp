#include "DeclareCommand.h"

#include "screen_controller.h"
DeclareCommand::DeclareCommand(int pid, std::string var1, uint16_t var2):ICommand(pid, DECLARE)
{
	this->pid = pid;
	this->var1 = var1;
	this->var2 = var2;

}


void DeclareCommand::execute() const {

	process_table[pid]->setCurrMsgLog("Declaring " + var1 + ":" + std::to_string(var2));
	process_table[pid]->ConsoleLogPush();

	process_table[pid]->updateSymbolTable(var1, var2);

}