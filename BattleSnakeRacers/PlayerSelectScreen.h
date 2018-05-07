#pragma once

#include "Screen.h"
#include <vector>

class Entity;
class PlayerSelectScreen : public Screen
{
public:
	PlayerSelectScreen();
	~PlayerSelectScreen() override;
	void buttonPressed() override;
	void checkControllerInput();
	void checkPlayerReadyInput();
	bool checkEnoughPlayersJoined();
	void update() override;

	TextLabel createUniqueTextLabel(std::string labelText, glm::vec2 position, float scale = 1.0f, glm::vec3 color = glm::vec3(0.8f, 0.8f, 0.8f));

private:
	TextLabel m_centreText;
	TextLabel m_p1Ready;
	TextLabel m_p2PressStart;
	TextLabel m_p3PressStart;
	TextLabel m_p4PressStart;

	bool m_bGameCanStart;
};

