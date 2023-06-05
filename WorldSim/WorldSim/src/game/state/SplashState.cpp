#include "SplashState.h"
#include "game/generate/Biome.h"

#include <iostream>
#include <algorithm>

SplashState::SplashState(WindowManager* win_mgr, RenderManager* renderer) : GameState(win_mgr, renderer) {

}

SplashState::~SplashState() {
	delete mapGen;
	delete mapTex;
}

void SplashState::init() {
	renderer->setRenderMode(RenderModes::GUI);
	mapSize = 1024;
	mapGen = new MapGen(mapSize, 10000);
	mapGen->generateMap();
	loadMap();
}

void SplashState::tick() {
	if (win_mgr->isKeyPressed(GLFW_KEY_R)) {
		mapGen->generateMap();
		loadMap();
	}

	if (win_mgr->isKeyPressed(GLFW_KEY_UP)) {
		if (win_mgr->isKeyDown(GLFW_KEY_J)) {
			moveSpeed += 0.1;
			std::cout << "MOVE SPEED: " << moveSpeed << std::endl;
		}

		if (win_mgr->isKeyDown(GLFW_KEY_K)) {
			zoomSpeed += 0.1;
			std::cout << "ZOOM SPEED: " << zoomSpeed << std::endl;
		}
	}

	if (win_mgr->isKeyPressed(GLFW_KEY_DOWN)) {
		if (win_mgr->isKeyDown(GLFW_KEY_J)) {
			moveSpeed -= 0.1;
			std::cout << "MOVE SPEED: " << moveSpeed << std::endl;
		}

		if (win_mgr->isKeyDown(GLFW_KEY_K)) {
			zoomSpeed -= 0.1;
			std::cout << "ZOOM SPEED: " << zoomSpeed << std::endl;
		}
	}

	if (win_mgr->isKeyPressed(GLFW_KEY_1)) {
		viewMode = 1;
		std::cout << "VIEW BIOME MAP" << std::endl;
		loadMap();
	}

	if (win_mgr->isKeyPressed(GLFW_KEY_2)) {
		viewMode = 2;
		std::cout << "VIEW HEIGHT MAP" << std::endl;
		loadMap();
	}

	if (win_mgr->isKeyPressed(GLFW_KEY_3)) {
		viewMode = 3;
		std::cout << "VIEW TEMPERATURE MAP" << std::endl;
		loadMap();
	}

	if (win_mgr->isKeyPressed(GLFW_KEY_4)) {
		viewMode = 4;
		std::cout << "VIEW HUMIDITY MAP" << std::endl;
		loadMap();
	}

	if (win_mgr->isKeyDown(GLFW_KEY_A)) {
		offset.x += moveSpeed;
		std::cout << offset.x << "," << offset.y << " : " << scale << std::endl;
	}

	if (win_mgr->isKeyDown(GLFW_KEY_D)) {
		offset.x -= moveSpeed;
		std::cout << offset.x << "," << offset.y << " : " << scale << std::endl;
	}

	if (win_mgr->isKeyDown(GLFW_KEY_S)) {
		offset.y += moveSpeed;
		std::cout << offset.x << "," << offset.y << " : " << scale << std::endl;
	}

	if (win_mgr->isKeyDown(GLFW_KEY_W)) {
		offset.y -= moveSpeed;
		std::cout << offset.x << "," << offset.y << " : " << scale << std::endl;
	}

	if (win_mgr->isKeyDown(GLFW_KEY_Q)) {
		scale += zoomSpeed;
		std::cout << offset.x << "," << offset.y << " : " << scale << std::endl;
	}

	if (win_mgr->isKeyDown(GLFW_KEY_E)) {
		scale -= zoomSpeed;
		std::cout << offset.x << "," << offset.y << " : " << scale << std::endl;
	}
}

void SplashState::render() {
	renderer->render(mapTex, offset, 0.0f, glm::vec2(300 * scale, 300 * scale), glm::vec4(1, 1, 1, 1));
}

void SplashState::stop() {
	GameState::stop();
	renderer->setRenderMode(RenderModes::WORLD);
}

void SplashState::loadMap() {
	int* heightData = new int[mapSize * mapSize];
	double* humidityData = new double[mapSize * mapSize];
	double* temperatureData = new double[mapSize * mapSize];
	int* biomeData = new int[mapSize * mapSize];

	FILE* file;
	fopen_s(&file, "data/map/height.bin", "rb");
	fread(heightData, sizeof(*heightData), mapSize * mapSize, file);
	fclose(file);

	fopen_s(&file, "data/map/humidity.bin", "rb");
	fread(humidityData, sizeof(*humidityData), mapSize * mapSize, file);
	fclose(file);

	fopen_s(&file, "data/map/temperature.bin", "rb");
	fread(temperatureData, sizeof(*temperatureData), mapSize * mapSize, file);
	fclose(file);

	fopen_s(&file, "data/map/biome.bin", "rb");
	fread(biomeData, sizeof(*biomeData), mapSize * mapSize, file);
	fclose(file);


	unsigned char* color = new unsigned char[mapSize * mapSize * 4];

	int index = 0;
	for (int j = mapSize - 1; j >= 0; --j) {
		for (int i = 0; i < mapSize; ++i) {

			int height = heightData[i + j * mapSize];
			int heightColor = 255 * (height / 10000.0f); 
			if (heightColor < 0) heightColor = 0;

			double temperature = std::clamp(temperatureData[i + j * mapSize], 0.0, 50.0);
			int temperatureColor = 255 * ((temperature) / 50.0);

			double pressure = humidityData[i + j * mapSize];
			int humidityColor = 255 * (pressure);

			Biome biome = Biome(biomeData[i + j * mapSize]);

			if (viewMode == 1) {
				if (biome == Biome::OCEAN) {
					color[index++] = 0;
					color[index++] = 50;
					color[index++] = 100;
					color[index++] = 255;
				} else if (biome == Biome::TUNDRA) {
					color[index++] = 147;
					color[index++] = 167;
					color[index++] = 172;
					color[index++] = 255;
				} else if (biome == Biome::BOREAL) {
					color[index++] = 37;
					color[index++] = 92;
					color[index++] = 71;
					color[index++] = 255;
				} else if (biome == Biome::GRASSLAND) {
					color[index++] = 184;
					color[index++] = 189;
					color[index++] = 80;
					color[index++] = 255;
				} else if (biome == Biome::WOODLAND) {
					color[index++] = 147;
					color[index++] = 189;
					color[index++] = 80;
					color[index++] = 255;
				} else if (biome == Biome::FOREST) {
					color[index++] = 64;
					color[index++] = 144;
					color[index++] = 59;
					color[index++] = 255;
				} else if (biome == Biome::DESERT) {
					color[index++] = 229;
					color[index++] = 225;
					color[index++] = 152;
					color[index++] = 255;
				} else if (biome == Biome::SAVANNA) {
					color[index++] = 151;
					color[index++] = 165;
					color[index++] = 39;
					color[index++] = 255;
				} else if (biome == Biome::RAINFOREST) {
					color[index++] = 7;
					color[index++] = 83;
					color[index++] = 48;
					color[index++] = 255;
				} else {
					color[index++] = 255;
					color[index++] = 255;
					color[index++] = 0;
					color[index++] = 255;
				}
			} else if (viewMode == 2) {
				color[index++] = heightColor;
				color[index++] = heightColor;
				color[index++] = heightColor;
				color[index++] = 255;
			} else if (viewMode == 3) {
				color[index++] = temperatureColor;
				color[index++] = temperatureColor;
				color[index++] = temperatureColor;
				color[index++] = 255;
			}
			else {
				color[index++] = humidityColor;
				color[index++] = humidityColor;
				color[index++] = humidityColor;
				color[index++] = 255;
			}
		}
	}

	unsigned int texID = 0;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width = mapSize;
	int height = mapSize;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, color);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (mapTex) {
		unsigned int texID = mapTex->getTextureID();
		glDeleteTextures(1, &texID);
		delete mapTex;
	}

	mapTex = new Texture(texID, mapSize, mapSize);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] heightData;
	delete[] temperatureData;
	delete[] humidityData;
	delete[] biomeData;

	delete[] color;
}