#include <string>

#include "GUIMcaVkbd.h"
#include "res/resources.h"
#include "GUIMcaDisplayMessage.h"
#include "helpers.h"

CGUIMcaVkbd::CGUIMcaVkbd(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y)
	: CGUIMcaBaseWindow(renderer, input, timer, x, y)
	, m_over_num(-1)
	, m_max_chars(0)
	, m_display_caret(false)
	, m_caret_ticks(0)
	, m_curx(0)
	, m_cury(0)
	, m_caps(false)
	, m_shift(false)
	, m_tmp_shift(false)
	, m_filename(false)
{
	CResources::m_vkbd.loadTextureBuffer(CResources::vkbd_tm2, CResources::size_vkbd_tm2, true);
	CResources::m_cursor.loadTextureBuffer(CResources::cursor_tm2, CResources::size_cursor_tm2, true);
	CResources::m_vkbd_hover.loadTextureBuffer(CResources::vkbd_hover_tm2, CResources::size_vkbd_hover_tm2, true);
	CResources::m_cursor.setTexfilter(CIGUIFrameTexture::etFiltBilinear);
	CResources::m_vkbd_hover.setTexfilter(CIGUIFrameTexture::etFiltBilinear);
}

CGUIMcaVkbd::~CGUIMcaVkbd(void)
{
}

void CGUIMcaVkbd::drawWindow(float alpha)
{
	m_renderer->drawSpriteT(
		&CResources::m_vkbd
		, m_x, m_y
		, CResources::m_vkbd.getWidth(), CResources::m_vkbd.getHeight()
		, 0, 0
		, CResources::m_vkbd.getWidth(), CResources::m_vkbd.getHeight()
		, 128, 128, 128, alpha*0.90f
	);
}

void CGUIMcaVkbd::drawCursor(float alpha)
{
	m_renderer->drawSpriteT(
		&CResources::m_cursor
		, m_x +m_curx-42.0f, m_y +m_cury-37.0f
		, CResources::m_cursor.getWidth(), CResources::m_cursor.getHeight()
		, 0, 0
		, CResources::m_cursor.getWidth(), CResources::m_cursor.getHeight()
		, 128, 128, 128, alpha
	);
}

void CGUIMcaVkbd::drawText(float alpha)
{
	tVertex2 pos =
	CResources::mediumFont.printUTF8BoxShadow(
		m_text.c_str()
		, m_x+102, m_y+2
		, 372-12, 24
		, CIGUIFrameFont<CGUITexture>::etxAlignLeftRight
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);

	

	m_caret_ticks += m_ticks;
	if (m_caret_ticks > 50000)
	{
		m_caret_ticks = 0;
		m_display_caret = !m_display_caret;
	}
	if (m_display_caret)
	{
		float caretscale = (m_renderer->getFrameHeight() == 512 ? 1.0f : 512.0f/448.0f);
		CResources::mediumFont.printUTF8Shadow(
			"_"
			, pos.x, pos.y +CResources::mediumFont.getScale(22.00f*caretscale)*(float)CResources::mediumFont.getBasePos()
			, 22.00f*caretscale, 1.0f
			, 0, 0, 0, alpha
		);
	}
}

void CGUIMcaVkbd::drawKeys(float alpha)
{
	for (u32 i = 0; i < m_keys.size(); i++)
	{
		if (i == (u32)m_over_num)
		{
			m_renderer->drawSpriteT(&CResources::m_vkbd_hover
				, m_x +m_keys[i].x+m_keys[i].w-CResources::m_vkbd_hover.getWidth()+3, m_y +m_keys[i].y-2
				, CResources::m_vkbd_hover.getWidth(), CResources::m_vkbd_hover.getHeight()
				, 0, 0
				, CResources::m_vkbd_hover.getWidth(), CResources::m_vkbd_hover.getHeight()
				, 128, 128, 128, alpha
			);
		}
		int type = 0;
		if (m_caps && (m_shift || m_tmp_shift)) type = 3; //both
		else if (m_caps) type = 2; //caps
		else if (m_shift || m_tmp_shift) type = 1; //shift

		char key[] = {m_keys[i].key[type], 0};
		CResources::mediumFont.printUTF8Box(
			key,
			m_x +m_keys[i].x, m_y +m_keys[i].y+3,
			(int)m_keys[i].w, (int)m_keys[i].h-3,
			CIGUIFrameFont<CGUITexture>::etxAlignCenter,
			0.00f, 0,
			0, 0, 0,
			alpha*0.94f
		);
	}
}

void CGUIMcaVkbd::drawAll(CIGUIFrameTexture *prevBuffTex, float alpha)
{
	m_renderer->setAlpha(true);
	m_renderer->setTestAlpha(true);
	m_renderer->setScissor(false);
	m_renderer->clearFrontBuffer();
	if (prevBuffTex != NULL) m_renderer->restoreFrameTex(prevBuffTex);
	drawWindow(alpha);
	drawKeys(alpha);
	drawText(alpha);
	drawCursor(alpha);
}

bool CGUIMcaVkbd::checkMessages()
{
	bool windowCalled = false;

	updateKbPosition();

	if (m_input_state_new & CIGUIFrameInput::enInL1) m_caps = !m_caps;
	m_shift = m_input_state_all & CIGUIFrameInput::enInR1;
	if (m_input_state_new & CIGUIFrameInput::enInR1) m_tmp_shift = false;

	m_over_num = -1;

	if (m_curx > 0 && m_cury > 20 && m_curx < 484 && m_cury < 200)
	{
		for (u32 i = 0; i < m_keys.size(); i++)
		{
			if (m_curx >= m_keys[i].x && m_curx < m_keys[i].x + m_keys[i].w
				&& m_cury >= m_keys[i].y && m_cury < m_keys[i].y + m_keys[i].h)
			{
				m_over_num = i;
				break;
			}
		}
	}

	if (m_input_state_new & CIGUIFrameInput::enInCross)
	{
		if (m_over_num != -1 && (m_max_chars == 0 || m_text.size() <= m_max_chars) && m_over_num >= enfkKeyb)
		{
			int type = 0;
			if (m_caps && (m_shift || m_tmp_shift)) type = 3; //both
			else if (m_caps) type = 2; //caps
			else if (m_shift || m_tmp_shift) type = 1; //shift

			m_tmp_shift = false;

			m_text += m_keys[m_over_num].key[type];
		} else if (m_over_num != -1 && m_over_num < enfkKeyb)
		{
			switch (m_over_num)
			{
				case enfkCaps:
					m_caps = !m_caps;
					break;
				case enfkLshift:
					m_tmp_shift = !m_tmp_shift;
					break;
				case enfkRshift:
					m_tmp_shift = !m_tmp_shift;
					break;
				case enfkBackspace:
					if (m_text.size() > 0) m_text.resize(m_text.size()-1);
					break;
				case enfkEnter:
					m_exit = true;
					windowCalled = true;
					if (m_filename && m_text.find_first_of("\\/:*?\"<>|") != std::string::npos)
					{
						m_exit = false;
						CGUIMcaDisplayMessage myMessage(m_renderer, m_input, m_timer, 110, 106, CResources::mainLang.getText("LNG_VKBD_WARN_WRONG_NAME"), NULL, CGUIMcaDisplayMessage::enIcFail, CIGUIFrameFont<CGUITexture>::etxAlignCenter);
						myMessage.display(true);
					} else if (m_filename)
					{
						CGUIMcaLang::trimLeft(m_text, " ");
						CGUIMcaLang::trimRight(m_text, " ");
					}
					break;
			}
		}
	} else if (m_input_state_new & CIGUIFrameInput::enInSquare)
	{
		if (m_max_chars == 0 || m_text.size() <= m_max_chars) m_text += ' ';
	} else if (m_input_state_new & CIGUIFrameInput::enInCircle)
	{
		if (m_text.size() > 0) m_text.resize(m_text.size()-1);
	} else if (m_input_state_new & CIGUIFrameInput::enInStart)
	{
		m_exit = true;
		windowCalled = true;
		if (m_filename && m_text.find_first_of("\\/:*?\"<>|") != std::string::npos)
		{
			m_exit = false;
			CGUIMcaDisplayMessage myMessage(m_renderer, m_input, m_timer, 110, 106, CResources::mainLang.getText("LNG_VKBD_WARN_WRONG_NAME"), NULL, CGUIMcaDisplayMessage::enIcFail, CIGUIFrameFont<CGUITexture>::etxAlignCenter);
			myMessage.display(true);
		} else if (m_filename)
		{
			CGUIMcaLang::trimLeft(m_text, " ");
			CGUIMcaLang::trimRight(m_text, " ");
		}
	}

	return windowCalled;
}

void CGUIMcaVkbd::updateKbPosition()
{
	float cxadd, cyadd;
	m_input->getAdditive(cxadd, cyadd);
	if (m_input_state_all & CIGUIFrameInput::enInLeft) cxadd = -1.0f;
	else if (m_input_state_all & CIGUIFrameInput::enInRight) cxadd = 1.0f;
	if (m_input_state_all & CIGUIFrameInput::enInUp) cyadd = -1.0f;
	else if (m_input_state_all & CIGUIFrameInput::enInDown) cyadd = 1.0f;

	m_curx += (cxadd * ((float)m_ticks / 450.0f));
	m_cury += (cyadd * ((float)m_ticks / 450.0f));
	if (m_curx + m_x < 0) m_curx = -m_x;
	else if (m_curx + m_x > 640) m_curx = 640 - m_x;
	if (m_cury + m_y < 0) m_cury = -m_y;
	else if (m_cury + m_y > 512) m_cury = 512 - m_y;
}

int CGUIMcaVkbd::getEntry(const char *defname, std::string &ret, u32 max_chars, CIGUIFrameTexture *prevtex, bool blur, bool filename_mode)
{
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_text = defname;
	m_curx = 0;
	m_cury = 0;
	m_max_chars = max_chars;
	m_display_caret = false;
	m_caret_ticks = 0;
	m_exit = false;
	m_over_num = -1;
	m_filename = filename_mode;

	CIGUIFrameTexture *prevBuffTex = (prevtex != NULL) ? prevtex : getFrameTexture(blur);

	fadeInOut(prevBuffTex, 25000, false);
	drawLoop(prevBuffTex, CIGUIFrameInput::enInTriangle);
	fadeInOut(prevBuffTex, 25000, true);

	if (prevtex == NULL) delete prevBuffTex;

	if (m_exit)
	{
		ret = m_text;
		return 1;
	} else
	{
		ret = "";
		return 0;
	}
}

const CGUIMcaVkbd::keymapRaw CGUIMcaVkbd::m_keymap_raw[] = {
//funckeys - null
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(6, 94, 62, 30, 0, 0, 0, 0)), //capslock
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(6, 126, 78, 30, 0, 0, 0, 0)), //lshift
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(406, 126, 69, 30, 0, 0, 0, 0)), //rshift
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(422, 30, 53, 30, 0, 0, 0, 0)), //backspace
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(422, 94, 53, 30, 0, 0, 0, 0)), //enter
//first row
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(6, 30, 30, 30, '`', '~', '`', '~')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(38, 30, 30, 30, '1', '!', '1', '!')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(70, 30, 30, 30, '2', '@', '2', '@')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(102, 30, 30, 30, '3', '#', '3', '#')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(134, 30, 30, 30, '4', '$', '4', '$')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(166, 30, 30, 30, '5', '%', '5', '%')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(198, 30, 30, 30, '6', '^', '6', '^')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(230, 30, 30, 30, '7', '&', '7', '&')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(262, 30, 30, 30, '8', '*', '8', '*')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(294, 30, 30, 30, '9', '(', '9', '(')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(326, 30, 30, 30, '0', ')', '0', ')')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(358, 30, 30, 30, '-', '_', '-', '_')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(390, 30, 30, 30, '=', '+', '=', '+')),
//second row
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(54, 62, 30, 30, 'q', 'Q', 'Q', 'q')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(86, 62, 30, 30, 'w', 'W', 'W', 'w')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(118, 62, 30, 30, 'e', 'E', 'E', 'e')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(150, 62, 30, 30, 'r', 'R', 'R', 'r')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(182, 62, 30, 30, 't', 'T', 'T', 't')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(214, 62, 30, 30, 'y', 'Y', 'Y', 'y')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(246, 62, 30, 30, 'u', 'U', 'U', 'u')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(278, 62, 30, 30, 'i', 'I', 'I', 'i')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(310, 62, 30, 30, 'o', 'O', 'O', 'o')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(342, 62, 30, 30, 'p', 'P', 'P', 'p')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(374, 62, 30, 30, '[', '{', '[', '{')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(406, 62, 30, 30, ']', '}', ']', '}')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(438, 62, 37, 30, '\\', '|', '\\', '|')),
//third row
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(70, 94, 30, 30, 'a', 'A', 'A', 'a')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(102, 94, 30, 30, 's', 'S', 'S', 's')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(134, 94, 30, 30, 'd', 'D', 'D', 'd')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(166, 94, 30, 30, 'f', 'F', 'F', 'f')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(198, 94, 30, 30, 'g', 'G', 'G', 'g')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(230, 94, 30, 30, 'h', 'H', 'H', 'h')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(262, 94, 30, 30, 'j', 'J', 'J', 'j')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(294, 94, 30, 30, 'k', 'K', 'K', 'k')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(326, 94, 30, 30, 'l', 'L', 'L', 'l')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(358, 94, 30, 30, ';', ':', ';', ':')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(390, 94, 30, 30, '\'', '"', '\'', '"')),
//fourth row
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(86, 126, 30, 30, 'z', 'Z', 'Z', 'z')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(118, 126, 30, 30, 'x', 'X', 'X', 'x')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(150, 126, 30, 30, 'c', 'C', 'C', 'c')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(182, 126, 30, 30, 'v', 'V', 'V', 'v')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(214, 126, 30, 30, 'b', 'B', 'B', 'b')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(246, 126, 30, 30, 'n', 'N', 'N', 'n')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(278, 126, 30, 30, 'm', 'M', 'M', 'm')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(310, 126, 30, 30, ',', '<', ',', '<')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(342, 126, 30, 30, '.', '>', '.', '>')),
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(374, 126, 30, 30, '/', '?', '/', '?')),
//space
	CGUIMcaVkbd::keymapRaw(CGUIMcaVkbd::t_key(101, 158, 190, 30, ' ', ' ', ' ', ' ')),
};
const int CGUIMcaVkbd::m_numof_entries = countof(CGUIMcaVkbd::m_keymap_raw);
const std::vector<CGUIMcaVkbd::t_key> CGUIMcaVkbd::m_keys(CGUIMcaVkbd::m_keymap_raw, CGUIMcaVkbd::m_keymap_raw + CGUIMcaVkbd::m_numof_entries);
