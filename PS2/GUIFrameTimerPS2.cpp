#include "Include\GUIFrameTimerPS2.h"

#define T0_COUNT ((volatile u32*)0x10000000)
#define T0_MODE ((volatile u32*)0x10000010)

CGUIFrameTimerPS2::CGUIFrameTimerPS2(void)
{
	initTimer();
}


CGUIFrameTimerPS2::~CGUIFrameTimerPS2(void)
{
	deinitTimer();
}

u32 CGUIFrameTimerPS2::getTicks() //1 second is 100000 ticks!
{
	if (!m_bInitDone) initTimer();

	u32 count = *T0_COUNT;
	//printf("Count = %d, Interrupts = %d, all = %d\n", count, timer_interrupt_ticks, (u32)((((float)count+(65535.0f*(float)timer_interrupt_ticks)))*(256000.0f/147456.0f)/10.0f));
	return (u32)((((float)count+(65535.0f*(float)timer_interrupt_ticks)))*(256000.0f/147456.0f)/10.0f) ;
}

u64 CGUIFrameTimerPS2::getTicksRaw() //1 second is 100000 ticks!
{
	if (!m_bInitDone) initTimer();

	u64 count = *T0_COUNT;
	//printf("Count = %d, Interrupts = %d, all = %d\n", count, timer_interrupt_ticks, (u32)((((float)count+(65535.0f*(float)timer_interrupt_ticks)))*(256000.0f/147456.0f)/10.0f));
	return count+(65535ULL*timer_interrupt_ticks);
}

void CGUIFrameTimerPS2::initTimer()
{
	if (!m_bInitDone)
	{
		timer_interrupt_ticks = 0;
		*T0_MODE = 0;  // stop counting
		*T0_COUNT = 0;  // reset counter

		if (t0_interrupt_id >= 0)
		{
			DisableIntc(kINTC_TIMER0);
			RemoveIntcHandler(kINTC_TIMER0, t0_interrupt_id);
			t0_interrupt_id = -1;
		}
		t0_interrupt_id = AddIntcHandler(kINTC_TIMER0, t0_interrupt_handler, 0);
		EnableIntc(kINTC_TIMER0);
		*T0_MODE = //128 + 2;  // start (128) + counting bus clocks / 256
		      0x002  // 576000 Hz clock
			+ 0x080  // start counting
			+ 0x200; // generate interrupt on overflow

		m_bInitDone = true;
	}
}

void CGUIFrameTimerPS2::deinitTimer()
{
	if (m_bInitDone)
	{
		*T0_MODE = 0x0000;
		if (t0_interrupt_id >= 0)
		{
			DisableIntc(kINTC_TIMER0);
			RemoveIntcHandler(kINTC_TIMER0, t0_interrupt_id);
			t0_interrupt_id = -1;
		}
		m_bInitDone = false;
	}
}

s32 CGUIFrameTimerPS2::t0_interrupt_handler(s32 ca)
{
	timer_interrupt_ticks++;
	*T0_MODE |= 0x800; // clear overflow status
	return -1;
}

volatile s32 CGUIFrameTimerPS2::timer_interrupt_ticks = 0;
s32 CGUIFrameTimerPS2::t0_interrupt_id = -1;
bool CGUIFrameTimerPS2::m_bInitDone = false;
