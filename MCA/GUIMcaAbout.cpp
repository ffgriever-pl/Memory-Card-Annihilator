#include <string>
#include "GUIMcaAbout.h"
#include "res/resources.h"

CGUIMcaAbout::CGUIMcaAbout(float x, float y)
{
	m_x = x;
	m_y = y;
}

CGUIMcaAbout::CGUIMcaAbout(void)
{
}


CGUIMcaAbout::~CGUIMcaAbout(void)
{
}

void CGUIMcaAbout::fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out)
{
	u32 currTick = 0, oldTick = 0;
	currTick = oldTick = timer->getTicks();

	float alpha = 0.0f;
	u32 ticks = 0;
	do
	{
		ticks = currTick - oldTick;
		alpha = (float)ticks/(float)ms;
		if (alpha > 1.0f) alpha = 1.0f;
		if (out) alpha = 1.0f - alpha;

		drawAll(prevBuffTex, alpha);
		m_renderer->swapBuffers();

		currTick = timer->getTicks();
	} while (ticks <= ms);
	drawAll(prevBuffTex, alpha);
	m_renderer->swapBuffers();
}

void CGUIMcaAbout::drawMessage(float alpha)
{
	std::string formatted = CResources::mainLang.getText("LNG_WARN_NO_CARD_IN_SLOT");
	
	CResources::centurygoth38p.printUTF8BoxShadow(
		CResources::mainLang.getText("LNG_INFO_THX_CAP")
		, m_x+8, m_y+15
		, 402, 38
		, CIGUIFrameFont<CGUITexture>::etxAlignLeft
		, 00.00f, 1.0f
		, 32, 32, 32, alpha
		, 4, 4, alpha * 0.25f
		, 32, 32, 32, alpha
		, 0, 0, 0, alpha
		, 0, 0, 0, alpha
		, true
	);

	CResources::verdana18s.printUTF8BoxShadow(
		CResources::mainLang.getText("LNG_INFO_THX_TXT")
		, m_x+8, m_y+45+10
		, 402, 164
		, CIGUIFrameFont<CGUITexture>::etxAlignJustify
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);

	CResources::verdana18s.printUTF8BoxShadow(
		"~ ffgriever / berion\n2010"
		, m_x+8, m_y+240
		, 402, 164
		, CIGUIFrameFont<CGUITexture>::etxAlignRight
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
#ifdef MGMODE
	CResources::verdana18s.printUTF8BoxShadow(
		"MG MODE: "
		MGMODE
		"\nAlt MG By El_isra"
		, m_x+8, m_y+240
		, 402, 164
		, CIGUIFrameFont<CGUITexture>::etxAlignLeft
		, 00.00f, 1.0f
		, 255, 30, 30, alpha
	);
#endif
}

void CGUIMcaAbout::drawAll(CIGUIFrameTexture *prevBuffTex, float alpha)
{
	m_renderer->setAlpha(true);
	m_renderer->setTestAlpha(true);
	m_renderer->setScissor(false);
	m_renderer->clearFrontBuffer();
	if (prevBuffTex != NULL) m_renderer->restoreFrameTex(prevBuffTex);
	drawWindow(alpha);
	drawMessage(alpha);
}

int CGUIMcaAbout::display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur)
{
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_renderer = renderer;
	m_input = input;
	m_timer = timer;

	CIGUIFrameTexture *prevBuffTex;
	if (blur)
	{
		prevBuffTex = renderer->getFrameTex(1);
		prevBuffTex->blur(0);
		prevBuffTex->blur(0);
	} else
	{
		prevBuffTex = renderer->getFrameTex();
	}
	m_ticks = 0;
	fadeInOut(prevBuffTex, timer, 25000, false);
	u32 currTick = 0, oldTick = 0;
	currTick = oldTick = timer->getTicks();
	do
	{
		m_ticks = currTick - oldTick;
		input->update();
		m_input_state_new = input->getNew(m_ticks);
		m_input_state_all = input->getAll();

		drawAll(prevBuffTex);
		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = timer->getTicks();

	} while (m_input_state_new == 0);
	fadeInOut(prevBuffTex, timer, 25000, true);

	delete prevBuffTex;

	return 1;
}
