#ifndef _GUIMCAMCARDINFO_H_
#define _GUIMCAMCARDINFO_H_

#include "GUIMcaPopup.h"
#include "GUITypes.h"
#include "IGUIFrameFont.h"

class CGUIMcaCardInfo :
	public CGUIMcaPopup
{
private:
	bool m_psxmode;
	int m_oper_slot;
	CIGUIFrameFont<CGUITexture>::eAlignment m_align;
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawMessage(float alpha = 1.0f);
	CGUIMcaCardInfo(void);
public:
	CGUIMcaCardInfo(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y);
	~CGUIMcaCardInfo(void);
	int displayInfo(bool blur = false, int oper_slot = 0, bool psxmode = false);
	int display(bool blur = false);
};

#endif //_GUIMCAMCARDINFO_H_
