#ifndef _GUIMCAMCARDINFO_H_
#define _GUIMCAMCARDINFO_H_

#include "GUIMcaPopup.h"
#include "GUITypes.h"
#include "IGUIFrameFont.h"

class CGUIMcaCardInfo :
	public CGUIMcaPopup
{
private:
	u32 m_input_state_new;
	u32 m_input_state_all;
	u32 m_ticks;
	bool m_psxmode;
	int m_oper_slot;
	CIGUIFrameFont<CGUITexture>::eAlignment m_align;
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out = false);
	void drawMessage(float alpha = 1.0f);
	CGUIMcaCardInfo(void);
public:
	CGUIMcaCardInfo(float x, float y);
	~CGUIMcaCardInfo(void);
	int displayInfo(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur = false, int oper_slot = 0, bool psxmode = false);
	int display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur = false);
};

#endif //_GUIMCAMCARDINFO_H_
