#ifndef _GUIMCAPOPUP_H_
#define _GUIMCAPOPUP_H_

#include "GUIMcaBaseWindow.h"
#include "IGUIFrameRenderer.h"
#include "IGUIFrameInput.h"
#include "IGUIFrameTimer.h"
#include "GUITypes.h"

class CGUIMcaPopup :
	public CGUIMcaBaseWindow
{
protected:
	void drawWindow(float alpha = 1.0f);
	void drawWindowInfo(float alpha = 1.0f);
	void drawExclam(float alpha = 1.0f);
	void drawFail(float alpha = 1.0f);
	void drawSuccess(float alpha = 1.0f);
public:
	virtual int display(bool blur = false) = 0;
	CGUIMcaPopup(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x = 0, float y = 0);
	virtual ~CGUIMcaPopup(void);
};

#endif //_GUIMCAPOPUP_H_
