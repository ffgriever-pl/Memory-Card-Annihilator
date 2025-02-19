#ifndef _GUIMCAWARRNINGNOCARD_H_
#define _GUIMCAWARRNINGNOCARD_H_

#include "GUIMcaPopup.h"

class CGUIMcaWarrningNoCard :
	public CGUIMcaPopup
{
private:
	u32 m_input_state_new;
	u32 m_input_state_all;
	u32 m_ticks;
	int m_slotnum;
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out = false);
	void drawMessage(float alpha = 1.0f);
	CGUIMcaWarrningNoCard(void);
public:
	CGUIMcaWarrningNoCard(float x, float y, int slotnum);
	~CGUIMcaWarrningNoCard(void);
	int display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur = false);
};

#endif //_GUIMCAWARRNINGNOCARD_H_
