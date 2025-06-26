#pragma once
#include "ICommand.h"
#include "process.h"


class ForCommand: public ICommand
{
public:
	ForCommand(int pid, int counter, int layer);

	void execute() const override; // init

	void addCommand(std::shared_ptr<ICommand>);

	int getTotalInstructions() const;

private:
	std::vector<std::shared_ptr<ICommand>> commandList;
	int counter;
	int layer;
	int maxCommands;

	std::string currCounterVar;
	std::string currCommandIndexVar;

	static const int maxLayer = 3;
};

