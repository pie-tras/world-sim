#pragma once

#include "Global.h"
#include "main/WindowManager.h"
#include "render/RenderManager.h"
#include "game/state/GameState.h"
#include "game/state/SplashState.h"
#include "game/state/MenuState.h"
#include "game/state/PlayState.h"

class GameStateManager {
private:
	WindowManager* win_mgr;
	RenderManager* renderer;

	GameState* activeState;
	SplashState* splashState;
	MenuState* menuState;
	PlayState* playState;

public:
	GameStateManager(WindowManager* win_mgr, RenderManager* renderer);
	~GameStateManager();
	void update();
	void setActiveState(States state);
};