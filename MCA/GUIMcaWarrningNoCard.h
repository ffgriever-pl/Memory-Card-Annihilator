#ifndef _GUIMCAWARRNINGNOCARD_H_
#define _GUIMCAWARRNINGNOCARD_H_

#include "GUIMcaPopup.h"

class CGUIMcaWarrningNoCard :
	public CGUIMcaPopup
{
private:
	int m_slotnum;
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawMessage(float alpha = 1.0f);
public:
	CGUIMcaWarrningNoCard(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y, int slotnum);
	~CGUIMcaWarrningNoCard(void);
	int display(bool blur = false);
};

#endif //_GUIMCAWARRNINGNOCARD_H_
