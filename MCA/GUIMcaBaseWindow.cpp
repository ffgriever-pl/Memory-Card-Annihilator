#include "GUIMcaBaseWindow.h"

void CGUIMcaBaseWindow::fadeInOut(CIGUIFrameTexture* prevBuffTex, u32 ms, bool out)
{
	u32 currTick = 0, oldTick = 0;
	currTick = oldTick = m_timer->getTicks();

	float alpha = 0.0f;
	u32 ticks = 0;
	do
	{
		ticks = currTick - oldTick;
		alpha = (float)ticks / (float)ms;
		if (alpha > 1.0f) alpha = 1.0f;
		if (out) alpha = 1.0f - alpha;

		drawAll(prevBuffTex, alpha);
		m_renderer->swapBuffers();

		currTick = m_timer->getTicks();
	} while (ticks <= ms);
	drawAll(prevBuffTex, alpha);
	m_renderer->swapBuffers();
}

CGUIMcaBaseWindow::CGUIMcaBaseWindow(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y)
	: m_x(x)
	, m_y(y)
	, m_renderer(renderer)
	, m_input(input)
	, m_timer(timer)
	, m_input_state_new(0)
	, m_input_state_all(0)
	, m_ticks(0)
	, m_exit(false)
{
}
CGUIMcaBaseWindow::~CGUIMcaBaseWindow()
{
}

void CGUIMcaBaseWindow::drawLoop(CIGUIFrameTexture* prevBuffTex, u32 exit_buttons, float alpha)
{
	u32 currTick = 0, oldTick = 0;
	currTick = oldTick = m_timer->getTicks();
	do
	{
		m_ticks = currTick - oldTick;
		m_input->update();
		m_input_state_new = m_input->getNew(m_ticks);
		m_input_state_all = m_input->getAll();

		if (checkMessages())
		{
			currTick = oldTick = m_timer->getTicks();
			if (m_exit) break;
			else continue;
		}

		drawAll(prevBuffTex);
		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = m_timer->getTicks();

	} while ((m_input_state_new & exit_buttons) == 0);
}

bool CGUIMcaBaseWindow::checkMessages()
{
	return false;
}
