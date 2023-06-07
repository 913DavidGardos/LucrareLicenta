#pragma once
#include "ParticleManager.h"
#include "Gui.h"
#include <sstream>
#include <memory>

class Ui
{
public:
	Ui(ParticleManager& pm);
	std::vector<std::string> tokenizeString(const std::string& input, char delimiter);
	void quadTreeCommands(std::vector<std::string>& tokens);
	void bvhCommands(std::vector<std::string>& tokens);
	void gridCommands(std::vector<std::string>& tokens);
	void helpCommands(std::vector<std::string>& tokens);
	void update();
	void linesCommands(std::vector<std::string>& tokens);
	void startCommands(std::vector<std::string>& tokens);
	void guiCommands(std::vector<std::string>& tokens);
	void pauseCommands(std::vector<std::string>& tokens);
	void run();


private:
	ParticleManager& pm;
	int screenWidth, screenHeight;
	std::unique_ptr<Gui> gui;
	float previousTime, currentTime;
};

