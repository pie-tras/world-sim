#pragma once

#include "Global.h"
#include "game/state/GameState.h"
#include "main/WindowManager.h"
#include "render/RenderManager.h"
#include "game/States.h"
#include "render/RenderModes.h"

#include "game/generate/MapGen.h"

class SplashState : public GameState {
private:
	MapGen* mapGen;
	Texture* mapTex = nullptr;
	int mapSize;
	float scale = 1.0f;
	glm::vec2 offset = glm::vec2(0.0f, 0.0f);

	float moveSpeed = 2.0f;
	float zoomSpeed = 0.1f;

	int viewMode = 1; //1 = heightMap, 2 = color, 3 = temp 4 = pressure
public:
	SplashState(WindowManager* win_mgr, RenderManager* renderer);
	~SplashState();

	void init() override;
	void tick() override;
	void render() override;
	void stop() override;

	void loadMap();
};