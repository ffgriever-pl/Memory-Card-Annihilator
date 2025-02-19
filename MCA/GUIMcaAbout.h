#ifndef _GUIMCAABOUT_H_
#define _GUIMCAABOUT_H_

#include "GUIMcaPopup.h"

class CGUIMcaAbout :
	public CGUIMcaPopup
{
private:
	u32 m_input_state_new;
	u32 m_input_state_all;
	u32 m_ticks;
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out = false);
	void drawMessage(float alpha = 1.0f);
	CGUIMcaAbout(void);
public:
	CGUIMcaAbout(float x, float y);
	~CGUIMcaAbout(void);
	int display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur = false);
};

#endif //_GUIMCAABOUT_H_
