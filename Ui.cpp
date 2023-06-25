#include "Ui.h"
#include "Ui.h"

Ui::Ui(ParticleManager& pm) : pm(pm)
{
    screenWidth = pm.getScreenWidth();
    screenHeight = pm.getScreenHeight();
}

std::vector<std::string> Ui::tokenizeString(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

void Ui::quadTreeCommands(std::vector<std::string>& tokens)
{
    if (tokens.size() == 2)
    {
        pm.startQuadTree();
        int number = std::stoi(tokens[1]);
        pm.updateNumberOfParticles(number);
    }
    if (tokens.size() == 3)
    {
        if (tokens[1] == "velocity" || tokens[1] == "speed")
        {
            float number = std::stof(tokens[2]);
            pm.updateParticleVelocity(number);
        }
    }
}

void Ui::bvhCommands(std::vector<std::string>& tokens)
{
    if (tokens.size() == 2)
    {
        pm.startBoundingVolume();
        int number = std::stoi(tokens[1]);
        pm.updateNumberOfParticles(number);
    }
    if (tokens.size() == 3)
    {
        if (tokens[1] == "velocity" || tokens[1] == "speed")
        {
            float number = std::stof(tokens[2]);
            pm.updateParticleVelocity(number);
        }
    }
}

void Ui::gridCommands(std::vector<std::string>& tokens)
{
    if (tokens.size() == 2)
    {
        pm.startGrid();
        int number = std::stoi(tokens[1]);
        pm.updateNumberOfParticles(number);
    }
    if (tokens.size() == 3)
    {
        if (tokens[1] == "velocity" || tokens[1] == "speed")
        {
            float number = std::stof(tokens[2]);
            pm.updateParticleVelocity(number);
        }
    }
}

void Ui::helpCommands(std::vector<std::string>& tokens)
{
    std::cout << "help\n";
    std::cout << "quadtree/bvh/grid [number] - changes the number of particles\n";
    std::cout << "quadtree/bvh/grid velocity [number] - multiplies with the velocity of particles\n";
    std::cout << "exit - closes the program\n";
    std::cout << "start - start the simulation\n";
    std::cout << "gui - start the gui\n";
    std::cout << "test - start the simulation for all algorithms\n";
}

void Ui::startCommands(std::vector<std::string>& tokens)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    // Call the function you want to measure
    pm.updateParticles(0.15f);

    // Stop measuring time
    auto endTime = std::chrono::high_resolution_clock::now();

    // Calculate the duration in milliseconds
    auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Output the measured time
    std::cout << "Time taken: " << durationMs << " milliseconds" << std::endl;
}

void Ui::guiCommands(std::vector<std::string>& tokens)
{
    if (!gui)
        gui = std::make_unique<Gui>(pm);

    gui->Run();
}

void Ui::testCommands(std::vector<std::string>& tokens)
{
    for (int i = 1; i < 100; i++)
    {
        std::cout << "Test running for  " << i * 1000 << " elements \n";

        pm.InitParticles(i * 1000);
        //pm.startQuadTree();
        //pm.updateParticles(0.15f);

        pm.startBoundingVolume();
        pm.updateParticles(0.15f);

        //pm.startGrid();
        //pm.updateParticles(0.15f);
    }
}

void Ui::run()
{
    std::string command;

    while (true)
    {
        std::cout << "Enter a command: ";
        std::getline(std::cin, command);

        std::vector<std::string> tokens = tokenizeString(command, ' ');

        if (command == "quit" || command == "exit" || command == "bye")
        {
            break;  // Exit the loop and end the program
        }
        if (!tokens.empty())
        {
            if (tokens[0] == "quadtree" || tokens[0] == "qtree")
                quadTreeCommands(tokens);
            if (tokens[0] == "bvh" || tokens[0] == "boundingvolumehierarchy")
                bvhCommands(tokens);
            if (tokens[0] == "grid" || tokens[0] == "spatialhashing")
                gridCommands(tokens);
            if (tokens[0] == "help")
                helpCommands(tokens);
            if (tokens[0] == "start")
                startCommands(tokens);
            if (tokens[0] == "gui")
                guiCommands(tokens);
            if (tokens[0] == "test")
                testCommands(tokens);
        }
    }
}

