#include "Texture.h"

Texture::Texture(unsigned int textureID, int width, int height)
	: textureID(textureID), width(width), height(height) {}

Texture::Texture(unsigned int textureID, int width, int height, glm::vec2 offset, glm::vec2 tileScale)
	: textureID(textureID), width(width), height(height), offset(offset), tileScale(tileScale), usesTextureAtlas(true) {}

unsigned int Texture::getTextureID() {
	return textureID;
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}

bool Texture::isTextureAtlas() {
	return usesTextureAtlas;
}

glm::vec2 Texture::getOffset() {
	return offset;
}

glm::vec2 Texture::getTileScale() {
	return tileScale;
}