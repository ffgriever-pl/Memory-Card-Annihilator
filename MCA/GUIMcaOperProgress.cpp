#include <string>
#include "GUIMcaOperProgress.h"
#include "GUIMcaMan.h"
#include "res/resources.h"

CGUIMcaOperProgress::CGUIMcaOperProgress(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y)
	: CGUIMcaPopup(renderer, input, timer, x, y)
	, m_result(enresFail)
	, m_default(enresFail)
	, m_return(false)
	, m_locked(true)
	, m_progressBar(renderer, x + 25, y + 224)
{
}

CGUIMcaOperProgress::~CGUIMcaOperProgress(void)
{
}

bool CGUIMcaOperProgress::checkMessages()
{
	bool windowCalled = false;

	CGUIMcaMan::getProgress();

	m_progressBar.setProgress(CGUIMcaMan::progressBarData.promil/1000.0f);
	if (CGUIMcaMan::progressBarData.finished == 0)
	{
		m_progressBar.setState(CGUIMcaProgressBar::ensPending);
	} else
	{
		if (CGUIMcaMan::progressBarData.error == 0)
		{
			m_progressBar.setState(CGUIMcaProgressBar::ensSuccess);
		} else
		{
			m_progressBar.setState(CGUIMcaProgressBar::ensFail);
		}
	}

	if (!m_locked)
	{
		if (m_input_state_new & CIGUIFrameInput::enInTriangle)
		{
			m_return = true;
		} else if (m_input_state_new & CIGUIFrameInput::enInCross)
		{
			m_return = true;
		}
	}

	if (CGUIMcaMan::progressBarData.finished == 1)
	{
		m_locked = false;
	}
		
	return windowCalled;
}

void CGUIMcaOperProgress::drawMessage(float alpha)
{
	const char* message;
	if (CGUIMcaMan::progressBarData.finished == 0)
	{
		message = CResources::mainLang.getText("LNG_PROGRESS_DO_NOT_REMOVE");
	} else
	{
		if (CGUIMcaMan::progressBarData.error == 0)
		{
			message = CResources::mainLang.getText("LNG_PROGRESS_SUCCESS");
		} else
		{
			message = CResources::mainLang.getText("LNG_PROGRESS_FAIL");
		}
	}
	CResources::mediumFont.printUTF8BoxShadow(
		message
		, m_x+8, m_y+106
		, 402, 100
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
}

void CGUIMcaOperProgress::drawAll(CIGUIFrameTexture *prevBuffTex, float alpha)
{
	m_renderer->setAlpha(true);
	m_renderer->setTestAlpha(true);
	m_renderer->setScissor(false);
	m_renderer->clearFrontBuffer();
	if (prevBuffTex != NULL) m_renderer->restoreFrameTex(prevBuffTex);
	drawWindow(alpha);

	if (CGUIMcaMan::progressBarData.finished == 0)
	{
		drawExclam(alpha);
	} else
	{
		if (CGUIMcaMan::progressBarData.error == 0)
		{
			drawSuccess(alpha);
		} else
		{
			drawFail(alpha);
		}
	}

	drawMessage(alpha);
	m_progressBar.display(alpha);
}

int CGUIMcaOperProgress::display(bool blur)
{
	return 0;
}

int CGUIMcaOperProgress::doFormat(int slot, bool fast, bool psx, int pagestotal, bool blur)
{
	m_result = m_default;
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_progressBar.setProgress(0);
	CGUIMcaMan::progressBarData.error = 0;
	CGUIMcaMan::progressBarData.promil = 0;
	CGUIMcaMan::progressBarData.finished = 0;


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

	m_ticks = 0;
	fadeInOut(prevBuffTex, 25000, false);

	CGUIMcaMan::doFormat(slot, psx, fast, pagestotal);

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
			continue;
		}

		drawAll(prevBuffTex);
		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = m_timer->getTicks();

	} while ( !m_return );
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return m_result;
}

int CGUIMcaOperProgress::doUnformat(int slot, bool psx, int pagestotal, bool blur)
{
	m_result = m_default;
	m_input_state_new = 0;
	m_input_state_all = 0;

	m_progressBar.setProgress(0);
	CGUIMcaMan::progressBarData.error = 0;
	CGUIMcaMan::progressBarData.promil = 0;
	CGUIMcaMan::progressBarData.finished = 0;

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

	m_ticks = 0;
	fadeInOut(prevBuffTex, 25000, false);

	CGUIMcaMan::doUnformat(slot, psx, pagestotal);

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
			continue;
		}

		drawAll(prevBuffTex);
		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = m_timer->getTicks();

	} while ( !m_return );
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return m_result;
}

int CGUIMcaOperProgress::doCreateImage(int slot, bool psx, int pagestotal, const char *path, bool blur)
{
	m_result = m_default;
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_progressBar.setProgress(0);
	CGUIMcaMan::progressBarData.error = 0;
	CGUIMcaMan::progressBarData.promil = 0;
	CGUIMcaMan::progressBarData.finished = 0;

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

	m_ticks = 0;
	fadeInOut(prevBuffTex, 25000, false);

	CGUIMcaMan::doCreateImage(slot, psx, pagestotal, path);

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
			continue;
		}

		drawAll(prevBuffTex);
		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = m_timer->getTicks();

	} while ( !m_return );
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return m_result;
}

int CGUIMcaOperProgress::doRestoreImage(int slot, bool psx, const char *path, bool blur)
{
	m_result = m_default;
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_progressBar.setProgress(0);
	CGUIMcaMan::progressBarData.error = 0;
	CGUIMcaMan::progressBarData.promil = 0;
	CGUIMcaMan::progressBarData.finished = 0;

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

	m_ticks = 0;
	fadeInOut(prevBuffTex, 25000, false);

	CGUIMcaMan::doRestoreImage(slot, psx, path);

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
			continue;
		}

		drawAll(prevBuffTex);
		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = m_timer->getTicks();

	} while ( !m_return );
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return m_result;
}
