#pragma once
#include "ICommand.h"
#include "process.h"

class AddCommand: public ICommand
{
public:
	AddCommand(int pid, std::string var1, uint16_t var2, uint16_t var3);

	void execute() const override;

private:
	int pid;
	std::string var1;
	uint16_t var2;
	uint16_t var3;

};

