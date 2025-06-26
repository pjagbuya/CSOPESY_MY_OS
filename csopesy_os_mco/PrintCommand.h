#pragma once
#include "ICommand.h"
#include "process.h"

class PrintCommand : public ICommand
{
public:
	enum MessageTypes {
		HELLO,
		VAR
	};
	PrintCommand(int pid, std::string msg);
	void execute() const override;


};

