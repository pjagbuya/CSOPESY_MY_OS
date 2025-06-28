#pragma once


#include <string>
#include <thread>
#include <cstdint> 
#include <limits> 
#include <vector>
#include <memory>
#include "globals.h"
#include <mutex>
enum CommandType
{

	PRINT,
	DECLARE,
	ADD,
	SUBTRACT,
	SLEEP,
	FOR
};
class ICommand
{
public:

	ICommand(int pid, CommandType type);
	CommandType getCommandType() const;
	std::string getMsgLog() const;
	virtual void execute() const = 0;


protected:
	int pid; // Process ID
	CommandType commandType;
	std::string msgLog;

	
};

