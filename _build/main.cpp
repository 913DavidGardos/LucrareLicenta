#include<iostream>
#include "raylib.h"
#include "ParticleManager.h"
#include "Ui.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1000



int main()
{
	ParticleManager pm(SCREEN_WIDTH, SCREEN_HEIGHT);
	pm.InitParticles(100);

    Ui ui(pm);
	ui.run();


	return 0;
}