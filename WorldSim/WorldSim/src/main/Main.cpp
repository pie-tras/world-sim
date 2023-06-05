#include "Global.h"

#include "WindowManager.h"
#include "render/RenderManager.h"
#include "game/GameStateManager.h"
#include "main/FPSManager.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#else

#define _CHECKLEAKS

#endif

void cycle(GameStateManager* gsm, GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    gsm->update();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main(void) {

    int* testing = new int[256];

    WindowManager* win_mgr = new WindowManager();
    GLFWwindow* window = win_mgr->getWindow();

    if (!window) {
        delete win_mgr;
        std::cout << "Window Failed to Load!" << std::endl;
        return -1;
    }

    RenderManager* renderer = new RenderManager(win_mgr);
    GameStateManager* gsm = new GameStateManager(win_mgr, renderer);
    FPSManager* fps_mgr = new FPSManager(60, true);

    gsm->setActiveState(States::SPLASH);

    while (!glfwWindowShouldClose(window)) {
        fps_mgr->runCycle(cycle, gsm, window);
    }

    glfwTerminate();
    delete win_mgr;
    delete renderer;
    delete gsm;
    delete fps_mgr;

    delete[] testing;

    if (_CrtDumpMemoryLeaks() != 0) {
        std::cout << "MEMORY LEAK FOUND! See output for details." << std::endl;
    }
    
    return 0;
}