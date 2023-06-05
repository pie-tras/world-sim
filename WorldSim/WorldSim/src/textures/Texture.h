#pragma once

#include "Global.h"

enum TextureType {
	NONE = 0, NORMAL, ATLAS
};

class Texture {
public:
	Texture(unsigned int textureID, int width, int height);
	Texture(unsigned int textureID, int width, int height, glm::vec2 offset, glm::vec2 tileScale);

	unsigned int getTextureID();
	int getWidth();
	int getHeight();
	bool isTextureAtlas();
	glm::vec2 getOffset();
	glm::vec2 getTileScale();

	bool usesTextureAtlas = false;
	glm::vec2 offset;
	glm::vec2 tileScale;

private:
	unsigned int textureID;
	int width, height;
};

