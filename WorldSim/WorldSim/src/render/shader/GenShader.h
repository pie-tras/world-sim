#pragma once

#include "Global.h"
#include "render/shader.h"

class GenShader : public Shader {
private:
	unsigned int location_mvp = 0;
	unsigned int location_color = 0;
	unsigned int location_textureType = 0;
	unsigned int location_textureOffset = 0;
	unsigned int location_textureScale = 0;
public:
	GenShader(const std::string& key);
	void getAllUniformLocations() override;
	void loadMVP(const glm::mat4& mvp);
	void loadColor(const glm::vec4& color);
	void loadTextureType(const int textureType);
	void loadTextureOffset(const glm::vec2& textureOffset);
	void loadTextureScale(const glm::vec2& textureScale);
};