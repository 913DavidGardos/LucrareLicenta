#include "Gui.h"

Gui::Gui(ParticleManager& pm) : programState(ProgramState::Simulation), pm(pm), screenWidth(pm.getScreenWidth()), screenHeight(pm.getScreenHeight()), isPaused(false)
{
    InitWindow(screenWidth, screenHeight, "Particle simulator");
    //SetWindowState(FLAG_VSYNC_HINT);

    pm.InitParticles(100);

    previousTime = 0;
    currentTime = 0;

    for (int i = 0; i < MAX_OPTIONS; i++)
    {
        optionRectangles[i] = { (float)screenWidth / 2 - 100, 200.f + 60 * i, 200, 50 };
    }

    for (int i = 0; i < ALGO_OPTIONS; i++)
    {
        algoRectangles[i] = { (float)screenWidth / 2 - 100, 200.f + 60 * i, 200, 50 };
    }
}

Gui::~Gui()
{
    CloseWindow();
}

void Gui::Run()
{
    while (!WindowShouldClose()) 
    {
        drawMenuButton();
        checkInput();
        update();
        draw();

        if (programState == ProgramState::MainMenu)
        {
            updateMenuOptions();
            drawMenuOptions();
        }
        else if (programState == ProgramState::Simulation)
        {

        }
        else if (programState == ProgramState::Paused)
        {
        
        }
        else if (programState == ProgramState::AlgoMenu)
        {
            updateAlgoOptions();
            drawAlgoOptions();
        }
        else if (programState == ProgramState::Lines)
        {
            pm.toggleLines();
            programState = ProgramState::MainMenu;
        }
        else if (programState == ProgramState::VelocityOfParticles)
        {
            takeInputVelocity();
            drawTextBox();
        }
        else if (programState == ProgramState::NumberOfParticles)
        {
            takeInputNumberOfParticles();
            drawTextBoxNr();
        }
        else if (programState == ProgramState::ExitToDesktop)
        {
            return;
        }
    }
}

void Gui::takeInputNumberOfParticles()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Check if the mouse clicked inside the input box
        if (CheckCollisionPointRec(GetMousePosition(), { (float)screenWidth / 2 - 100, 200.f + 60 * 3, 200, 40 }))
        {
            inputN = true;
        }
        else
        {
            inputN = false;
        }
    }

    // Handle keyboard input if the input box is selected
    if (inputN)
    {
        int key = GetKeyPressed();
        std::array<KeyboardKey, 10> preferedKeys =
        {
            KEY_ONE,   KEY_TWO,
            KEY_THREE,  KEY_FOUR,  KEY_FIVE,
            KEY_SIX,    KEY_SEVEN, KEY_EIGHT,
            KEY_NINE,   KEY_ZERO
        };

        for (int i = 0; i < preferedKeys.size(); i++)
            if (key == preferedKeys[i])
            {
                // Process the key unless it's the backspace key
                if (key != KEY_BACKSPACE && inputNumberBox.length() < MAX_INPUT_LENGTH)
                {
                    inputNumberBox += static_cast<char>(key);
                }
                else if (key == KEY_BACKSPACE && !inputNumberBox.empty())
                {
                    inputNumberBox.pop_back();
                }
            }
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        int nParticles = 0;
        try
        {
            nParticles = std::stoi(inputNumberBox);
            std::cout << "int value: " << nParticles << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "Error converting string to float: " << e.what() << std::endl;
        }
        if (nParticles > 100000)
        {
            std::cout << "Let's not get over 100 000 for now\n";
            inputNumberBox.clear();
            return;
        }

        pm.updateNumberOfParticles(nParticles);
        inputNumberBox.clear();
        inputN = false;
        programState = ProgramState::MainMenu;
    }
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        inputNumberBox.pop_back();
    }


}

void Gui::drawTextBoxNr()
{
    DrawRectangle((float)screenWidth / 2 - 100, 200.f + 60 * 3, 200, 50, RED);
    DrawText(inputNumberBox.c_str(), (float)screenWidth / 2 - 100 + 10, 200.f + 60 * 3 + 10, 20, BLACK);
}

void Gui::drawTextBox()
{
    DrawRectangle( (float)screenWidth / 2 - 100, 200.f + 60 * 2, 200, 50, SKYBLUE);
    DrawText(inputVelocityBox.c_str(), (float)screenWidth / 2 - 100 + 10, 200.f + 60 * 2 + 10, 20, BLACK);
}

void Gui::takeInputVelocity()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Check if the mouse clicked inside the input box
        if (CheckCollisionPointRec(GetMousePosition(), { (float)screenWidth / 2 - 100, 200.f + 60 * 2, 200, 40 }))
        {
            inputVelocity = true;
        }
        else
        {
            inputVelocity = false;
        }
    }

    // Handle keyboard input if the input box is selected
    if (inputVelocity)
    {
        int key = GetKeyPressed();
        std::array<KeyboardKey, 11> preferedKeys = 
        { 
            KEY_PERIOD, KEY_ONE,   KEY_TWO,
            KEY_THREE,  KEY_FOUR,  KEY_FIVE,
            KEY_SIX,    KEY_SEVEN, KEY_EIGHT, 
            KEY_NINE,   KEY_ZERO
        };

        for(int i = 0; i < preferedKeys.size(); i++)
            if (key == preferedKeys[i])
            {
                // Process the key unless it's the backspace key
                if (key != KEY_BACKSPACE && inputVelocityBox.length() < MAX_INPUT_LENGTH)
                {
                    inputVelocityBox += static_cast<char>(key);
                }
                else if (key == KEY_BACKSPACE && !inputVelocityBox.empty())
                {
                    inputVelocityBox.pop_back();
                }
            }
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        float newVelocity = 0.f;
        try 
        {
            newVelocity = std::stof(inputVelocityBox);
            std::cout << "Float value: " << newVelocity << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout << "Error converting string to float: " << e.what() << std::endl;
        }
        if (newVelocity > 5.f)
        {
            std::cout << "Let's not multiply by 5 or over\n";
            inputVelocityBox.clear();
            return;
        }

        pm.updateParticleVelocity(newVelocity);
        inputVelocityBox.clear();
        inputVelocity = false;
        programState = ProgramState::MainMenu;
    }
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        inputVelocityBox.pop_back();
    }
}

void Gui::updateAlgoOptions()
{
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        keyOption++;
        if (keyOption >= ALGO_OPTIONS)
            keyOption = 0;
    }
    else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        keyOption--;
        if (keyOption < 0)
            keyOption = ALGO_OPTIONS - 1;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        selectedOption = keyOption;
    }

    mouseOption = -1;
    Vector2 mousePos = GetMousePosition();
    for (int i = 0; i < ALGO_OPTIONS; i++)
    {
        if (CheckCollisionPointRec(mousePos, algoRectangles[i]))
        {
            mouseOption = i;
            break;
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOption != -1)
    {
        selectedOption = mouseOption;
    }

    // act on the selected option
    if (selectedOption == 0)
    {
        // QuadTree
        pm.startQuadTree();
        selectedOption = -1;
    }
    else if (selectedOption == 1)
    {
        // Grid
        pm.startGrid();
        selectedOption = -1;
    }
    else if (selectedOption == 2)
    {
        // Bounding Volume
        pm.startBoundingVolume();
        selectedOption = -1;
    }
}

void Gui::drawAlgoOptions()
{
    for (int i = 0; i < ALGO_OPTIONS; i++)
    {
        if (i == selectedOption)
            DrawRectangleRec(algoRectangles[i], DARKGRAY);
        else if (i == mouseOption || i == keyOption)
            DrawRectangleRec(algoRectangles[i], GRAY);
        else
            DrawRectangleRec(algoRectangles[i], LIGHTGRAY);

        DrawText(algoOptions[i], algoRectangles[i].x + 10, algoRectangles[i].y + 15, 20, BLACK);
    }
}

void Gui::checkInput()
{
    if (IsKeyPressed(KEY_TAB))
    {
        if (programState == ProgramState::MainMenu)
            programState = ProgramState::Simulation;
        else        
        programState = ProgramState::MainMenu;
    }

    Vector2 mousePos = GetMousePosition();
    if (CheckCollisionPointRec(mousePos, buttonRectangle))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            if (programState == ProgramState::MainMenu)
                programState = ProgramState::Simulation;
            else
                programState = ProgramState::MainMenu;
        }
    }

    // update state from selected Option
    if (selectedOption == 0)
    {
        programState = ProgramState::AlgoMenu;
        selectedOption = -1;
    } 
    else if (selectedOption == 1)
    {
        programState = ProgramState::Lines;
        selectedOption = -1;
    }
    else if (selectedOption == 2)
    {
        programState = ProgramState::VelocityOfParticles;
        selectedOption = -1;
    }
    else if (selectedOption == 3)
    {
        programState = ProgramState::NumberOfParticles;
        selectedOption = -1;
    }
    else if (selectedOption == 4)
    {
        programState = ProgramState::ExitToDesktop;
        selectedOption = -1;
    }
}

void Gui::updateMenuOptions()
{
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        keyOption++;
        if (keyOption >= MAX_OPTIONS)
            keyOption = 0;
    }
    else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        keyOption--;
        if (keyOption < 0)
            keyOption = MAX_OPTIONS - 1;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        selectedOption = keyOption;
        keyOption = -1;
    }

    mouseOption = -1;
    Vector2 mousePos = GetMousePosition();
    for (int i = 0; i < MAX_OPTIONS; i++)
    {
        if (CheckCollisionPointRec(mousePos, optionRectangles[i]))
        {
            mouseOption = i;
            break;
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOption != -1)
    {
        selectedOption = mouseOption;
        keyOption = -1;
    }   
}

void Gui::drawMenuOptions()
{
    for (int i = 0; i < 5; i++)
    {
        if (i == selectedOption)
            DrawRectangleRec(optionRectangles[i], DARKGRAY);
        else if (i == mouseOption || i == keyOption)
            DrawRectangleRec(optionRectangles[i], GRAY);
        else
            DrawRectangleRec(optionRectangles[i], LIGHTGRAY);

        DrawText(options[i], optionRectangles[i].x + 10, optionRectangles[i].y + 15, 20, BLACK);
    }
}

void Gui::drawMenuButton()
{
    DrawRectangleRec(buttonRectangle, BLUE);
    DrawText("Menu", buttonRectangle.x + 10, buttonRectangle.y + 15, 20, WHITE);
}

void Gui::update() {
    if (!isPaused) {
        previousTime = currentTime;
        currentTime = GetTime();
        float deltaT = (float)(currentTime - previousTime);
        deltaT = deltaT * 10.f;

        if (deltaT > 0.15f)
            deltaT = 0.15f;

        pm.updateParticles(deltaT);
    }

    if (IsKeyPressed(KEY_SPACE)) 
    {
        isPaused = !isPaused;
        programState = ProgramState::Paused;
    }
}

void Gui::draw() {
    BeginDrawing();

    ClearBackground(WHITE);
    DrawFPS(10, 10);
    pm.drawParticles();

    if (isPaused) {
        DrawText("Simulation Paused", screenWidth / 2 - MeasureText("Simulation Paused", 40) / 2, 0 + 40, 40, GRAY);
    }

    EndDrawing();
}