#include "Include/GUIControlLabel.h"


CGUIControlLabel::CGUIControlLabel(void)
{
	m_visible = false;
	m_txt = "";
	m_boxed = false;
	m_font = NULL;
}


CGUIControlLabel::~CGUIControlLabel(void)
{
}

void CGUIControlLabel::draw(CIGUIFrameRenderer *renderer, u32 lastTick, u32 curTick)
{
	if (!m_visible || !m_font || m_txt == "")
		return;

	if (m_boxed)
	{
		m_font->printUTF8Box(
			m_txt.c_str()
			,m_xpos, m_ypos
			,m_w, m_h
			,m_align
			,m_txt_height
			,1.0f
			,m_r, m_g, m_b, m_a
			);
	} else
	{
		m_font->printUTF8(
			m_txt.c_str()
			,m_xpos, m_ypos
			,m_txt_height
			,1.0f
			,m_r, m_g, m_b, m_a
			);
	}
}

void CGUIControlLabel::setupText(CIGUIFrameFont<CGUITexture> *font, std::string text, float txt_height, float xpos, float ypos, u8 r, u8 g, u8 b, u8 a, bool boxed, int w, int h, CIGUIFrameFont<CGUITexture>::eAlignment align)
{
	m_font = font;
	m_txt = text;
	m_txt_height = txt_height;
	m_xpos = xpos;
	m_ypos = ypos;
	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;
	m_boxed = boxed;
	m_w = w;
	m_h = h;
	m_align = align;
	m_visible = true;
}
