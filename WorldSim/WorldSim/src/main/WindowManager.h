#pragma once

#include <string>

#include "Global.h"
#include "game/Camera.h"

class WindowManager {
private:
    GLFWwindow* window;
    GLFWmonitor* monitor;
    const GLFWvidmode* mode;

    int window_width = 800;
    int window_height = 600;
    const std::string TITLE = "World Simulator";
    const double VERSION = 0.1;

    bool fullscreen = false;
    int startup_width;
    double content_scale;

    bool keys[GLFW_KEY_LAST];
    bool buttons[3];

    void updateInput();
    void updateWindowFunctions();
public:
    Camera* camera;

    WindowManager();
    ~WindowManager();

    bool isKeyDown(int key);
    bool isKeyPressed(int key);
    bool isKeyReleased(int key);

    bool isMouseButtonDown(int button);
    bool isMouseButtonPressed(int button);
    bool isMouseButtonReleased(int button);

    glm::vec2 getMousePosition();
    glm::vec2 getMouseWorldPosition();

    bool onScreen(const glm::vec2& pos);

    void updateWindow();

    GLFWwindow* createWindow();
    GLFWwindow* getWindow();

    int getWindowWidth();
    int getWindowHeight();
    float getContentScale();
};