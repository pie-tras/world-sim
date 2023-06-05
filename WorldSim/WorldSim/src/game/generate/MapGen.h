#pragma once

#include "Global.h"

class MapGen {
private:
	int size;
	int amplitude;

	int* heightData; //-15000 - 15000 [m]
	double* humidityData; // 0.0 - 1.0
	double* temperatureData; // -100 ~ 100 [C]
	
	int* biomeData;

public:
	MapGen(int size, int amplitude);
	~MapGen();
	void generateMap();
};