#include "include.h"

//TODO: Create more advanced collision detection
//TODO: Create more advanced collision response

//TODO: Add Functions to implement different Scenes
//TODO: Add ability to choose scene at start

int main()
{
    Scene scene(SceneChooser::TEST_SCENE_0);

    scene.mainLoop();

    return EXIT_SUCCESS;
}
