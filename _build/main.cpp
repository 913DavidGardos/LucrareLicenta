#include<iostream>
#include "raylib.h"
#include "ParticleManager.h"
#include "Gui.h"
#include <vector>
#include <chrono>
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1000

int main()
{
	ParticleManager pm(SCREEN_WIDTH, SCREEN_HEIGHT);
	pm.InitParticles(100);

	Gui gui(pm);
	gui.Run();


	return 0;
}