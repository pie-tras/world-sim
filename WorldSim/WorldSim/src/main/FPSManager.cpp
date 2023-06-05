#include "FPSManager.h"

FPSManager::FPSManager(int fps_cap, bool print_fps) :
	fps_cap(fps_cap), print_fps(print_fps) {

	frames = 0;
	unprocessed = 0;
	frameTime = ONE_SECOND / fps_cap;

	cycleTimer.setStopwatch();
	fpsTimer.setStopwatch();
}


void FPSManager::runCycle(void (*cycle)(GameStateManager*, GLFWwindow*), GameStateManager* gsm, GLFWwindow* window) {
	unprocessed += cycleTimer.getDelta();

	if (unprocessed >= frameTime) {
		cycle(gsm, window);
		frames++;
		unprocessed = 0;
	}

	if (print_fps && fpsTimer.getStopwatch() >= ONE_SECOND) {
		std::cout << "[FPS]: " << frames << "\n";
		frames = 0;
		fpsTimer.setStopwatch();
	}
}