#include "MapGen.h"
#include "Biome.h"

#include <fstream>
#include <string>
#include <sstream>

#include <numeric>
#include <algorithm>
#include <vector>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

MapGen::MapGen(int size, int amplitude) : size(size), amplitude(amplitude) {
	heightData = new int[size * size];
	humidityData = new double[size * size];
	temperatureData = new double[size * size];

	biomeData = new int[size * size];
	srand(time(NULL));
}

MapGen::~MapGen() {
	delete[] heightData;
	delete[] humidityData;
	delete[] temperatureData;

	delete[] biomeData;
}

void MapGen::generateMap() {
	int heighestPoint = 0;
	int lowestPoint = 0;
	int landCount = 0;
	int mountainCount = 0;
	int oceanCount = 0;
	double heighestTemperature = 0.0;
	double lowestTemperature = 0.0;

	std::cout << "Generating Map..." << std::endl;

	int seed = rand();

	std::cout << "Using seed: " << seed << std::endl;

	std::cout << "Generating Terrain..." << std::endl;

	double noiseScale = 3.0f / size;

	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {

			double centerDis = sqrt(pow(x - (size / 2), 2) + pow(y - (size / 2), 2));
			double dropOff = pow(2.0 * centerDis / size, 3);

			double coastLine = stb_perlin_fbm_noise3((x * noiseScale), (y * noiseScale), seed, 2.7, 0.5, 6) - dropOff;
			double mountain = stb_perlin_ridge_noise3((x * noiseScale), (y * noiseScale), seed, 2.9, 0.7, 0.9, 6) - dropOff;

			if (coastLine > 0) {
				mountain = std::clamp(mountain, 0.0, 1.0);
				double ramp = std::clamp(mountain / 0.5, 0.0, 1.0);

				heightData[x + y * size] = amplitude * ((coastLine / 4) + (mountain * ramp));
				
				landCount++;
				if (heightData[x + y * size] > 6000) {
					mountainCount++;
				}

			} else {
				heightData[x + y * size] = amplitude * coastLine;
				oceanCount++;
			}

			heightData[x + y * size] = std::clamp(heightData[x + y * size], -15000, 15000);

			if (heightData[x + y * size] > heighestPoint) heighestPoint = heightData[x + y * size];
			if (heightData[x + y * size] < lowestPoint) lowestPoint = heightData[x + y * size];
		}
	}

	std::cout << "Generating Humidity..." << std::endl;

	double* humidityLine = new double[size];

	for (int y = 0; y < size; y++) {
		double omega = 3.1415 / size;
		double humidity = (cos(omega * y) + 1) / 2.0;
		humidityLine[y] = humidity;
	}

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			humidityData[x + y * size] = 0.0;
		}
	}

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {

			double omega = 3.1415 / size;
			double cap = (cos(omega * y) + 1) / 2.0;

			int height = heightData[x + y * size];

			if (height > 0) {
				humidityLine[y] -= pow(height / double(amplitude), 7);
				
				if (humidityLine[y] < 0) {
					humidityLine[y] = 0.0;
				}
			} else {
				humidityLine[y] += pow(-height / double(amplitude), 3);
				if (humidityLine[y] > cap) {
					humidityLine[y] = cap;
				}
			}

			double noise = stb_perlin_turbulence_noise3((x * noiseScale * 0.5), (y * noiseScale * 0.5), seed, 2.2, 0.5, 6);
			noise *= cap;

			humidityData[x + y * size] = (humidityLine[y] * 0.6) + (noise * 0.4);
		}
	}

	delete[] humidityLine;

	std::cout << "Smoothing Humidity..." << std::endl;

	for (int i = 0; i < 50; i++) {
		for (int y = 1; y < size - 1; y++) {
			for (int x = 1; x < size - 1; x++) {
				humidityData[x + y * size] += humidityData[x + (y + 1) * size] + humidityData[x + (y - 1) * size];
				humidityData[x + y * size] += humidityData[(x + 1) + y * size] + humidityData[(x - 1) + y * size];
				humidityData[x + y * size] += humidityData[(x + 1) + (y + 1) * size] + humidityData[(x + 1) + (y + 1) * size];
				humidityData[x + y * size] += humidityData[(x - 1) + (y - 1) * size] + humidityData[(x - 1) + (y - 1) * size];
				humidityData[x + y * size] /= 9.0;
			}
		}
	}

	for (int i = 0; i < size * size; i++) {
		humidityData[i] = std::clamp(humidityData[i], 0.0, 1.0);
	}

	std::cout << "Generating Temperature..." << std::endl;

	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			double omega = 3.1415 / size;
			double temp = 30 * cos(omega * y) + 25;

			double humidity = humidityData[x + y * size];

			double dryLapseRate = 0.0098;
			double wetLapseRate = 0.0040;

			double lapseRate = (wetLapseRate - dryLapseRate) * humidity + wetLapseRate;

			int height = heightData[x + y * size];

			if (height > 0) {
				temperatureData[x + y * size] = temp - (lapseRate * height);
			}
			else {
				temperatureData[x + y * size] = temp;
			}

			if (temperatureData[x + y * size] > heighestTemperature) heighestTemperature = temperatureData[x + y * size];
			if (temperatureData[x + y * size] < lowestTemperature) lowestTemperature = temperatureData[x + y * size];
		}
	}

	std::cout << "Generating Biomes..." << std::endl;

	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			double height = heightData[x + y * size];
			double temp = temperatureData[x + y * size];
			double humidity = humidityData[x + y * size];

			Biome biome = Biome::OCEAN;

			if (height > 0) {

				if (height > 5000) {
					biome = Biome::TUNDRA;
				} else {
					if (temp < 0) {
						biome = Biome::TUNDRA;
					}
					else if (temp >= 0 && temp < 10) {
						if (humidity < 0.2) {
							biome = Biome::GRASSLAND;
						}
						else if (humidity >= 0.2 && humidity < 0.4) {
							biome = Biome::WOODLAND;
						}
						else {
							biome = Biome::BOREAL;
						}
					}
					else if (temp >= 10 && temp < 30) {
						if (humidity < 0.3) {
							biome = Biome::GRASSLAND;
						}
						else if (humidity >= 0.3 && humidity < 0.45) {
							biome = Biome::WOODLAND;
						}
						else {
							biome = Biome::FOREST;
						}
					}
					else {
						if (humidity < 0.4) {
							biome = Biome::DESERT;
						}
						else if (humidity >= 0.4 && humidity < 0.7) {
							biome = Biome::SAVANNA;
						}
						else {
							biome = Biome::RAINFOREST;
						}
					}
				}
			}

			biomeData[x + y * size] = biome;
		}
	}

	std::cout << "Terrain #################:" << std::endl;
	std::cout << "Heighest Point: " << heighestPoint << "m" << std::endl;
	std::cout << "Lowest Point: " << lowestPoint << "m" << std::endl;
	std::cout << "Ocean Area: " << oceanCount / float(size * size) << "%" << std::endl;
	std::cout << "Land Area: " << landCount / float(size * size) << "%" << std::endl;
	std::cout << "Mountainous Land Area: " << mountainCount / float(landCount) << "%" << std::endl;
	std::cout << "Temperature #################:" << std::endl;
	std::cout << "Heighest Temperature: " << heighestTemperature << "C" << std::endl;
	std::cout << "Lowest Temperature: " << lowestTemperature << "C" << std::endl;

	std::cout << "Saving Map..." << std::endl;

	FILE* file;
	fopen_s(&file, "data/map/height.bin", "wb");
	fwrite(heightData, sizeof(*heightData), size * size, file);
	fclose(file);

	fopen_s(&file, "data/map/humidity.bin", "wb");
	fwrite(humidityData, sizeof(*humidityData), size * size, file);
	fclose(file);

	fopen_s(&file, "data/map/temperature.bin", "wb");
	fwrite(temperatureData, sizeof(*temperatureData), size* size, file);
	fclose(file);

	fopen_s(&file, "data/map/biome.bin", "wb");
	fwrite(biomeData, sizeof(*biomeData), size * size, file);
	fclose(file);
}