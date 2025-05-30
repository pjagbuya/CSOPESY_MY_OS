#pragma once

#include <string>
#include <vector>

class Header
{
	Header() {
		this->header = {
			" _____  _____  ___________ _____ _______   __",
			"/  __ \\/  ___||  _  | ___ \\  ___/  ___\\ \\ / /",
			"| /  \\/\\ `--. | | | | |_/ / |__ \\ `--. \\ V / ",
			"| |     `--. \\| | | |  __/|  __| `--. \\ \\ /  ",
			"| \\__/\\/\\__/ /\\ \\_/ / |   | |___/\\__/ / | |  ",
			" \\____/\\____/  \\___/\\_|   \\____/\\____/  \\_/  "
		};
		header.push_back("Welcome to screen command line");
		header.push_back("exit' to quit, 'clear' to clear the screen");

	}


	int getHeaderSize() const {
		return headerSize;
	}


	std::vector<std::string> header;
	int headerSize;
};

