#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include "heindarobo.h"
#include "screen_controller.h"

const static std::array<std::vector<std::string>, 8> hidr_frames = {
	 hidr_frame1,
	 hidr_frame2,
	 hidr_frame3,
	 hidr_frame4,
	 hidr_frame5,
	 hidr_frame6,
	 hidr_frame7,
	 hidr_frame8
};
int hidr_numVars = hidr_frames.size();
int hidr_curInd = 0;

vector<string> ascii_art = {
	" _____  _____  ___________ _____ _______   __",
	"/  __ \\/  ___||  _  | ___ \\  ___/  ___\\ \\ / /",
	"| /  \\/\\ `--. | | | | |_/ / |__ \\ `--. \\ V / ",
	"| |     `--. \\| | | |  __/|  __| `--. \\ \\ /  ",
	"| \\__/\\/\\__/ /\\ \\_/ / |   | |___/\\__/ / | |  ",
	" \\____/\\____/  \\___/\\_|   \\____/\\____/  \\_/  "
};

using namespace std;


void startMainLoop() {

	ScreenController::initialize();

	while (!ScreenController::getInstance()->isCommandQuit()) {
		ScreenController::getInstance()->callInputListener();
	}
	ScreenController::destroy();
}

int main()
{


	startMainLoop();

}