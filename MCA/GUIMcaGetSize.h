#ifndef _GUIMCAGETSIZE_H_
#define _GUIMCAGETSIZE_H_

#include "GUIMcaPopup.h"

class CGUIMcaGetSize :
	public CGUIMcaPopup
{
private:
	u32 m_input_state_new;
	u32 m_input_state_all;
	u32 m_ticks;
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out = false);
	void drawMessage(float alpha = 1.0f);
	int m_card_mbytes;
	int m_card_mbytes_ret;
	bool m_return;
	CGUIMcaGetSize(void);
	bool checkMessages();
	u32 lzw(u32 val);
public:
	CGUIMcaGetSize(float x, float y, int defaultmbytes);
	~CGUIMcaGetSize(void);
	int display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur = false);
};

#endif //_GUIMCAGETSIZE_H_
