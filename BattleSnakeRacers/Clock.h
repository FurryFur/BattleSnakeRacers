//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School.
//
// File Name	: Clock.h
// Description	: Header file for the clock
// Author		: Lance Chaney
// Mail			: lance.cha7337@mediadesign.school.nz
//

#pragma once

namespace Clock {
	void update();
	float getTime();
	float getDeltaTime();
	float getUIDeltaTime();
	void setTimeDilation(double timeDilation);
}
