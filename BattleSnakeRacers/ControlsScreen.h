#pragma once

#include "Screen.h"
#include "TextLabel.h"
#include <vector>
class Entity;
class ControlsScreen : public Screen
{
public:
	ControlsScreen();
	~ControlsScreen() override;
	void buttonPressed() override;

private:
	Entity* m_controlDiagram;
	TransformComponent controlsTransform{};
};

