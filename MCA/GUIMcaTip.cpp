#include "GUIMcaTip.h"
#include "res/resources.h"

CGUIMcaTip::CGUIMcaTip(void)
{
	m_fade_ticks = 1;
	m_fade_ticks_total = 0;

	CResources::m_popup_tip.loadTextureBuffer(CResources::popup_tip_tm2, CResources::size_popup_tip_tm2, true);
}

CGUIMcaTip::CGUIMcaTip(float x, float y, int time)
{
	m_x = x;
	m_y = y;
	m_fade_ticks = time;
	m_fade_ticks_total = 0;
	m_visible = true;

	CResources::m_popup_tip.loadTextureBuffer(CResources::popup_tip_tm2, CResources::size_popup_tip_tm2, true);
}

void CGUIMcaTip::addTip(std::string message, u8 r, u8 g, u8 b, float a, u32 time, bool key_pressed)
{
	m_queue.push( t_entry(message, r, g, b, a, time, key_pressed) );
}

void CGUIMcaTip::drawTip(CIGUIFrameRenderer *renderer, u32 new_input, u32 ticks, float alpha)
{
	if (!m_visible || m_queue.size() == 0) return;
	//float addcol = (float)ticks/(float)m_cspd;
	float fadealpha = 0;

	t_entry &currentry = m_queue.front();

	if (new_input != 0) currentry.key_pressed = true;

	if (currentry.total_time == 0 && currentry.reverse == false)
	{
		m_fade_ticks_total += ticks;
		fadealpha = (float)m_fade_ticks_total/(float)m_fade_ticks;
		if (fadealpha > 1.0f)
		{
			fadealpha = 1.0f;
			currentry.total_time = 1;
		}
	} else if (currentry.total_time > 0 && currentry.reverse == false)
	{
		fadealpha = 1.0f;
		currentry.total_time += ticks;
		if (currentry.total_time >= currentry.time && currentry.key_pressed)
		{
			currentry.reverse = true;
			m_fade_ticks_total = 0;
		}
	} else
	{
		m_fade_ticks_total += ticks;
		fadealpha = (float)m_fade_ticks_total/(float)m_fade_ticks;
		if (fadealpha > 1.0f)
		{
			fadealpha = 1.0f;
			currentry.total_time = 1;

			m_queue.pop();
			return;
		}
		fadealpha = 1.0f - fadealpha;
	}
	//finally draw the thing

	renderer->drawSpriteT(
		&CResources::m_popup_tip,
		m_x, m_y,
		CResources::m_popup_tip.getWidth(), CResources::m_popup_tip.getHeight(),
		0, 0,
		CResources::m_popup_tip.getWidth(), CResources::m_popup_tip.getHeight(),
		128, 128, 128, alpha*fadealpha
	);
	CResources::verdana18s.printUTF8Box(
		currentry.text.c_str(),
		m_x + 6 +2,
		m_y + 7 +2 +4,
		298,
		44,
		CIGUIFrameFont<CGUITexture>::etxAlignLeft,
		0.00f, 0.0f,
		currentry.r, currentry.g, currentry.b, currentry.a*alpha*fadealpha*0.15f
	);
	CResources::verdana18s.printUTF8Box(
		currentry.text.c_str(),
		m_x + 6,
		m_y + 7 +4,
		298,
		44,
		CIGUIFrameFont<CGUITexture>::etxAlignLeft,
		0.00f, 0.0f,
		currentry.r, currentry.g, currentry.b, currentry.a*alpha*fadealpha
	);
}


CGUIMcaTip::~CGUIMcaTip(void)
{
	while (m_queue.size()) m_queue.pop();
}
