#include <string>
#include "GUIMcaGetYesNo.h"
#include "res/resources.h"

CGUIMcaGetYesNo::CGUIMcaGetYesNo(float x, float y, const char* message, int defaultpos)
	: m_hover_yesno(NULL)
{
	m_x = x;
	m_y = y;
	m_default = m_result = defaultpos;
	m_return = false;
	m_message = message;
	m_hover_yesno = new CGUIMcaHover(222, 230, 150, 38, 0.5f, 100, 255, 255, 255, 32, 32, 32, 0.50f, 0.25f, true);
}

CGUIMcaGetYesNo::CGUIMcaGetYesNo(void)
	: m_hover_yesno(NULL)
{
	m_result = enresNo;
	m_return = false;
	m_message = "";
}


CGUIMcaGetYesNo::~CGUIMcaGetYesNo(void)
{
	if (m_hover_yesno) delete m_hover_yesno;
}

bool CGUIMcaGetYesNo::checkMessages()
{
	bool windowCalled = false;

	if (m_input_state_new & CIGUIFrameInput::enInTriangle)
	{
		m_return = true;
		m_result = enresNo;
		if (m_hover_yesno) m_hover_yesno->setDest(m_x+222+20,m_y+232, true);
	} else if (m_input_state_new & CIGUIFrameInput::enInCross)
	{
		m_return = true;
	} else if (m_input_state_new & CIGUIFrameInput::enInLeft)
	{
		m_result = enresYes;
		if (m_hover_yesno) m_hover_yesno->setDest(m_x+8+20,m_y+232);
	} else if (m_input_state_new & CIGUIFrameInput::enInRight)
	{
		m_result = enresNo;
		if (m_hover_yesno) m_hover_yesno->setDest(m_x+222+20,m_y+232);
	}
		
	return windowCalled;
}

void CGUIMcaGetYesNo::fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out)
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

void CGUIMcaGetYesNo::drawMessage(float alpha)
{
	CResources::verdana22.printUTF8BoxShadow(
		m_message
		, m_x+8, m_y+106
		, 402, 110
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);

	if (m_hover_yesno) m_hover_yesno->drawHover(m_renderer, m_ticks, alpha);
	CResources::centurygoth38p.printUTF8BoxShadow(
		CResources::mainLang.getText("LNG_OPER_YES")
		, m_x+8, m_y+230
		, 190, 50
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);

	CResources::centurygoth38p.printUTF8BoxShadow(
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

int CGUIMcaGetYesNo::display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur)
{
	m_result = m_default;
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_renderer = renderer;
	m_input = input;
	m_timer = timer;

	if (m_hover_yesno)
	{
		if (m_result == enresNo)
			m_hover_yesno->setDest(m_x+222+20,m_y+232, true);
		else
			m_hover_yesno->setDest(m_x+8+20,m_y+232, true);
	}

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

		if (checkMessages())
		{
			currTick = oldTick = timer->getTicks();
			continue;
		}

		drawAll(prevBuffTex);
		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = timer->getTicks();

	} while ( !m_return );
	fadeInOut(prevBuffTex, timer, 25000, true);

	delete prevBuffTex;

	return m_result;
}
