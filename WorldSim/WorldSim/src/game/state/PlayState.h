#pragma once

#include "Global.h"
#include "game/state/GameState.h"
#include "main/WindowManager.h"
#include "render/RenderManager.h"

class PlayState : public GameState {
private:
	
public:
	PlayState(WindowManager* win_mgr, RenderManager* renderer);
	~PlayState();

	void init() override;
	void tick() override;
	void render() override;
	void stop() override;
};