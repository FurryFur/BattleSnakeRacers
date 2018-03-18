//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : A system which handles movement based on an
//                entities input state.
// Author       : Lance Chaney
// Mail         : lance.cha7337@mediadesign.school.nz
//

#define _USE_MATH_DEFINES

#include "PlayerControlSystem.h"

#include "ControlComponent.h"
#include "EngineEntityDefs.h"
#include "GLUtils.h"
#include "GLMUtils.h"
#include "Scene.h"
#include "Entity.h"
#include "Clock.h"
#include "Utils.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <cmath>

PlayerControlSystem::PlayerControlSystem(Scene& scene)
	: m_scene{ scene }
{
}

void PlayerControlSystem::update(Entity& entity)
{
	// Filter movable
	const size_t kMovableMask = COMPONENT_PLAYER_CONTROL | COMPONENT_INPUT | COMPONENT_TRANSFORM;
	if (!entity.hasComponents(kMovableMask))
		return;
}