#ifndef _GUIMCAGETYESNO_H_
#define _GUIMCAGETYESNO_H_

#include "GUIMcaPopup.h"
#include "GUIMcaHover.h"

class CGUIMcaGetYesNo :
	public CGUIMcaPopup
{
private:
	u32 m_input_state_new;
	u32 m_input_state_all;
	u32 m_ticks;
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawMessage(float alpha = 1.0f);
	int m_result;
	int m_default;
	bool m_return;
	const char *m_message;
	CGUIMcaHover m_hover_yesno;
	bool checkMessages();
public:
	enum enResult
	{
		enresNo = 0,
		enresYes = 1,
	};
	CGUIMcaGetYesNo(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y, const char* message, int defaultpos = enresNo);
	~CGUIMcaGetYesNo(void);
	int display(bool blur = false);
};

#endif //_GUIMCAGETYESNO_H_
