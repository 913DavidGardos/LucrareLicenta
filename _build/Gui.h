#pragma once
#include "raylib.h"
#include "ParticleManager.h"
#include <chrono>
#include <string>
#include <iostream>
#define MAX_OPTIONS 5
#define ALGO_OPTIONS 3
#define MAX_INPUT_LENGTH 5

enum class ProgramState 
{
	MainMenu,
	Simulation,
	Paused,
	AlgoMenu,
	Lines,
	VelocityOfParticles,
	NumberOfParticles,
	ExitToDesktop
};

class Gui
{
public:
	Gui(ParticleManager& pm);
	~Gui();
	void Run();

	void takeInputNumberOfParticles();

	void drawTextBoxNr();

	void drawTextBox();

	void takeInputVelocity();

	void updateAlgoOptions();

	void drawAlgoOptions();

	void checkInput();

	void updateMenuOptions();

	void drawMenuOptions();

	void drawMenuButton();



private:
	void update();
	void draw();

	int screenWidth;
	int screenHeight;
	bool isPaused;
	ProgramState programState;
	ParticleManager& pm;
	double previousTime;
	double currentTime;

	const char* options[MAX_OPTIONS] = {
	  "Algorithms",
	  "Lines On / Off",
	  "Velocity of Particles",
	  "Number of Particles",
	  "Exit to Desktop"
	};
	
	const char* algoOptions[ALGO_OPTIONS] = {
	  "Quad Tree",
	  "Grid",
	  "Bounding Volume"
	};

	int selectedOption = -1;
	int keyOption = -1;
	int mouseOption = -1;
	Rectangle optionRectangles[MAX_OPTIONS];
	Rectangle algoRectangles[ALGO_OPTIONS];
	Rectangle buttonRectangle = { 20, screenHeight - 70.f, 120, 50 };

	std::string inputVelocityBox;
	std::string inputNumberBox;
	bool inputVelocity = false;
	bool inputN = false;
};

