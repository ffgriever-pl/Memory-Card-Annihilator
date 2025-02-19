#ifndef _GUIMCADISPLAYMESSAGE_H_
#define _GUIMCADISPLAYMESSAGE_H_

#include "GUIMcaPopup.h"
#include "GUITypes.h"
#include "IGUIFrameFont.h"

class CGUIMcaDisplayMessage :
	public CGUIMcaPopup
{
public:
	enum enIconType
	{
		enIcExclam,
		enIcFail,
		enIcSuccess,
		enIcNone,
	};
private:
	u32 m_input_state_new;
	u32 m_input_state_all;
	u32 m_ticks;
	const char *m_message;
	const char *m_caption;
	enIconType m_icon;
	CIGUIFrameFont<CGUITexture>::eAlignment m_align;
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out = false);
	void drawMessage(float alpha = 1.0f);
	CGUIMcaDisplayMessage(void);
public:
	CGUIMcaDisplayMessage(float x, float y, const char *message, const char *caption, enIconType icon = enIcNone, CIGUIFrameFont<CGUITexture>::eAlignment align = CIGUIFrameFont<CGUITexture>::etxAlignJustify);
	~CGUIMcaDisplayMessage(void);
	int display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur = false);
};

#endif //_GUIMCADISPLAYMESSAGE_H_
