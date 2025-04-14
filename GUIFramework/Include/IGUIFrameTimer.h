#ifndef _IGUIFRAMETIMER_H_
#define _IGUIFRAMETIMER_H_

#include "IGUIFrameIncludes.h"

class CIGUIFrameTimer
{
public:
	virtual ~CIGUIFrameTimer(void);
	virtual u32 getTicks() = 0;
	virtual void initTimer() = 0;
	virtual void deinitTimer() = 0;
	CIGUIFrameTimer(void);
};

#endif //_IGUIFRAMETIMER_H_

