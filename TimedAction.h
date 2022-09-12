// TimedAction.h

#ifndef _TIMEDACTION_h
#define _TIMEDACTION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class TimedActionClass
{
 protected:


 public:
	void init();
};

extern TimedActionClass TimedAction;

#endif

