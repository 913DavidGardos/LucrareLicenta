#include<iostream>
#include "raylib.h"
#include "ParticleManager.h"
#include <vector>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particle simulator");
	SetWindowState(FLAG_VSYNC_HINT);

	ParticleManager pm(SCREEN_WIDTH, SCREEN_HEIGHT);
	pm.InitParticles(20);

	double previousTime = 0;
	double currentTime = 0;


	while (!WindowShouldClose())
	{
		previousTime = currentTime;
		currentTime = GetTime();
		float deltaT = (float)(currentTime - previousTime);
		deltaT = deltaT * 10.f;

		if (deltaT > 0.15f) deltaT = 0.15f;
		
		BeginDrawing();
		ClearBackground(WHITE);
		
		pm.updateParticles(deltaT);
		pm.drawParticles();

		DrawFPS(10, 10);
		EndDrawing();
	}
	 


	return 0;
}