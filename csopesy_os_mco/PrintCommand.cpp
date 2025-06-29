#include "PrintCommand.h"

PrintCommand::PrintCommand(int pid, std::string msg) : ICommand(pid, CommandType::PRINT)
{
	this->pid = pid;
	this->msgLog = msg;

}




void PrintCommand::execute() const {  
    std::shared_ptr<Process> process = process_table[pid];

    const auto& table = process->getSymbolTable();
    std::string msg;
    auto it = table.find(this->msgLog);
    if (it != table.end()) {
        msg = "Value from: " + std::to_string(it->second);


    }

    process->sendPrintOut(msg);

    process->setCurrMsgLog(msg);


}
