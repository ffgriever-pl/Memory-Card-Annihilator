#ifndef _GUIMCAABOUT_H_
#define _GUIMCAABOUT_H_

#include "GUIMcaPopup.h"

class CGUIMcaAbout :
	public CGUIMcaPopup
{
private:
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawMessage(float alpha = 1.0f);
public:
	CGUIMcaAbout(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y);
	~CGUIMcaAbout(void);
	int display(bool blur = false);
};

#endif //_GUIMCAABOUT_H_
