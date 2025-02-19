#ifndef _GUIMCAPOPUP_H_
#define _GUIMCAPOPUP_H_

#include "IGUIFrameRenderer.h"
#include "IGUIFrameInput.h"
#include "IGUIFrameTimer.h"
#include "GUITypes.h"

class CGUIMcaPopup
{
protected:
	float m_x;
	float m_y;
	CIGUIFrameRenderer *m_renderer;
	CIGUIFrameInput *m_input;
	CIGUIFrameTimer *m_timer;
	void drawWindow(float alpha = 1.0f);
	void drawWindowInfo(float alpha = 1.0f);
	void drawExclam(float alpha = 1.0f);
	void drawFail(float alpha = 1.0f);
	void drawSuccess(float alpha = 1.0f);
public:
	virtual int display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur = false) = 0;
	CGUIMcaPopup(void);
	virtual ~CGUIMcaPopup(void);
};

#endif //_GUIMCAPOPUP_H_
