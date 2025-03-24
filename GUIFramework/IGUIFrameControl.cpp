#include "Include/IGUIFrameControl.h"


CIGUIFrameControl::CIGUIFrameControl(void)
	: m_has_focus(false)
	, m_visible(true)
	, m_accepts_msg(true)
	, m_is_multi(false)
	, m_xpos(0.0f)
	, m_ypos(0.0f)
{
}


CIGUIFrameControl::~CIGUIFrameControl(void)
{
	
}
