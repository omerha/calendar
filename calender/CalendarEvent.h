#pragma once
#include <time.h>
#include <string>
#include <iostream>
using namespace std;
class CalendarEvent
{
	time_t startTime; // time_t is defined in the header file <ctime>
	time_t duration; // duration of the event, in seconds
	string description; // should not contain special characters or newline
public:
	CalendarEvent() {
		startTime = NULL;
		duration = NULL;
	};
	CalendarEvent(time_t sTime, time_t dTime, string desc){
		startTime = sTime;
		duration = dTime;
		description = desc;
	}
	time_t getStartTime() { return startTime; }
	time_t getDuration() { return duration; }
	// constructor, destructor, other methods as needed
	void print(); // "print" the event to cout
};