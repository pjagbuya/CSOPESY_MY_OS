#pragma once
#include "ICommand.h"
#include "process.h"

class SubtractCommand: public ICommand
{
public:

	SubtractCommand(int pid, std::string var1, uint16_t var2, uint16_t var3);
	void execute() const override;
private:
	int pid;
	std::string var1;
	uint16_t var2;
	uint16_t var3;
};

