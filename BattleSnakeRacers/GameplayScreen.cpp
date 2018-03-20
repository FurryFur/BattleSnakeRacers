#include "GameplayScreen.h"

#include "InputSystem.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "MovementSystem.h"


GameplayScreen::GameplayScreen()
{
	m_activeSystems.push_back(std::make_unique<InputSystem>(m_scene));
	m_activeSystems.push_back(std::make_unique<PhysicsSystem>(m_scene));
	m_activeSystems.push_back(std::make_unique<RenderSystem>(m_scene));
	m_activeSystems.push_back(std::make_unique<MovementSystem>(m_scene));
}


GameplayScreen::~GameplayScreen()
{
}
