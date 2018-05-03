#pragma once

#include "Scene.h"
#include "System.h"

#include <vector>
#include <memory>

class Screen
{
public:
	virtual ~Screen() {};
	Screen(const Screen&) = delete;
	Screen& operator=(const Screen&) = delete;

	virtual void update();

	void createTextLabel(std::string labelText, glm::vec2 position, std::vector<TextLabel>* screen, float scale = 1.0f);

protected:
	Screen() {};

	Scene m_scene;
	std::vector<std::unique_ptr<System>> m_activeSystems;
};

