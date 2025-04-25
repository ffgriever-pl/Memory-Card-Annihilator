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
	CIGUIFrameTexture *prevBuffTex = getFrameTexture(blur);

	fadeInOut(prevBuffTex, 25000, false);
	drawLoop(prevBuffTex);
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return 1;
}
