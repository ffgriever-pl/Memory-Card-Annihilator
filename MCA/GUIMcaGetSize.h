#ifndef _GUIMCAGETSIZE_H_
#define _GUIMCAGETSIZE_H_

#include "GUIMcaPopup.h"

class CGUIMcaGetSize :
	public CGUIMcaPopup
{
private:
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawMessage(float alpha = 1.0f);
	int m_card_mbytes;
	int m_card_mbytes_ret;
	bool checkMessages();
	u32 lzw(u32 val);
public:
	CGUIMcaGetSize(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y, int defaultmbytes);
	~CGUIMcaGetSize(void);
	int display(bool blur = false);
};

#endif //_GUIMCAGETSIZE_H_
