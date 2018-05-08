#pragma once

struct GLFWwindow;

class Entity;

namespace Game {
	void init();
	GLFWwindow* getWindowContext();
	void preloadModelsAndTextures();
	void executeOneFrame();
	void killPlayer(Entity & player);
	void resetPlayer(Entity & player);
}
