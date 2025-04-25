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
	fadeInOut(prevBuffTex, 25000, false);
	drawLoop(prevBuffTex);
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return 1;
}
