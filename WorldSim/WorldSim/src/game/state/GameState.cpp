#include "GameState.h"

GameState::GameState(WindowManager* win_mgr, RenderManager* renderer) :
	win_mgr(win_mgr), renderer(renderer) {

}

GameState::~GameState() {
}

inline void init() {}
inline void tick() {}
inline void render() {}

void GameState::stop() {
	nextState = States::NONE;
}