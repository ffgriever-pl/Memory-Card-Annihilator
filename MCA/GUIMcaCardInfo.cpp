#include <string>
#include "GUIMcaCardInfo.h"
#include "res/resources.h"
#include "GUIMcaMan.h"

CGUIMcaCardInfo::CGUIMcaCardInfo(float x, float y)
{
	m_x = x;
	m_y = y;
	m_psxmode = false;
}

CGUIMcaCardInfo::CGUIMcaCardInfo(void)
{
	m_psxmode = false;
}


CGUIMcaCardInfo::~CGUIMcaCardInfo(void)
{
}

void CGUIMcaCardInfo::fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out)
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

void CGUIMcaCardInfo::drawMessage(float alpha)
{
	CResources::centurygoth38p.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_CAPTION")
		, m_x+60 +4, m_y+11 +4
		, 310, 50
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.25f
	);
	CResources::centurygoth38p.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_CAPTION")
		, m_x+60, m_y+11
		, 310, 50
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 48, 48, 48, alpha
		, 48, 48, 48, alpha
		, 0, 0, 0, alpha
		, 0, 0, 0, alpha
		, true
	);

	int i = 0;
	const int multfact = 21;
	const int infstartx = 13;
	const int infstarty = 70;
	const int infw = 260;
	const int infh = 22;
	CIGUIFrameFont<CGUITexture>::eAlignment infalign = CIGUIFrameFont<CGUITexture>::etxAlignLeft;
	const int infvalstartx = 170;
	const int infvalstarty = infstarty;
	const int infvalw = 190;
	const int infvalh = infh;
	CIGUIFrameFont<CGUITexture>::eAlignment infvalalign = CIGUIFrameFont<CGUITexture>::etxAlignRight;

	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_SLOT")
		, m_x+infstartx +2, m_y+infstarty +2 +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_SLOT")
		, m_x+infstartx, m_y+infstarty +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
	std::string infslotnum = "{SLOTNUM}";
	CResources::mainLang.replace(&infslotnum, "{SLOTNUM}", m_oper_slot+1);
	CResources::verdana22.printUTF8Box(
		infslotnum.c_str()
		, m_x+infvalstartx +2, m_y+infvalstarty +2 +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		infslotnum.c_str()
		, m_x+infvalstartx, m_y+infvalstarty +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);


	i++;
	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_TYPE")
		, m_x+infstartx +2, m_y+infstarty +2 +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_TYPE")
		, m_x+infstartx, m_y+infstarty +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
	std::string inftype = "{MCTYPE}";
	CResources::mainLang.replace(&inftype, "{MCTYPE}", m_psxmode ? CResources::mainLang.getText("LNG_INFO_MC_PSX") : CResources::mainLang.getText("LNG_INFO_MC_PS2"));
	CResources::verdana22.printUTF8Box(
		inftype.c_str()
		, m_x+infvalstartx +2, m_y+infvalstarty +2 +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		inftype.c_str()
		, m_x+infvalstartx, m_y+infvalstarty +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);


	i++;
	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_PAGESIZE")
		, m_x+infstartx +2, m_y+infstarty +2 +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_PAGESIZE")
		, m_x+infstartx, m_y+infstarty +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
	std::string infpagesize = "{PAGESIZE}";
	CResources::mainLang.replace(&infpagesize, "{PAGESIZE}", CGUIMcaMan::mce_memcards[m_oper_slot].pageSize);
	CResources::verdana22.printUTF8Box(
		infpagesize.c_str()
		, m_x+infvalstartx +2, m_y+infvalstarty +2 +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		infpagesize.c_str()
		, m_x+infvalstartx, m_y+infvalstarty +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);


	i++;
	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_PAGESINBLOCK")
		, m_x+infstartx +2, m_y+infstarty +2 +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_PAGESINBLOCK")
		, m_x+infstartx, m_y+infstarty +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
	std::string infpagesinblock = "{PAGESINBLOCK}";
	CResources::mainLang.replace(&infpagesinblock, "{PAGESINBLOCK}", CGUIMcaMan::mce_memcards[m_oper_slot].pagesPerBlock);
	CResources::verdana22.printUTF8Box(
		infpagesinblock.c_str()
		, m_x+infvalstartx +2, m_y+infvalstarty +2 +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		infpagesinblock.c_str()
		, m_x+infvalstartx, m_y+infvalstarty +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);


	i++;
	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_PAGESTOTAL")
		, m_x+infstartx +2, m_y+infstarty +2 +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		CResources::mainLang.getText("LNG_INFO_PAGESTOTAL")
		, m_x+infstartx, m_y+infstarty +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
	std::string infpagestotal = "{PAGESTOTAL} ({CARDSIZE} {UNIT})";
	CResources::mainLang.replace(&infpagestotal, "{PAGESTOTAL}", CGUIMcaMan::mce_memcards[m_oper_slot].totalPages);
	if (m_psxmode)
	{
		CResources::mainLang.replace(&infpagestotal, "{CARDSIZE}", CGUIMcaMan::mce_memcards[m_oper_slot].totalPages*CGUIMcaMan::mce_memcards[m_oper_slot].pageSize/1024);
		CResources::mainLang.replace(&infpagestotal, "{UNIT}", CResources::mainLang.getText("LNG_INFO_UNIT_KB"));
	} else
	{
		CResources::mainLang.replace(&infpagestotal, "{CARDSIZE}", CGUIMcaMan::mce_memcards[m_oper_slot].totalPages*CGUIMcaMan::mce_memcards[m_oper_slot].pageSize/1024/1024);
		CResources::mainLang.replace(&infpagestotal, "{UNIT}", CResources::mainLang.getText("LNG_INFO_UNIT_MB"));
	}
	CResources::verdana22.printUTF8Box(
		infpagestotal.c_str()
		, m_x+infvalstartx +2, m_y+infvalstarty +2 +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8Box(
		infpagestotal.c_str()
		, m_x+infvalstartx, m_y+infvalstarty +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
}

void CGUIMcaCardInfo::drawAll(CIGUIFrameTexture *prevBuffTex, float alpha)
{
	m_renderer->setAlpha(true);
	m_renderer->setTestAlpha(true);
	m_renderer->setScissor(false);
	m_renderer->clearFrontBuffer();
	if (prevBuffTex != NULL) m_renderer->restoreFrameTex(prevBuffTex);
	drawWindowInfo(alpha);

	drawMessage(alpha);
}

int CGUIMcaCardInfo::display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur)
{
	return displayInfo(renderer, input, timer, blur, 0, false);
}
int CGUIMcaCardInfo::displayInfo(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur, int oper_slot, bool psxmode)
{
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_renderer = renderer;
	m_input = input;
	m_timer = timer;
	m_psxmode = psxmode;
	m_oper_slot = oper_slot;

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
