#include <string>
#include "GUIMcaWarrningNoCard.h"
#include "res/resources.h"

CGUIMcaWarrningNoCard::CGUIMcaWarrningNoCard(float x, float y, int slotnum)
{
	m_x = x;
	m_y = y;
	m_slotnum = slotnum;
}

CGUIMcaWarrningNoCard::CGUIMcaWarrningNoCard(void)
{
}


CGUIMcaWarrningNoCard::~CGUIMcaWarrningNoCard(void)
{
}

void CGUIMcaWarrningNoCard::fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out)
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

void CGUIMcaWarrningNoCard::drawMessage(float alpha)
{
	std::string formatted = CResources::mainLang.getText("LNG_WARN_NO_CARD_IN_SLOT");
	
	CResources::mainLang.replace(&formatted, "{SLOTNUM}", m_slotnum+1);
	CResources::verdana22.printUTF8Box(
		formatted.c_str()
		, m_x+8 +2, m_y+106 +2
		, 402, 180
		, CIGUIFrameFont<CGUITexture>::etxAlignJustify
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		formatted.c_str()
		, m_x+8, m_y+106
		, 402, 180
		, CIGUIFrameFont<CGUITexture>::etxAlignJustify
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
}

void CGUIMcaWarrningNoCard::drawAll(CIGUIFrameTexture *prevBuffTex, float alpha)
{
	m_renderer->setAlpha(true);
	m_renderer->setTestAlpha(true);
	m_renderer->setScissor(false);
	m_renderer->clearFrontBuffer();
	if (prevBuffTex != NULL) m_renderer->restoreFrameTex(prevBuffTex);
	drawWindow(alpha);
	drawExclam(alpha);
	drawMessage(alpha);
}

int CGUIMcaWarrningNoCard::display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur)
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
