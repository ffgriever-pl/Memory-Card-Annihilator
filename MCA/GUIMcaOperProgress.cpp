#include "GUIMcaOperProgress.h"
#include "GUIMcaMan.h"
#include "res/resources.h"
#include <string>

CGUIMcaOperProgress::CGUIMcaOperProgress(float x, float y)
	: m_progressBar(NULL)
{
	m_x = x;
	m_y = y;
	m_return = false;
	m_locked = true;
	m_progressBar = new CGUIMcaProgressBar(m_x+25, m_y+224);
}

CGUIMcaOperProgress::CGUIMcaOperProgress(void)
	: m_progressBar(NULL)
{
	m_result = enresFail;
	m_return = false;
	m_locked = true;
}


CGUIMcaOperProgress::~CGUIMcaOperProgress(void)
{
	if (m_progressBar) delete m_progressBar;
}

bool CGUIMcaOperProgress::checkMessages()
{
	bool windowCalled = false;

	CGUIMcaMan::getProgress();

	if (m_progressBar)
	{
		m_progressBar->setProgress(CGUIMcaMan::progressBarData.promil/1000.0f);
		if (CGUIMcaMan::progressBarData.finished == 0)
		{
			//m_progressBar->setColor(255, 133, 112, 244, 116, 96, 207, 31, 0, 149, 22, 0);
			m_progressBar->setState(CGUIMcaProgressBar::ensPending);
		} else
		{
			if (CGUIMcaMan::progressBarData.error == 0)
			{
				//m_progressBar->setColor(109, 243, 107, 109, 210, 107, 3, 170, 0, 2, 105, 0);
				m_progressBar->setState(CGUIMcaProgressBar::ensSuccess);
			} else
			{
				//m_progressBar->setColor(165, 165, 165, 124, 124, 124, 0, 0, 0, 0, 0, 0);
				m_progressBar->setState(CGUIMcaProgressBar::ensFail);
				//m_progressBar->setColor(121, 135, 255, 82, 104, 255, 9, 43, 255, 0, 38, 203);
			}
		}
	}

	if (!m_locked)
	{
		if (m_input_state_new & CIGUIFrameInput::enInTriangle)
		{
			m_return = true;
			//m_result = ;
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

void CGUIMcaOperProgress::fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out)
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
	CResources::verdana22.printUTF8Box(
		message
		, m_x+8 +2, m_y+106 +2
		, 402, 110
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
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
	if (m_progressBar) m_progressBar->display(m_renderer, alpha);
}

int CGUIMcaOperProgress::display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur)
{
	return 0;
}

int CGUIMcaOperProgress::doFormat(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, int slot, bool fast, bool psx, int pagestotal, bool blur)
{
	m_result = m_default;
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_renderer = renderer;
	m_input = input;
	m_timer = timer;
	if (m_progressBar) m_progressBar->setProgress(0);
	CGUIMcaMan::progressBarData.error = 0;
	CGUIMcaMan::progressBarData.promil = 0;
	CGUIMcaMan::progressBarData.finished = 0;


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

	CGUIMcaMan::doFormat(slot, psx, fast, pagestotal);

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

int CGUIMcaOperProgress::doUnformat(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, int slot, bool psx, int pagestotal, bool blur)
{
	m_result = m_default;
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_renderer = renderer;
	m_input = input;
	m_timer = timer;
	if (m_progressBar) m_progressBar->setProgress(0);
	CGUIMcaMan::progressBarData.error = 0;
	CGUIMcaMan::progressBarData.promil = 0;
	CGUIMcaMan::progressBarData.finished = 0;

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

	CGUIMcaMan::doUnformat(slot, psx, pagestotal);

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

int CGUIMcaOperProgress::doCreateImage(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, int slot, bool psx, int pagestotal, const char *path, bool blur)
{
	m_result = m_default;
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_renderer = renderer;
	m_input = input;
	m_timer = timer;
	if (m_progressBar) m_progressBar->setProgress(0);
	CGUIMcaMan::progressBarData.error = 0;
	CGUIMcaMan::progressBarData.promil = 0;
	CGUIMcaMan::progressBarData.finished = 0;

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

	CGUIMcaMan::doCreateImage(slot, psx, pagestotal, path);

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

int CGUIMcaOperProgress::doRestoreImage(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, int slot, bool psx, const char *path, bool blur)
{
	m_result = m_default;
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_renderer = renderer;
	m_input = input;
	m_timer = timer;
	if (m_progressBar) m_progressBar->setProgress(0);
	CGUIMcaMan::progressBarData.error = 0;
	CGUIMcaMan::progressBarData.promil = 0;
	CGUIMcaMan::progressBarData.finished = 0;

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

	CGUIMcaMan::doRestoreImage(slot, psx, path);

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
