#pragma once

#include <functional>

namespace TaskDeferrer {
	void after(float seconds, std::function<void()> task, bool inRealTime = true);

	void update();
}
