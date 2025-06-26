#pragma once
#include "ICommand.h"
#include "process.h"

class SleepCommand: public ICommand
{
public:
	SleepCommand(int pid, uint8_t sleepcount);

	void execute() const override;
private:
	uint8_t sleepcount;

};

