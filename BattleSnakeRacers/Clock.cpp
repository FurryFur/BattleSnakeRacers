//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School.
//
// File Name	: clock.cpp
// Description	: The clock for keeping time
// Author		: Lance Chaney
// Mail			: lance.cha7337@mediadesign.school.nz
//

#include "Clock.h"

#include <GLFW\glfw3.h>

double g_timeDilation = 1;
double g_frameStartTime;
double g_deltaTime;
double g_uiDeltaTime;

void Clock::update()
{
	static double s_lastFrameTime = glfwGetTime();

	g_frameStartTime = glfwGetTime();
	g_deltaTime = g_frameStartTime - s_lastFrameTime;
	g_uiDeltaTime = g_deltaTime;
	g_deltaTime *= g_timeDilation;
	s_lastFrameTime = g_frameStartTime;
}

float Clock::getTime()
{
	return g_frameStartTime;
}

float Clock::getDeltaTime()
{
	return static_cast<float>(g_deltaTime);
}

float Clock::getUIDeltaTime()
{
	return static_cast<float>(g_uiDeltaTime);
}

void Clock::setTimeDilation(double timeDilation)
{
	g_timeDilation = timeDilation;
}
