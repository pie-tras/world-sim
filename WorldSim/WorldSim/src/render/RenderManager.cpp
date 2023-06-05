#include "Global.h"

#include "RenderManager.h"

RenderManager::RenderManager(WindowManager* win_mgr) {
	this->win_mgr = win_mgr;
	genShader = new GenShader("gen");

	loadModel(quad);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
}

RenderManager::~RenderManager() {
	delete genShader;
}

void RenderManager::loadModel(const float* quad) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RenderManager::prepare(Shader* shader) {
	if (activeShader != shader) {
		if (activeShader != nullptr) {
			finish(activeShader);
		}
		activeShader = shader;
		shader->start();
	}
}

void RenderManager::finish(Shader* shader) {
	shader->stop();
}

void RenderManager::loadGenUniforms(const glm::vec2& position, const float rotation_deg, const glm::vec2& scale, const glm::vec4& color) {
	glm::mat4 projection = glm::ortho(-(float)win_mgr->getWindowWidth() / 2, (float)win_mgr->getWindowWidth() / 2, -(float)win_mgr->getWindowHeight() / 2, (float)win_mgr->getWindowHeight() / 2, -1.0f, 1.0f);

	if (renderMode == RenderModes::WORLD) {
		projection = glm::scale(projection, glm::vec3(glm::vec2(win_mgr->getContentScale()), 1.0f));
	}

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(glm::vec2(position.x, position.y), 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation_deg), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale.x + 0.001f, scale.y + 0.001f, 1.0f));

	genShader->loadMVP(projection * win_mgr->camera->getViewMatrix() * model);
	genShader->loadColor(color);
}

void RenderManager::render(Texture* texture, const glm::vec2& position, const float rotation_deg, const glm::vec2& scale, const glm::vec4& color) {
	prepare(genShader);

	//if (!win_mgr->onScreen(position)) return;

	if (texture != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

		if (texture->isTextureAtlas()) {
			genShader->loadTextureType(TextureType::ATLAS);
			genShader->loadTextureOffset(texture->getOffset());
			genShader->loadTextureScale(texture->getTileScale());
		}
		else {
			genShader->loadTextureType(TextureType::NORMAL);
		}

	}
	else {
		genShader->loadTextureType(TextureType::NONE);
	}

	loadGenUniforms(position, rotation_deg, scale, color);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RenderManager::render(Texture* texture, const glm::vec2& position, const float rotation_deg, const glm::vec2& scale) {
	prepare(genShader);

	//if (!win_mgr->onScreen(position)) return;

	if (texture != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getTextureID());

		if (texture->isTextureAtlas()) {
			genShader->loadTextureType(TextureType::ATLAS);
			genShader->loadTextureOffset(texture->getOffset());
			genShader->loadTextureScale(texture->getTileScale());
		}
		else {
			genShader->loadTextureType(TextureType::NORMAL);
		}

	}
	else {
		genShader->loadTextureType(TextureType::NONE);
	}

	loadGenUniforms(position, rotation_deg, scale, glm::vec4(1, 1, 1, 1));
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RenderManager::setRenderMode(RenderModes renderMode) {
	this->renderMode = renderMode;
}

RenderModes RenderManager::getRenderMode() {
	return renderMode;
}