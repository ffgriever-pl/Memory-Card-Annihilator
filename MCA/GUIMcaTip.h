#ifndef _GUIMCATIP_H_
#define _GUIMCATIP_H_

#include <queue>
#include "IGUIFrameRenderer.h"

class CGUIMcaTip
{
private:
	float m_x;
	float m_y;
	int m_fade_ticks_total;
	int m_fade_ticks;
	struct t_entry
	{
		std::string text;
		u8 r; u8 g; u8 b; float a;
		u32 time;
		u32 total_time;
		bool reverse;
		bool key_pressed;
		t_entry(std::string _text, u8 _r, u8 _g, u8 _b, float _a, u32 _time = 25000, bool _key_pressed = false)
		{
			text = _text;
			r = _r; g = _g; b = _b; a = _a;
			time = _time;
			total_time = 0;
			reverse = false;
			key_pressed = _key_pressed;
		}
	};
	std::queue<t_entry> m_queue;
	bool m_visible;
	CIGUIFrameRenderer* m_renderer;
public:
	CGUIMcaTip(CIGUIFrameRenderer* renderer, float x, float y, int time);
	virtual ~CGUIMcaTip(void);
	virtual void addTip(std::string message, u8 r = 0, u8 g = 0, u8 b = 0, float a = 1.0f, u32 time = 25000, bool key_pressed = false);
	virtual void drawTip(u32 new_input, u32 ticks, float alpha);
	void setVisibility(bool value) { m_visible = value; }
};

#endif //_GUIMCATIP_H_
