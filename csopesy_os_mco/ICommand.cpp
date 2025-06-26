#include "ICommand.h"


ICommand::ICommand(int pid, CommandType type) {
	this->pid = pid;
	this->commandType = type;
	this->msgLog = "DEFAULT";
}

std::string ICommand::getMsgLog() const {
	return msgLog;
}
CommandType ICommand::getCommandType() const {
	return commandType;
}


void ICommand::execute() const {
	//Default implementation
}