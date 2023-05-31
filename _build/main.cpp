#include<iostream>
#include "raylib.h"
#include "ParticleManager.h"
#include <vector>
#include <chrono>
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particle simulator");
	//SetWindowState(FLAG_VSYNC_HINT);

	ParticleManager pm(SCREEN_WIDTH, SCREEN_HEIGHT);

	auto start_time = std::chrono::high_resolution_clock::now();

	pm.InitParticles(100);	

	auto end_time = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

	std::cout << "Execution time InitParticles 1000: " << duration.count() << " milliseconds" << std::endl;

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
		
		{
			auto start_time = std::chrono::high_resolution_clock::now();

			pm.drawParticles();
			pm.updateParticles(deltaT);

			auto end_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

			//printf("Duration of update Particle %d \n", duration.count());
		}

		DrawFPS(10, 10);
		EndDrawing();

	}
	 


	return 0;
}