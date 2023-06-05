#include "PlayState.h"

PlayState::PlayState(WindowManager* win_mgr, RenderManager* renderer) : GameState(win_mgr, renderer) {

}

PlayState::~PlayState() {

}

void PlayState::init() {
	std::cout << "Init playstate" << std::endl;
}

void PlayState::tick() {

}

void PlayState::render() {
	
}

void PlayState::stop() {
	GameState::stop();
}