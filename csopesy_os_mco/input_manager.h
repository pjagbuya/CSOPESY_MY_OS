#pragma once

#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <regex>
#include <array>


#define STRING_COMMANDS_INDEX_MAX 9
class InputMan
{
public:
	InputMan();
	void Input(std::string& input, int& sKeys, bool& isCmdDone);
	void CLI_Comms(std::string& input, int& action, bool& isCmdDone, std::smatch& match);
private:
	int indexLoopCommandMain;
	int indexLoopCommandOptions;

};

