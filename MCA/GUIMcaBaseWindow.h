#ifndef _GUIMCABASEWINDOW_H_
#define _GUIMCABASEWINDOW_H_

#include "IGUIFrameRenderer.h"
#include "IGUIFrameInput.h"
#include "IGUIFrameTimer.h"
#include "GUITypes.h"

class CGUIMcaBaseWindow
{
public:
	virtual void drawAll(CIGUIFrameTexture* prevBuffTex = NULL, float alpha = 1.0f) = 0;
	virtual void fadeInOut(CIGUIFrameTexture* prevBuffTex, u32 ms, bool out = false);
protected:
	float m_x;
	float m_y;
	CIGUIFrameRenderer* m_renderer;
	CIGUIFrameInput* m_input;
	CIGUIFrameTimer* m_timer;

	CGUIMcaBaseWindow(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x = 0, float y = 0);
	virtual ~CGUIMcaBaseWindow();
};

#endif //_GUIMCABASEWINDOW_H_
