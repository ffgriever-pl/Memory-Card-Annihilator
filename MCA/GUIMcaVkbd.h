#ifndef _GUIMCAVKBD_H_
#define _GUIMCAVKBD_H_

#include <vector>
#include <string>
#include "IGUIFrameRenderer.h"
#include "IGUIFrameInput.h"
#include "IGUIFrameTimer.h"
#include "GUIMcaBaseWindow.h"
#include "GUITypes.h"

class CGUIMcaVkbd
	: public CGUIMcaBaseWindow
{
public:
	struct t_key
	{
		float x;
		float y;
		float w;
		float h;
		char key[4];//norm, shift, caps
		t_key(float _x, float _y, float _w, float _h, char norm, char shift, char caps, char both)
		{
			x = _x;
			y = _y;
			h = _h;
			w = _w;
			key[0] = norm;
			key[1] = shift;
			key[2] = caps;
			key[3] = both;
		}
	};
	typedef std::vector<t_key>::value_type keymapRaw;
private:
	enum enFunKeys
	{
		enfkCaps = 0,
		enfkLshift = 1,
		enfkRshift = 2,
		enfkBackspace = 3,
		enfkEnter = 4,
		enfkKeyb = 5,
	};
	static const int m_numof_entries;
	static const keymapRaw m_keymap_raw[];
	static const std::vector<t_key> m_keys;
	int m_over_num;
	u32 m_max_chars;
	bool m_display_caret;
	u32 m_caret_ticks;
	float m_curx;
	float m_cury;
	bool m_caps;
	bool m_shift;
	bool m_tmp_shift;
	std::string m_text;
	bool m_filename;
	void drawWindow(float alpha = 1.0f);
	void drawCursor(float alpha = 1.0f);
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawText(float alpha = 1.0f);
	void drawKeys(float alpha = 1.0f);
	bool checkMessages();
	void updateKbPosition();
public:
	CGUIMcaVkbd(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y);
	~CGUIMcaVkbd(void);
	int getEntry(const char *defname, std::string &ret, u32 max_chars = 0, CIGUIFrameTexture *prevtex = NULL, bool blur = false, bool filename_mode = false);
};

#endif //_GUIMCAVKBD_H_
