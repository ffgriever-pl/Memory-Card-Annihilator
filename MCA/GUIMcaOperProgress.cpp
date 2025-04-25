#include <string>
#include "GUIMcaOperProgress.h"
#include "res/resources.h"

CGUIMcaOperProgress::CGUIMcaOperProgress(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y)
	: CGUIMcaPopup(renderer, input, timer, x, y)
	, m_result(enresFail)
	, m_default(enresFail)
	, m_locked(true)
	, m_progressBar(renderer, x + 25, y + 224)
{
}

CGUIMcaOperProgress::~CGUIMcaOperProgress(void)
{
}

int CGUIMcaOperProgress::doOperation(const CGUIMcaMan::opParams& params, void(*operation)(const CGUIMcaMan::opParams&), float blur)
{
	resetProgress();

	CIGUIFrameTexture* prevBuffTex = getFrameTexture(blur);

	fadeInOut(prevBuffTex, 25000, false);
	operation(params);
	drawLoop(prevBuffTex, CIGUIFrameInput::enInTriangle | CIGUIFrameInput::enInCross);
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return m_result;
}

bool CGUIMcaOperProgress::checkMessages()
{
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

	if (m_locked)
		m_input_state_new = 0; //operation in progress, do not allow exit

	if (CGUIMcaMan::progressBarData.finished == 1)
		m_locked = false;
		
	return false;
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

void CGUIMcaOperProgress::resetProgress()
{
	m_result = m_default;
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_progressBar.setProgress(0);
	CGUIMcaMan::progressBarData.error = 0;
	CGUIMcaMan::progressBarData.promil = 0;
	CGUIMcaMan::progressBarData.finished = 0;
}

int CGUIMcaOperProgress::doFormat(int slot, bool fast, bool psx, int pagestotal, bool blur)
{
	return doOperation(CGUIMcaMan::opParams(slot, psx, fast, pagestotal, ""), CGUIMcaMan::doFormat, blur);
}

int CGUIMcaOperProgress::doUnformat(int slot, bool psx, int pagestotal, bool blur)
{
	return doOperation(CGUIMcaMan::opParams(slot, psx, false, pagestotal, ""), CGUIMcaMan::doUnformat, blur);
}

int CGUIMcaOperProgress::doCreateImage(int slot, bool psx, int pagestotal, const char *path, bool blur)
{
	return doOperation(CGUIMcaMan::opParams(slot, psx, false, pagestotal, path), CGUIMcaMan::doCreateImage, blur);
}

int CGUIMcaOperProgress::doRestoreImage(int slot, bool psx, const char *path, bool blur)
{
	return doOperation(CGUIMcaMan::opParams(slot, psx, false, 0, path), CGUIMcaMan::doRestoreImage, blur);
}
