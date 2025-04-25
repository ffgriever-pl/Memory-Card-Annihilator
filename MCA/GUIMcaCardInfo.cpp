#include <string>
#include "GUIMcaCardInfo.h"
#include "res/resources.h"
#include "GUIMcaMan.h"

CGUIMcaCardInfo::CGUIMcaCardInfo(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y)
	: CGUIMcaPopup(renderer, input, timer, x, y)
	, m_psxmode(false)
{
}

CGUIMcaCardInfo::~CGUIMcaCardInfo(void)
{
}

void CGUIMcaCardInfo::drawMessage(float alpha)
{
	CResources::headerFont.printUTF8BoxShadow(
		CResources::mainLang.getText("LNG_INFO_CAPTION")
		, m_x+60, m_y+11
		, 310, 50
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 48, 48, 48, alpha
		, 4, 4, alpha * 0.25f
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

	CResources::mediumFont.printUTF8BoxShadow(
		CResources::mainLang.getText("LNG_INFO_SLOT")
		, m_x+infstartx, m_y+infstarty +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
	std::string infslotnum = "{SLOTNUM}";
	CResources::mainLang.replace(&infslotnum, "{SLOTNUM}", m_oper_slot+1);
	CResources::mediumFont.printUTF8BoxShadow(
		infslotnum.c_str()
		, m_x+infvalstartx, m_y+infvalstarty +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);


	i++;
	CResources::mediumFont.printUTF8BoxShadow(
		CResources::mainLang.getText("LNG_INFO_TYPE")
		, m_x+infstartx, m_y+infstarty +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
	std::string inftype = "{MCTYPE}";
	CResources::mainLang.replace(&inftype, "{MCTYPE}", m_psxmode ? CResources::mainLang.getText("LNG_INFO_MC_PSX") : CResources::mainLang.getText("LNG_INFO_MC_PS2"));
	CResources::mediumFont.printUTF8BoxShadow(
		inftype.c_str()
		, m_x+infvalstartx, m_y+infvalstarty +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);


	i++;
	CResources::mediumFont.printUTF8BoxShadow(
		CResources::mainLang.getText("LNG_INFO_PAGESIZE")
		, m_x+infstartx, m_y+infstarty +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
	std::string infpagesize = "{PAGESIZE}";
	CResources::mainLang.replace(&infpagesize, "{PAGESIZE}", CGUIMcaMan::mce_memcards[m_oper_slot].pageSize);
	CResources::mediumFont.printUTF8BoxShadow(
		infpagesize.c_str()
		, m_x+infvalstartx, m_y+infvalstarty +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);


	i++;
	CResources::mediumFont.printUTF8BoxShadow(
		CResources::mainLang.getText("LNG_INFO_PAGESINBLOCK")
		, m_x+infstartx, m_y+infstarty +i*multfact
		, infw, infh
		, infalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
	std::string infpagesinblock = "{PAGESINBLOCK}";
	CResources::mainLang.replace(&infpagesinblock, "{PAGESINBLOCK}", CGUIMcaMan::mce_memcards[m_oper_slot].pagesPerBlock);
	CResources::mediumFont.printUTF8BoxShadow(
		infpagesinblock.c_str()
		, m_x+infvalstartx, m_y+infvalstarty +i*multfact
		, infvalw, infvalh
		, infvalalign
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);


	i++;
	CResources::mediumFont.printUTF8BoxShadow(
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
	CResources::mediumFont.printUTF8BoxShadow(
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

int CGUIMcaCardInfo::display(bool blur)
{
	return displayInfo(blur, 0, false);
}
int CGUIMcaCardInfo::displayInfo(bool blur, int oper_slot, bool psxmode)
{
	m_psxmode = psxmode;
	m_oper_slot = oper_slot;

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
	fadeInOut(prevBuffTex, 25000, false);
	drawLoop(prevBuffTex);
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return 1;
}
