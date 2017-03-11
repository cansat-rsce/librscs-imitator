/*
 * timeservice.cpp
 *
 *  Created on: 05 марта 2017 г.
 *      Author: snork
 */

#include <chrono>

extern "C" {
	#include "../timeservice.h"
}

typedef std::chrono::steady_clock _clock_t;

static _clock_t::time_point _start;
static bool _started = false;


void rscs_time_init()
{
	if (_started)
		return;

	_start = _clock_t::now();
	_started = true;
}


void rscs_time_deinit()
{
	_started = false;
}


uint32_t rscs_time_get()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(_clock_t::now() - _start).count();
}
