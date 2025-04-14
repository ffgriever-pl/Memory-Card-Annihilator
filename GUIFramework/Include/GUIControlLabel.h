#ifndef _GUICONTROLLABEL_H_
#define _GUICONTROLLABEL_H_

#include <string>
#include "IGUIFrameControl.h"
#include "IGUIFrameFont.h"
#include "GUITypes.h"

class CGUIControlLabel :
	public CIGUIFrameControl
{
protected:
	std::string m_txt;
	bool m_boxed;
	int m_w;
	int m_h;
	float m_txt_height;
	u8 m_r;
	u8 m_g;
	u8 m_b;
	float m_a;
	CIGUIFrameFont<CGUITexture>::eAlignment m_align;
	CIGUIFrameFont<CGUITexture> *m_font;
public:
	CGUIControlLabel(void);
	virtual ~CGUIControlLabel(void);
	virtual void draw(CIGUIFrameRenderer *renderer, u32 lastTick, u32 curTick);
	virtual void setupText(CIGUIFrameFont<CGUITexture> *font, std::string text, float txt_height, float xpos, float ypos, u8 r, u8 g, u8 b, u8 a, bool boxed = false, int w = 0, int h = 0, CIGUIFrameFont<CGUITexture>::eAlignment align = CIGUIFrameFont<CGUITexture>::etxAlignJustify);
};

#endif //_GUICONTROLLABEL_H_
