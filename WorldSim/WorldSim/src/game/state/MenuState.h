#pragma once

#include "Global.h"
#include "game/state/GameState.h"
#include "main/WindowManager.h"
#include "render/RenderManager.h"

class MenuState : public GameState {
private:

public:
	MenuState(WindowManager* win_mgr, RenderManager* renderer);
	~MenuState();

	void init() override;
	void tick() override;
	void render() override;
	void stop() override;
};