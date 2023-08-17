#pragma once

#include "include.h"

enum class SceneChooser {
    TEST_COLLISION,
    TEST_SCENE,
    SCENE_0,
    SCENE_1,
    SCENE_2
};

enum class SceneState {
    SIM_CHOOSE,
    SIM,
    SIM_PAUSE
};

class Scene {
    enum SceneState sceneState;

    enum SceneChooser sceneChooser;

    sf::RenderWindow * pWindow;
    sf::ContextSettings * pSettings;

    sf::RectangleShape * pBackground;

    sf::Clock * pDeltaClock;
    sf::Time * pDeltaTime;

    sf::Font * pFont;
    sf::Text * pSimulationText;
    sf::Text * pSimulationControls;

    int fpsCounter = 0;

    std::mt19937 * pMt;
    std::random_device rd;
    std::uniform_int_distribution<int> randColor;
    std::uniform_real_distribution<float> randVel;
    std::uniform_real_distribution<float> randPosX;
    std::uniform_real_distribution<float> randPosY;

    std::vector<GravitySource> gravitySources;
    std::vector<Particle> particles;

public:
    Scene();
    ~Scene();

    void run();
    void update();

    void eventAction();

    void sceneInit();

    void sceneChooserRender();
    void simulationRender();
};
