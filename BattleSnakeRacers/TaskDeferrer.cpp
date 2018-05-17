#include "TaskDeferrer.h"

#include "Clock.h"

#include <vector>
#include <utility>

struct Alarm {
	float timeRemaining;
	std::function<void()> task;
	bool inRealTime;
};

std::vector<Alarm> alarmList;

void TaskDeferrer::after(float seconds, std::function<void()> task, bool inRealTime)
{
	alarmList.push_back({ seconds, task, inRealTime });
}

void TaskDeferrer::update()
{
	std::vector<Alarm>::iterator it = alarmList.begin();
	while (it != alarmList.end()) {
		Alarm& alarm = *it;

		// Alarm expired
		if (alarm.timeRemaining <= 0) {
			// Perform task
			alarm.task();

			// Pop alarm, goto next alarm
			it = alarmList.erase(it);
		}
		else {
			// Count down alarm
			if (alarm.inRealTime)
				alarm.timeRemaining -= Clock::getUIDeltaTime();
			else
				alarm.timeRemaining -= Clock::getDeltaTime();

			// Goto next alarm
			++it;
		}
	}
}

