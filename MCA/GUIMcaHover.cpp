#include "GUIMcaHover.h"

#define HOV_ALPHAMIN 0.10f
CGUIMcaHover::CGUIMcaHover(void)
{
}

CGUIMcaHover::CGUIMcaHover(float x, float y, float w, float h, float divpoint, u32 speed_ms, u8 r, u8 g, u8 b, u8 br, u8 bg, u8 bb, float alpha, float balpha, bool visible)
{
	m_x = m_dx = x;
	m_y = m_dy = y;
	m_w = w;
	m_h = h;

	m_divpoint = divpoint;

	m_speed = m_speed_cur = speed_ms;

	m_r = r;
	m_g = g;
	m_b = b;
	m_alpha = alpha;

	m_br = br;
	m_bg = bg;
	m_bb = bb;
	m_balpha = balpha;

	m_visible = visible;

	m_astate = HOV_ALPHAMIN;
	m_aspd = 60000;
	m_areverse = false;
}

void CGUIMcaHover::setDest(float dx, float dy, bool instantly)
{
	if (instantly)
	{
		m_x = m_dx = dx;
		m_y = m_dy = dy;
		m_speed_cur = m_speed;
	} else
	{
		if (m_x != m_dx || m_y != m_dy) m_speed_cur >>= 1;
		if (m_speed_cur == 0) m_speed_cur = 1;

		m_dx = dx;
		m_dy = dy;
	}
}

void CGUIMcaHover::drawHover(CIGUIFrameRenderer *renderer, u32 ticks, float alpha)
{
	if (!m_visible) return;
	//float addcol = (float)ticks/(float)m_cspd;
	float addalp = (float)ticks/(float)m_aspd;
	float addpos = (float)ticks/(float)m_speed_cur;

	if (m_areverse) addalp = -addalp;
	m_astate += addalp;
	if (m_astate >= 1.0f)
	{
		m_astate = 1.0f - (m_astate - 1.0f); //fixme - it might get over at very low fps
		m_areverse = true;
	}
	if (m_astate <= HOV_ALPHAMIN)
	{
		m_astate = HOV_ALPHAMIN + (HOV_ALPHAMIN - m_astate); //fixme - it might get over at very low fps
		m_areverse = false;
	}
	//calc x
	if (m_dx > m_x)
	{
		m_x += addpos;
		if (m_x >= m_dx)
		{
			m_x = m_dx;
			m_speed_cur = m_speed;
		}
	} else if (m_dx < m_x)
	{
		m_x -= addpos;
		if (m_x <= m_dx)
		{
			m_x = m_dx;
			m_speed_cur = m_speed;
		}
	}
	//calc y
	if (m_dy > m_y)
	{
		m_y += addpos;
		if (m_y >= m_dy)
		{
			m_y = m_dy;
			m_speed_cur = m_speed;
		}
	} else if (m_dy < m_y)
	{
		m_y -= addpos;
		if (m_y <= m_dy)
		{
			m_y = m_dy;
			m_speed_cur = m_speed;
		}
	}
	//finally draw the thing
	//top border
	renderer->drawQuadG(
		m_x, m_y -2.0f,
		m_x+ m_divpoint*m_w, m_y -2.0f,
		m_x, m_y,
		m_x+ m_divpoint*m_w, m_y,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		0, m_balpha *m_astate*alpha, 0, m_balpha *m_astate*alpha
	);
	renderer->drawQuadG(
		m_x+ m_divpoint*m_w, m_y -2.0f,
		m_x+ m_w, m_y -2.0f,
		m_x+ m_divpoint*m_w, m_y,
		m_x+ m_w, m_y,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		m_balpha *m_astate*alpha, 0, m_balpha *m_astate*alpha, 0
	);
	//center
	renderer->drawQuadG(
		m_x, m_y,
		m_x+ m_divpoint*m_w, m_y,
		m_x, m_y+m_h,
		m_x+ m_divpoint*m_w, m_y+m_h,
		m_r, m_g, m_b,
		m_r, m_g, m_b,
		m_r, m_g, m_b,
		m_r, m_g, m_b,
		0, m_alpha *m_astate*alpha, 0, m_alpha *m_astate*alpha
	);
	renderer->drawQuadG(
		m_x+ m_divpoint*m_w, m_y,
		m_x+ m_w, m_y,
		m_x+ m_divpoint*m_w, m_y+m_h,
		m_x+ m_w, m_y+m_h,
		m_r, m_g, m_b,
		m_r, m_g, m_b,
		m_r, m_g, m_b,
		m_r, m_g, m_b,
		m_alpha *m_astate*alpha, 0, m_alpha *m_astate*alpha, 0
	);
	//bottom border
	renderer->drawQuadG(
		m_x, m_y +m_h,
		m_x+ m_divpoint*m_w, m_y +m_h,
		m_x, m_y+2.0f +m_h,
		m_x+ m_divpoint*m_w, m_y+2.0f +m_h,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		0, m_balpha *m_astate*alpha, 0, m_balpha *m_astate*alpha
	);
	renderer->drawQuadG(
		m_x+ m_divpoint*m_w, m_y +m_h,
		m_x+ m_w, m_y +m_h,
		m_x+ m_divpoint*m_w, m_y+2.0f +m_h,
		m_x+ m_w, m_y+2.0f +m_h,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		m_br, m_bg, m_bb,
		m_balpha *m_astate*alpha, 0, m_balpha *m_astate*alpha, 0
	);
}


CGUIMcaHover::~CGUIMcaHover(void)
{
}
