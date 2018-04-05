#include "CameraSystem.h"

CameraSystem::CameraSystem(Scene& scene)
	: System(scene)
{
}


CameraSystem::~CameraSystem()
{
}

void CameraSystem::update(Entity& entity)
{
	if (!entity.hasComponents(COMPONENT_CAMERA))
		return;


}

void CameraSystem::beginFrame()
{
}

void CameraSystem::endFrame()
{
}
