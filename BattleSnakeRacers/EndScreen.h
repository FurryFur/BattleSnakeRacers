#pragma once

#include "Screen.h"

class EndScreen : public Screen
{
public:
	EndScreen(std::string);
	~EndScreen() override;
	void buttonPressed() override;
	void update() override;
private:
	bool buttonReleasedOnce;
};