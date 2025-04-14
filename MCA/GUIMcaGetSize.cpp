#include "GUIMcaGetSize.h"
#include "res/resources.h"
#include <string>

CGUIMcaGetSize::CGUIMcaGetSize(float x, float y, int defaultmbytes)
{
	m_x = x;
	m_y = y;
	m_card_mbytes = defaultmbytes;
	//m_card_mbytes = 8;
	m_return = false;
}

CGUIMcaGetSize::CGUIMcaGetSize(void)
{
	m_card_mbytes = 8;
	m_return = false;
}


CGUIMcaGetSize::~CGUIMcaGetSize(void)
{
}

u32 CGUIMcaGetSize::lzw(u32 val)
{
	u32 res;
	__asm__ __volatile__ ("   plzcw   %0, %1    " : "=r" (res) : "r" (val));
	return(res);
}

bool CGUIMcaGetSize::checkMessages()
{
	bool windowCalled = false;

	if (m_input_state_new & CIGUIFrameInput::enInLeft)
	{
		int new_mbytes = m_card_mbytes;
		new_mbytes = 31 - (lzw(new_mbytes) + 1);
		
		if (m_card_mbytes == (1 << new_mbytes))
			new_mbytes--;

		if (new_mbytes < 3) new_mbytes = 7;

		m_card_mbytes = 1 << new_mbytes;
	} else if (m_input_state_new & CIGUIFrameInput::enInRight)
	{
		int new_mbytes = m_card_mbytes;
		new_mbytes = 31 - (lzw(new_mbytes) + 1);
		
		new_mbytes++;

		if (new_mbytes > 7) new_mbytes = 3;

		m_card_mbytes = 1 << new_mbytes;
	} else if (m_input_state_new & CIGUIFrameInput::enInL1)
	{
		m_card_mbytes--;
		if (m_card_mbytes < 1) m_card_mbytes = 128;
	} else if (m_input_state_new & CIGUIFrameInput::enInR1)
	{
		m_card_mbytes++;
		if (m_card_mbytes > 128) m_card_mbytes = 1;
	} else if (m_input_state_new & CIGUIFrameInput::enInTriangle)
	{
		m_return = true;
		m_card_mbytes_ret = -1;
	} else if (m_input_state_new & CIGUIFrameInput::enInCross)
	{
		m_return = true;
		m_card_mbytes_ret = m_card_mbytes;
	}
	
	return windowCalled;
}

void CGUIMcaGetSize::fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out)
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

void CGUIMcaGetSize::drawMessage(float alpha)
{
	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_OPER_CHOOSE_MC_SIZE")
		, m_x+8 +2, m_y+106 +2
		, 402, 90
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_OPER_CHOOSE_MC_SIZE")
		, m_x+8, m_y+106
		, 402, 90
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);

	std::string formatted = CResources::mainLang.getText("LNG_OPER_MEGABYTE");
	CResources::mainLang.replace(&formatted, "{CARDSIZE}", m_card_mbytes);
	CResources::centurygoth38p.printUTF8Box(
		formatted.c_str()
		, m_x+8 +3, m_y+206 +3
		, 402, 50
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.25f
	);
	CResources::centurygoth38p.printUTF8Box(
		formatted.c_str()
		, m_x+8, m_y+206
		, 402, 50
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
}

void CGUIMcaGetSize::drawAll(CIGUIFrameTexture *prevBuffTex, float alpha)
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

int CGUIMcaGetSize::display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur)
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

	return m_card_mbytes_ret;
}
