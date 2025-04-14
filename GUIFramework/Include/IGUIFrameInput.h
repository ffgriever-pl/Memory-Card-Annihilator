#ifndef _IGUIFRAMEINPUT_H_
#define _IGUIFRAMEINPUT_H_

#include "IGUIFrameIncludes.h"

class CIGUIFrameInput
{
protected:
	float m_addx;
	float m_addy;
private:
	u32 m_buttonStatus;
	u32 m_buttonStatusOld;
	u32 m_total_ticks;
	u32 m_total_repeated;
	bool m_last_zero;
	bool m_repeat;
	virtual u32 updateInternal() = 0;
public:
	enum enInputData
	{
		enInTriangle	= 0x00000001,
		enInCross		= 0x00000002,
		enInLeft		= 0x00000004,
		enInRight		= 0x00000008,
		enInUp			= 0x00000010,
		enInDown		= 0x00000020,
		enInStart		= 0x00000040,
		enInSelect		= 0x00000080,
		enInR1			= 0x00000100,
		enInL1			= 0x00000200,
		enInR2			= 0x00000400,
		enInL2			= 0x00000800,
		enInSquare		= 0x00001000,
		enInCircle		= 0x00002000,
		enInKey			= 0x80000000
	};
	CIGUIFrameInput(void);
	virtual ~CIGUIFrameInput(void);

	void update();
	u32 getAll();
	u32 getNew(u32 ticks);
	void getAdditive(float &x, float &y);

	virtual bool getSystemState() = 0;
	virtual bool initInput() = 0;
};

#endif //_IGUIFRAMEINPUT_H_

