//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2017 Media Design School
//
// Description  : Per entity movement settings.
// Author       : Lance Chaney Jack Mair
// Mail         : lance.cha7337@mediadesign.school.nz
//

#pragma once

struct ControlComponent {
	ControlComponent();
	float maxAcceleration;    // This sets the maximum turning / acceleration speed
	float accelerationWeight; // This scales how fast the entity will turn / accelerate
	float maxMoveSpeed;
};