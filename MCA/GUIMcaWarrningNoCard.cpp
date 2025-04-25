#include <string>
#include "GUIMcaWarrningNoCard.h"
#include "res/resources.h"

CGUIMcaWarrningNoCard::CGUIMcaWarrningNoCard(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y, int slotnum)
	: CGUIMcaPopup(renderer, input, timer, x, y)
	, m_slotnum(slotnum)
{
}

CGUIMcaWarrningNoCard::~CGUIMcaWarrningNoCard(void)
{
}

void CGUIMcaWarrningNoCard::drawMessage(float alpha)
{
	std::string formatted = CResources::mainLang.getText("LNG_WARN_NO_CARD_IN_SLOT");
	
	CResources::mainLang.replace(&formatted, "{SLOTNUM}", m_slotnum+1);
	CResources::mediumFont.printUTF8BoxShadow(
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

int CGUIMcaWarrningNoCard::display(bool blur)
{
	u32 state_new = 0;
	u32 state_all = 0;

	CIGUIFrameTexture *prevBuffTex;
	if (blur)
	{
		prevBuffTex = m_renderer->getFrameTex(1);
		prevBuffTex->blur(0);
		prevBuffTex->blur(0);
	} else
	{
		prevBuffTex = m_renderer->getFrameTex();
	}
	u32 ticks = 0;
	fadeInOut(prevBuffTex, 25000, false);
	u32 currTick = 0, oldTick = 0;
	currTick = oldTick = m_timer->getTicks();
	do
	{
		ticks = currTick - oldTick;
		m_input->update();
		state_new = m_input->getNew(ticks);
		state_all = m_input->getAll();

		drawAll(prevBuffTex);
		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = m_timer->getTicks();

	} while (state_new == 0);
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return 1;
}
