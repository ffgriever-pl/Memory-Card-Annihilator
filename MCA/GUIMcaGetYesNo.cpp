#include <string>
#include "GUIMcaGetYesNo.h"
#include "res/resources.h"

CGUIMcaGetYesNo::CGUIMcaGetYesNo(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y, const char* message, int defaultpos)
	: CGUIMcaPopup(renderer, input, timer, x, y)
	, m_result(defaultpos)
	, m_default(defaultpos)
	, m_message(message)
	, m_hover_yesno(renderer, 222, 230, 150, 38, 0.5f, 100, 255, 255, 255, 32, 32, 32, 0.50f, 0.25f, true)
{
}

CGUIMcaGetYesNo::~CGUIMcaGetYesNo(void)
{
}

bool CGUIMcaGetYesNo::checkMessages()
{
	if (m_input_state_new & CIGUIFrameInput::enInTriangle)
	{
		m_result = enresNo;
		m_hover_yesno.setDest(m_x+222+20,m_y+232, true);
	} else if (m_input_state_new & CIGUIFrameInput::enInLeft)
	{
		m_result = enresYes;
		m_hover_yesno.setDest(m_x+8+20,m_y+232);
	} else if (m_input_state_new & CIGUIFrameInput::enInRight)
	{
		m_result = enresNo;
		m_hover_yesno.setDest(m_x+222+20,m_y+232);
	}
		
	return false;
}

void CGUIMcaGetYesNo::drawMessage(float alpha)
{
	CResources::mediumFont.printUTF8BoxShadow(
		m_message
		, m_x+8, m_y+106
		, 402, 110
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);

	m_hover_yesno.drawHover(m_ticks, alpha);
	CResources::headerFont.printUTF8BoxShadow(
		CResources::mainLang.getText("LNG_OPER_YES")
		, m_x+8, m_y+230
		, 190, 50
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);

	CResources::headerFont.printUTF8BoxShadow(
		CResources::mainLang.getText("LNG_OPER_NO")
		, m_x+222, m_y+230
		, 190, 50
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
		, 3, 3, alpha * 0.25f
	);
}

void CGUIMcaGetYesNo::drawAll(CIGUIFrameTexture *prevBuffTex, float alpha)
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

int CGUIMcaGetYesNo::display(bool blur)
{
	m_result = m_default;
	m_input_state_new = 0;
	m_input_state_all = 0;

	if (m_result == enresNo)
		m_hover_yesno.setDest(m_x+222+20,m_y+232, true);
	else
		m_hover_yesno.setDest(m_x+8+20,m_y+232, true);

	CIGUIFrameTexture* prevBuffTex = getFrameTexture(blur);

	fadeInOut(prevBuffTex, 25000, false);
	drawLoop(prevBuffTex, CIGUIFrameInput::enInTriangle | CIGUIFrameInput::enInCross);
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return m_result;
}
