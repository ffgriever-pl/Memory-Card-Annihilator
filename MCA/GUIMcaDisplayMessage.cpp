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
	CIGUIFrameTexture* prevBuffTex = getFrameTexture(blur);

	fadeInOut(prevBuffTex, 25000, false);
	drawLoop(prevBuffTex);
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return 1;
}
