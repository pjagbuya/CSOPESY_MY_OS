#include "SubtractCommand.h"


SubtractCommand::SubtractCommand(int pid, std::string var1, uint16_t var2, uint16_t var3):ICommand(pid, SUBTRACT)
{
	this->pid = pid;
	this->var1 = var1;
	this->var2 = var2;
	this->var3 = var3;

}



void SubtractCommand::execute() const {
    long long temp_result = static_cast<long long>(var2) - var3;
    std::shared_ptr<Process> processTemp = process_table[pid];
    std::unordered_map<std::string, uint16_t> symbolTable = processTemp->getSymbolTable();
    uint16_t final_result;


    processTemp->setCurrMsgLog("SUBTRACT\t " + var1 + "=" + std::to_string(var2) + "-" + std::to_string(var3) );

    if (temp_result < 0) {
        final_result = 0;
    }
    else {
        final_result = static_cast<uint16_t>(temp_result);
    }
    processTemp->updateSymbolTable(var1, final_result);

}