#include <string>
#include "GUIMcaAbout.h"
#include "res/resources.h"

CGUIMcaAbout::CGUIMcaAbout(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y)
	: CGUIMcaPopup(renderer, input, timer, x, y)
{
}

CGUIMcaAbout::~CGUIMcaAbout(void)
{
}

void CGUIMcaAbout::drawMessage(float alpha)
{	
	CResources::headerFont.printUTF8BoxShadow(
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

	CResources::smallFont.printUTF8BoxShadow(
		CResources::mainLang.getText("LNG_INFO_THX_TXT")
		, m_x+8, m_y+45+10
		, 402, 164
		, CIGUIFrameFont<CGUITexture>::etxAlignJustify
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);

	CResources::smallFont.printUTF8BoxShadow(
		"~ ffgriever / berion\n2010"
		, m_x+8, m_y+240
		, 402, 164
		, CIGUIFrameFont<CGUITexture>::etxAlignRight
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
#ifdef MGMODE
	CResources::smallFont.printUTF8BoxShadow(
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

int CGUIMcaAbout::display(bool blur)
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
