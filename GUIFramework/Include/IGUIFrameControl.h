#ifndef _IGUIFRAMECONTROL_H_
#define _IGUIFRAMECONTROL_H_

#include "IGUIFrameIncludes.h"
#include "IGUIFrameRenderer.h"

class CIGUIFrameControl
{
protected:
	bool m_has_focus;
	bool m_visible;
	bool m_accepts_msg;
	bool m_is_multi;
public:
	float m_xpos;
	float m_ypos;
	bool isMulti() const { return m_is_multi; }
	bool acceptsMsg() const { return m_accepts_msg; }
	bool hasFocus() const { return m_has_focus; }
	void setFocus(bool val) { m_has_focus = val; }
	CIGUIFrameControl(void);
	virtual ~CIGUIFrameControl(void);
	virtual void draw(CIGUIFrameRenderer *renderer, u32 lastTick, u32 curTick) = 0;
};

#endif //_IGUIFRAMECONTROL_H_
