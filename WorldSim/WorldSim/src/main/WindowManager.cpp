#include <format>

#include "Global.h"
#include "WindowManager.h"

WindowManager::WindowManager() {
	createWindow();

	camera = new Camera();

	std::cout << "Init Window Manager" << std::endl;
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		keys[i] = false;
	}

	for (int i = 0; i < 3; i++) {
		buttons[i] = false;
	}
}

WindowManager::~WindowManager() {
	delete camera;
}

bool WindowManager::isKeyDown(int key) {
	return glfwGetKey(window, key);
}

bool WindowManager::isKeyPressed(int key) {
	return isKeyDown(key) && !keys[key];
}

bool WindowManager::isKeyReleased(int key) {
	return !isKeyDown(key) && keys[key];
}

bool WindowManager::isMouseButtonDown(int button) {
	return glfwGetMouseButton(window, button);
}

bool WindowManager::isMouseButtonPressed(int button) {
	return isMouseButtonDown(button) && !buttons[button];
}

bool WindowManager::isMouseButtonReleased(int button) {
	return !isMouseButtonDown(button) && buttons[button];
}

glm::vec2 WindowManager::getMousePosition() {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	return glm::vec2(xpos, ypos);
}

glm::vec2 WindowManager::getMouseWorldPosition() {
	glm::vec2 mousePos = getMousePosition() / glm::vec2(getWindowWidth(), getWindowHeight()) - 0.5f;

	return (glm::vec2(getWindowWidth() / camera->getZoom() * mousePos.x,
		-getWindowHeight() / camera->getZoom() * mousePos.y) / getContentScale() + camera->getPosition());
}

bool WindowManager::onScreen(const glm::vec2& pos) {
	glm::vec2 dist = { (camera->getPosition().x - pos.x) * camera->getZoom(),
					   (camera->getPosition().y - pos.y) * camera->getZoom() };

	int buffer = 128;

	if (dist.x > (getWindowWidth() / 2) + buffer || dist.y > (getWindowHeight() / 2) + buffer ||
		dist.x < (-getWindowWidth() / 2) - buffer || dist.y < (-getWindowHeight() / 2) - buffer) {
		return false;
	}
	return true;
}

void WindowManager::updateWindow() {
	updateWindowFunctions();
	updateInput(); // Must be last
}

void WindowManager::updateWindowFunctions() {
	if (isKeyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (isKeyPressed(GLFW_KEY_F11)) {
		if (!fullscreen) {
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			fullscreen = true;
		}
		else {
			glfwSetWindowMonitor(window, NULL, (mode->width / 2) - window_width / 2, (mode->height / 2) - window_height / 2,
				window_width, window_height, mode->refreshRate);
			fullscreen = false;
		}
	}

	if (!fullscreen) {
		glfwGetWindowSize(window, &window_width, &window_height);
	}
}

void WindowManager::updateInput() {
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		keys[i] = isKeyDown(i);
	}

	for (int i = 0; i < 3; i++) {
		buttons[i] = isMouseButtonDown(i);
	}
}

void updateViewport(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLFWwindow* WindowManager::createWindow() {
	if (!glfwInit()) {
		return nullptr;
	}

	monitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	startup_width = window_width;

	window = glfwCreateWindow(window_width, window_height,
		std::format("{} v{}", TITLE, VERSION).c_str(), NULL, NULL);

	if (!window) {
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "GLEW init failed!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	glfwSetFramebufferSizeCallback(window, updateViewport);

	return window;
}

GLFWwindow* WindowManager::getWindow() {
	return window;
}

int WindowManager::getWindowWidth() {
	return fullscreen ? mode->width : window_width;
}

int WindowManager::getWindowHeight() {
	return fullscreen ? mode->height : window_height;
}

float WindowManager::getContentScale() {
	return (fullscreen ? (float)mode->width : (float)window_width) / (float)startup_width;
}