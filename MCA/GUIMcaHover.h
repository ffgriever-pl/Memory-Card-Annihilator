#ifndef _GUIMCAHOVER_H_
#define _GUIMCAHOVER_H_

#include "IGUIFrameRenderer.h"

class CGUIMcaHover
{
private:
	CGUIMcaHover(void);
	float m_x;
	float m_y;
	float m_w;
	float m_h;
	float m_dx;
	float m_dy;

	float m_divpoint;

	u32 m_speed;
	u32 m_speed_cur;
	u8 m_r;
	u8 m_g;
	u8 m_b;
	float m_alpha;
	u8 m_br; //border
	u8 m_bg;
	u8 m_bb;
	float m_balpha;
	bool m_visible;
	float m_cstate; u32 m_cspd; bool m_creverse;//for color blinking
	float m_astate; u32 m_aspd; bool m_areverse;//for alpha blinking
public:
	CGUIMcaHover(float x, float y, float w, float h, float divpoint, u32 speed_ms, u8 r, u8 g, u8 b, u8 br, u8 bg, u8 bb, float alpha, float balpha, bool visible);
	virtual ~CGUIMcaHover(void);

	virtual void setDest(float dx, float dy, bool instantly = false);
	virtual void drawHover(CIGUIFrameRenderer *renderer, u32 ticks, float alpha);
	void setVisibility(bool value) { m_visible = value; }
};

#endif //_GUIMCAHOVER_H_
