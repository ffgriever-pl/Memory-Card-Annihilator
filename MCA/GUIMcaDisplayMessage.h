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
	const char *m_message;
	const char *m_caption;
	enIconType m_icon;
	CIGUIFrameFont<CGUITexture>::eAlignment m_align;
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawMessage(float alpha = 1.0f);
	CGUIMcaDisplayMessage(void);
public:
	CGUIMcaDisplayMessage(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x = 0, float y = 0, const char* message = NULL, const char* caption = NULL, enIconType icon = enIcNone, CIGUIFrameFont<CGUITexture>::eAlignment align = CIGUIFrameFont<CGUITexture>::etxAlignJustify);
	~CGUIMcaDisplayMessage(void);
	int display(bool blur = false);
};

#endif //_GUIMCADISPLAYMESSAGE_H_
