#pragma once

#include <iostream>
#include <string>
#include <regex>
#include <conio.h>
#include <regex>
#include <array>

class InputMan
{
	public:
		InputMan() {
			this->command_list = {
				//(std::regex)"(invalid)",			    //-1
				(std::regex)"(initialize)", 			//0
				(std::regex)"(exit)", 					//1
				(std::regex)R"(screen -s (\S+))",		//2
				(std::regex)"(screen -ls)",				//3
				(std::regex)"(scheduler-start)",		//4
				(std::regex)"(scheduler-stop)",			//5
				(std::regex)"(report-util)",			//6
				(std::regex)"(ping)",					//7
				(std::regex)"(clear)"					//8
			};


		}

		void Input(std::string& input, int& sKeys, bool& isCmdDone) {

			if (_kbhit()) {
				char key = _getch();

				// Check if it's an extended key
				if (key == 0 || key == 224) {
					// It's an extended key, so get the actual key code
					key = _getch(); // Read the second byte for extended keys

					switch (key) {
					case 72: // Up arrow
						sKeys = 72;
						// No need to add to 'input' for arrow keys
						break;
					case 80: // Down arrow
						sKeys = 80;
						// No need to add to 'input' for arrow keys
						break;

					default:
						// You can add debugging here if you want to see other extended key codes
						// std::cout << "Unhandled extended key code: " << key << std::endl;
						break;
					}

				}
				else {
					// It's a regular character or symbol key
					switch (key) {
					case '\r': // Enter key (often '\r' on Windows)
					case '\n': // Sometimes '\n' is also returned or preferred for consistency
						if ((input).length() != 0) {
							isCmdDone = true;
						}
						break;
					case 8:
						if (!input.empty()) { // Ensure input is not empty before popping
							input.pop_back();
						}
						break;
					default:

						input += key;
						break;
					}
				}





			}
		}
		void CLI_Comms(std::string& input, int& action, bool& isCmdDone, std::smatch& match){
	

			bool is_valid = false;
			bool is_key = false;
			if (isCmdDone) {

				for (int i = 0; i < this->command_list.size(); i++) {
					if (regex_match(input, match, this->command_list[i])) {

						is_valid = true;
						action = i;
						i = command_list.size();
					}
				}
				if (!is_valid) {
					action = -1;
				}
				input = "";
				isCmdDone = false;

			}

		}





	private:


		std::vector<std::regex> command_list;


};

