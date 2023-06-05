#pragma once

#include "Global.h"
#include "main/WindowManager.h"
#include "render/shader/GenShader.h"
#include "RenderModes.h"
#include "textures/Texture.h"

class RenderManager {
private:
	WindowManager* win_mgr;
	Shader* activeShader;

	GenShader* genShader;
	RenderModes renderMode = RenderModes::WORLD;

	float quad[8] = { -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };

	void loadModel(const float* quad);
	void prepare(Shader* shader);
	void finish(Shader* shader);
	void loadGenUniforms(const glm::vec2& position, const float rotation_deg, const glm::vec2& scale, const glm::vec4& color);
public:
	RenderManager(WindowManager* win_mgr);
	~RenderManager();
	void render(Texture* texture, const glm::vec2& position, const float rotation_deg, const glm::vec2& scale, const glm::vec4& color);
	void render(Texture* texture, const glm::vec2& position, const float rotation_deg, const glm::vec2& scale);
	RenderModes getRenderMode();
	void setRenderMode(RenderModes renderMode);
};