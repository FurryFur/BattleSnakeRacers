#include "Screen.h"

#include "TextLabel.h"

void Screen::update()
{
	for (auto& system : m_activeSystems) {
		system->beginFrame();
	}

	for (auto& system : m_activeSystems) {
		system->update();
	}

	for (auto& system : m_activeSystems) {
		system->endFrame();
	}
}

void Screen::createTextLabel(std::string labelText, glm::vec2 position, std::vector<TextLabel>* screen, float scale)
{
	TextLabel label(labelText, "Assets/Fonts/NYCTALOPIATILT.TTF");
	label.setScale(scale);
	label.setPosition(position);
	label.setColor(glm::vec3(0.8f, 0.8f, 0.8f));
	screen->push_back(label);
}
