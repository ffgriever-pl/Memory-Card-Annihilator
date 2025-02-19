#ifndef _GUIFRAMETIMERPS2_H_
#define _GUIFRAMETIMERPS2_H_ 1

#include "GUIFramePS2Includes.h"
#include "IGUIFrameTimer.h"
class CGUIFrameTimerPS2 :
	public CIGUIFrameTimer
{
private:
	static volatile s32 timer_interrupt_ticks;
	static s32 t0_interrupt_id;
	static bool m_bInitDone; //init it only once, even if it is used multiple times (don't want singleton here though)

	static s32 t0_interrupt_handler(s32 ca);

	enum eInterrupts
	{
		kINTC_GS,
		kINTC_SBUS,
		kINTC_VBLANK_START,
		kINTC_VBLANK_END,
		kINTC_VIF0,
		kINTC_VIF1,
		kINTC_VU0,
		kINTC_VU1,
		kINTC_IPU,
		kINTC_TIMER0,
		kINTC_TIMER1
	}; 
public:
	CGUIFrameTimerPS2(void);
	~CGUIFrameTimerPS2(void);
	u32 getTicks();
	u64 getTicksRaw();
	void initTimer();
	void deinitTimer();
};

#endif //_GUIFRAMETIMERPS2_H_
