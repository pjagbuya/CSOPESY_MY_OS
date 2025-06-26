#pragma once
#include "ICommand.h"
#include "process.h"

class DeclareCommand: public ICommand
{
public:
	DeclareCommand(int pid, std::string var1, uint16_t var2);
	void execute() const override;
private:
	int pid;
	std::string var1;
	uint16_t var2; 

};

