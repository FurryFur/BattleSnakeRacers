#include "Entity.h"

#include "EntityEventListener.h"

#include <glm\glm.hpp>

#include "Log.h"

Entity::Entity(std::vector<EntityEventListener*>& eventListeners)
	: m_componentMask{ 0 }
	, m_eventListeners{ eventListeners }
{
}

void Entity::destroy()
{
	triggerAboutToRemoveComponentsEvent(m_componentMask);

	m_componentMask = 0;
}

bool Entity::operator==(const Entity& rhs) const
{
	return this == &rhs;
}

bool Entity::hasComponents(size_t componentMask) const
{
	return (m_componentMask & componentMask) == componentMask;
}

bool Entity::hasComponentsAny(size_t componentMask) const
{
	return (m_componentMask & componentMask) > 0;
}

bool Entity::hasComponents() const
{
	return m_componentMask != 0;
}

bool Entity::matches(size_t lhsComponentMask, size_t rhsComponentMask)
{
	return (lhsComponentMask & rhsComponentMask) == rhsComponentMask;
}

bool Entity::matchesAny(size_t lhsComponentMask, size_t rhsComponentMask)
{
	return (lhsComponentMask & rhsComponentMask) > 0;
}

void Entity::triggerAddedComponentsEvent(size_t componentMask)
{
	for (auto eventListener : m_eventListeners) {
		eventListener->onAddedComponents(*this, componentMask);
	}
}

void Entity::triggerAboutToRemoveComponentsEvent(size_t componentMask)
{
	for (auto eventListener : m_eventListeners) {
		eventListener->onBeforeRemoveComponents(*this, componentMask);
	}
}

void Entity::addComponents(size_t componentMask)
{
	m_componentMask |= componentMask;

	if (matches(componentMask, COMPONENT_TRANSFORM)) {
		transform = {};
	}
	if (matches(componentMask, COMPONENT_PHYSICS)) {
		physics = {};
	}
	if (matches(componentMask, COMPONENT_MODEL)) {
		model = {};
	}
	if (matches(componentMask, COMPONENT_CAMERA)) {
		camera = {};
	}
	if (matches(componentMask, COMPONENT_PLAYER_CONTROL)) {
		controlVars = {};
	}
	if (matches(componentMask, COMPONENT_PLAYER)) {
		
	}
	if (matches(componentMask, COMPONENT_INPUT)) {
		input = {};
	}
	if (matches(componentMask, COMPONENT_INPUT_MAP)) {
		inputMap = {};
	}

	triggerAddedComponentsEvent(componentMask);
}

void Entity::removeComponents(size_t componentMask)
{
	triggerAboutToRemoveComponentsEvent(componentMask);
	m_componentMask &= (~componentMask);
}

size_t Entity::assembleComponentMask(size_t componentMask)
{
	return componentMask;
}
