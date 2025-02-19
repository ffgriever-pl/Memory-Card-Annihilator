#include "Include\IGUIFrameInput.h"


CIGUIFrameInput::CIGUIFrameInput(void)
	: m_buttonStatus(0)
{
	m_addx = m_addy = 0.0f;
	m_last_zero = false;
	m_repeat = false;
}


CIGUIFrameInput::~CIGUIFrameInput(void)
{
}

void CIGUIFrameInput::update()
{
	m_buttonStatusOld = m_buttonStatus;
	m_buttonStatus = updateInternal();
}

u32 CIGUIFrameInput::getAll()
{
	return m_buttonStatus;
}

u32 CIGUIFrameInput::getNew(u32 ticks)
{
	u32 retval = (m_buttonStatus ^ m_buttonStatusOld) & m_buttonStatus;
	if (m_buttonStatus > 0 && retval == 0)
	{
		if (m_last_zero)
		{
			m_total_ticks += ticks;
			if (m_repeat)
			{
				if (m_total_ticks > (m_total_repeated > 10 ? (m_total_repeated > 40 ? 5000 : 8000) : 12000))
				{
					m_total_ticks = 0;
					m_total_repeated++;
					retval = m_buttonStatus;
				}
			} else
			{
				if (m_total_ticks > 50000)
				{
					m_total_ticks = 0;
					m_total_repeated = 1;
					m_repeat = true;
					retval = m_buttonStatus;
				}
			}
		} else
		{
			m_total_ticks = 0;
			m_total_repeated = 0;
			m_last_zero = true;
			m_repeat = false;
		}
	} else
	{
		m_total_ticks = 0;
		m_total_repeated = 0;
		m_last_zero = false;
		m_repeat = false;
	}
	return retval;
}

void CIGUIFrameInput::getAdditive(float &x, float &y)
{
	x = m_addx;
	y = m_addy;
}
