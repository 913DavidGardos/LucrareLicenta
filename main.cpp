#include<iostream>
#include "raylib.h"
#include "ParticleManager.h"
#include "Ui.h"
#include "MeasurementCollector.h"
#include "FileManager.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 900

int main()
{
	MeasurementCollector measureCollector;
	FileManager filemanager;

	ParticleManager pm(SCREEN_WIDTH, SCREEN_HEIGHT, measureCollector);
	pm.InitParticles(100);

	Ui ui(pm);
	ui.run();

	filemanager.storeToFile(measureCollector);

	return 0;
}