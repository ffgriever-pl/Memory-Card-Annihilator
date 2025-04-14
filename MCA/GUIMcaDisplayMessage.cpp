#include <string>
#include "GUIMcaDisplayMessage.h"
#include "res/resources.h"

CGUIMcaDisplayMessage::CGUIMcaDisplayMessage(float x, float y, const char *message, const char *caption, enIconType icon, CIGUIFrameFont<CGUITexture>::eAlignment align)
{
	m_x = x;
	m_y = y;
	m_message = message;
	m_caption = caption;
	m_icon = icon;
	m_align = align;
}

CGUIMcaDisplayMessage::CGUIMcaDisplayMessage(void)
	: m_message(NULL)
	, m_caption(NULL)
{
}


CGUIMcaDisplayMessage::~CGUIMcaDisplayMessage(void)
{
}

void CGUIMcaDisplayMessage::fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out)
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

void CGUIMcaDisplayMessage::drawMessage(float alpha)
{
	if (m_message)
	{
		if (m_icon != enIcNone || m_caption != NULL)
		{
			if (m_caption != NULL)
			{
				CResources::centurygoth38p.printUTF8Box(
					m_caption
					, m_x+8 +4, m_y+34 +4
					, 402, 50
					, CIGUIFrameFont<CGUITexture>::etxAlignCenter
					, 00.00f, 1.0f
					, 0, 0, 0, alpha*0.25f
				);
				CResources::centurygoth38p.printUTF8Box(
					m_caption
					, m_x+8, m_y+34
					, 402, 50
					, CIGUIFrameFont<CGUITexture>::etxAlignCenter
					, 00.00f, 1.0f
					, 64, 64, 64, alpha
					, 64, 64, 64, alpha
					, 0, 0, 0, alpha
					, 0, 0, 0, alpha
					, true
				);
			}
			CResources::verdana22.printUTF8Box(
				m_message
				, m_x+8 +2, m_y+106 +2
				, 402, 180
				, m_align
				, 00.00f, 1.0f
				, 0, 0, 0, alpha*0.15f
			);
			CResources::verdana22.printUTF8Box(
				m_message
				, m_x+8, m_y+106
				, 402, 180
				, m_align
				, 00.00f, 1.0f
				, 0, 0, 0, alpha
			);
		} else
		{
			CResources::verdana22.printUTF8Box(
				m_message
				, m_x+8 +2, m_y+18 +2
				, 402, 266
				, m_align
				, 00.00f, 1.0f
				, 0, 0, 0, alpha*0.15f
			);
			CResources::verdana22.printUTF8Box(
				m_message
				, m_x+8, m_y+18
				, 402, 266
				, m_align
				, 00.00f, 1.0f
				, 0, 0, 0, alpha
			);
		}
	}
}

void CGUIMcaDisplayMessage::drawAll(CIGUIFrameTexture *prevBuffTex, float alpha)
{
	m_renderer->setAlpha(true);
	m_renderer->setTestAlpha(true);
	m_renderer->setScissor(false);
	m_renderer->clearFrontBuffer();
	if (prevBuffTex != NULL) m_renderer->restoreFrameTex(prevBuffTex);
	drawWindow(alpha);

	switch (m_icon)
	{
		case enIcExclam:
			drawExclam(alpha);
			break;
		case enIcSuccess:
			drawSuccess(alpha);
			break;
		case enIcFail:
			drawFail(alpha);
			break;
		default:
			break;
	}

	drawMessage(alpha);
}

int CGUIMcaDisplayMessage::display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur)
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
