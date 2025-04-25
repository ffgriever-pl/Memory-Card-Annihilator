#include <string>
#include "GUIMcaDisplayMessage.h"
#include "res/resources.h"

CGUIMcaDisplayMessage::CGUIMcaDisplayMessage(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y, const char* message, const char* caption, enIconType icon, CIGUIFrameFont<CGUITexture>::eAlignment align)
	: CGUIMcaPopup(renderer, input, timer, x, y)
	, m_message(message)
	, m_caption(caption)
	, m_icon(icon)
	, m_align(align)

{
}

CGUIMcaDisplayMessage::~CGUIMcaDisplayMessage(void)
{
}

void CGUIMcaDisplayMessage::drawMessage(float alpha)
{
	if (m_message)
	{
		if (m_icon != enIcNone || m_caption != NULL)
		{
			if (m_caption != NULL)
			{
				CResources::headerFont.printUTF8BoxShadow(
					m_caption
					, m_x+8, m_y+34
					, 402, 50
					, CIGUIFrameFont<CGUITexture>::etxAlignCenter
					, 00.00f, 1.0f
					, 64, 64, 64, alpha
					, 4, 4, alpha * 0.25f
					, 64, 64, 64, alpha
					, 0, 0, 0, alpha
					, 0, 0, 0, alpha
					, true
				);
			}
			CResources::mediumFont.printUTF8BoxShadow(
				m_message
				, m_x+8, m_y+106
				, 402, 180
				, m_align
				, 00.00f, 1.0f
				, 0, 0, 0, alpha
			);
		} else
		{
			CResources::mediumFont.printUTF8BoxShadow(
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

int CGUIMcaDisplayMessage::display(bool blur)
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
