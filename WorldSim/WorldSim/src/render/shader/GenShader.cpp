#include "GenShader.h"

GenShader::GenShader(const std::string& key) : Shader(key) {
	getAllUniformLocations();
}

void GenShader::getAllUniformLocations() {
	location_mvp = glGetUniformLocation(programID, "u_mvp");
	location_color = glGetUniformLocation(programID, "u_color");
	location_textureType = glGetUniformLocation(programID, "u_textureType");
	location_textureOffset = glGetUniformLocation(programID, "u_textureOffset");
	location_textureScale = glGetUniformLocation(programID, "u_textureScale");
}

void GenShader::loadMVP(const glm::mat4& mvp) {
	loadMat4f(location_mvp, mvp);
}

void GenShader::loadColor(const glm::vec4& color) {
	loadVec4f(location_color, color);
}

void GenShader::loadTextureType(const int textureType) {
	loadInt(location_textureType, textureType);
}

void GenShader::loadTextureOffset(const glm::vec2& textureOffset) {
	loadVec2f(location_textureOffset, textureOffset);
}

void GenShader::loadTextureScale(const glm::vec2& textureScale) {
	loadVec2f(location_textureScale, textureScale);
}