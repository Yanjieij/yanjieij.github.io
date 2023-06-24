#pragma once

#define UP 1
#define DOWN 0

struct request
{
	int passengerNum = -1;
	int curFloor = -1;
	int targetFloor = -1;
	bool status = UP;

	int duration_wait = 0;
	int duration_take = 0;
};