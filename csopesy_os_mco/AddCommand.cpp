#include "AddCommand.h"


AddCommand::AddCommand(int pid, std::string var1, uint16_t var2, uint16_t var3):ICommand(pid, ADD)
{
    this->pid = pid;
	this->var1 = var1;
	this->var2 = var2;
	this->var3 = var3;


}



void AddCommand::execute() const {

    unsigned long long temp_sum = static_cast<unsigned long long>(var2) + var3;
    std:: shared_ptr<Process> processTemp = process_table[pid];
    std::unordered_map<std::string, uint16_t> symbolTable = processTemp->getSymbolTable();
    uint16_t final_result;



    if (temp_sum > 65535) {
        final_result = 65535;
    }
    else {
        final_result = static_cast<uint16_t>(temp_sum); // Safe to cast back now
    }

    processTemp->updateSymbolTable(var1, final_result);
    processTemp->setCurrMsgLog("ADD\t " + var1 + "="  + std::to_string(var2) + " + " + std::to_string(var3));
}