#include "SleepCommand.h"


SleepCommand::SleepCommand(int pid, uint8_t sleepcount):ICommand(pid, SLEEP)
{
	this->pid = pid;
	this->sleepcount = sleepcount;

}

void SleepCommand::execute() const {
	std::shared_ptr<Process> process = process_table[pid];

	process->setCurrMsgLog("SLEEP\t Sleeping for " + std::to_string(sleepcount) + "ms");

	std::this_thread::sleep_for(std::chrono::milliseconds(this->sleepcount));

}