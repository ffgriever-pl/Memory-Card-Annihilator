#include "GUIMcaOperWnd.h"
#include "res/resources.h"
#include "GUIMcaMan.h"
#include "GUIMcaWarrningNoCard.h"
#include "GUIMcaGetSize.h"
#include "GUIMcaGetYesNo.h"
#include "GUIMcaOperProgress.h"
#include "GUIMcaGetPath.h"
#include "GUIMcaDisplayMessage.h"
#include "GUIMcaAbout.h"
#include "GUIMcaCardInfo.h"
#include "helpers.h"
#include "Version.h"

const char *CGUIMcaOperWnd::m_menu_options[] = {
	"LNG_OPER_FORMAT",
	"LNG_OPER_UNFORMAT",
	"LNG_OPER_MAKE_IMAGE",
	"LNG_OPER_RESTORE_IMAGE",
	"LNG_OPER_INFO"
};

const char *CGUIMcaOperWnd::m_menu_format_type[] = {
	"LNG_OPER_FAST",
	"LNG_OPER_FULL"
};

bool CGUIMcaOperWnd::checkMessages()
{
	bool windowCalled = false;
	CGUIMcaMan::updateMca();

	if ((m_psx_mode && (CGUIMcaMan::mce_memcards[m_oper_slot].type != CGUIMcaMan::enctPsx && CGUIMcaMan::mce_memcards[m_oper_slot].type != CGUIMcaMan::enctPda)) || (!m_psx_mode && CGUIMcaMan::mce_memcards[m_oper_slot].type != CGUIMcaMan::enctPs2))
	{
		CGUIMcaWarrningNoCard myWarn(m_renderer, m_input, m_timer, 110, 106, m_oper_slot);
		myWarn.display(true);
		m_exit = true;
		windowCalled = true;
		return windowCalled;
	}
	if (m_input_state_new & CIGUIFrameInput::enInDown)
	{
		m_menu_item++;
		if (m_menu_item > 4)
		{
			m_menu_item = 0;
			m_hover_menu.setDest(70, 248 + 26 * m_menu_item, true);
		}
		m_hover_menu.setDest(70, 248 + 26 * m_menu_item);
	}
	else if (m_input_state_new & CIGUIFrameInput::enInUp)
	{
		m_menu_item--;
		if (m_menu_item < 0)
		{
			m_menu_item = 4;
			m_hover_menu.setDest(70, 248 + 26 * m_menu_item, true);
		}
		m_hover_menu.setDest(70, 248 + 26 * m_menu_item);
	}

	if (m_menu_item == 0)
	{
		if (m_input_state_new & CIGUIFrameInput::enInLeft || m_input_state_new & CIGUIFrameInput::enInRight) m_menu_item_format ^= 1;
	}


	if (m_input_state_new & CIGUIFrameInput::enInSelect)
	{
		CGUIMcaCardInfo myCardInfo(m_renderer, m_input, m_timer, 138, 166);
		myCardInfo.displayInfo(true, m_oper_slot, m_psx_mode);
		windowCalled = true;
		return windowCalled;
	}
	else if (m_input_state_new & CIGUIFrameInput::enInStart)
	{
		CGUIMcaAbout myAbout(m_renderer, m_input, m_timer, 110, 106);
		myAbout.display(true);
		windowCalled = true;
	}


	if (m_input_state_new & CIGUIFrameInput::enInCross)
	{
		switch (m_menu_item)
		{
			case 0://format
			{
				int cardSize = 1;
				int totalpages = 1;
				bool skip = false;
				if (!m_psx_mode)
				{
					int defaultsize = CGUIMcaMan::mce_memcards[m_oper_slot].totalPages * CGUIMcaMan::mce_memcards[m_oper_slot].pageSize / 1024 / 1024;
					CGUIMcaGetSize getCardSize(m_renderer, m_input, m_timer, 110, 106, defaultsize);//change to real size
					cardSize = getCardSize.display(true);

					if (cardSize != -1 && cardSize > defaultsize)
					{
						int resYesNo;
						CGUIMcaGetYesNo getYesNo(m_renderer, m_input, m_timer, 110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_SIZE_MISMATCH"), CGUIMcaGetYesNo::enresNo);
						if ((resYesNo = getYesNo.display(m_psx_mode ? true : false)) == CGUIMcaGetYesNo::enresNo)
							skip = true;
					}
				}
				if (cardSize != -1)
				{
					int resYesNo;
					if (!skip)
					{
						CGUIMcaGetYesNo getYesNo(m_renderer, m_input, m_timer, 110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_FORMAT_DATA_LOST"), CGUIMcaGetYesNo::enresNo);
						if ((resYesNo = getYesNo.display(m_psx_mode ? true : false)) == CGUIMcaGetYesNo::enresYes)
						{
							//here call format progress
							if (!m_psx_mode)
							{
								totalpages = (cardSize * 1024 * 1024) / CGUIMcaMan::mce_memcards[m_oper_slot].pageSize;
							}
							CGUIMcaOperProgress formatProgress(m_renderer, m_input, m_timer, 110, 106);
							formatProgress.doFormat(m_oper_slot, m_menu_item_format == 0 ? true : false, m_psx_mode, totalpages);
							m_exit = true;
						}
					}
				}
				windowCalled = true;
				return windowCalled;
			}
			break;
			case 1: //unformat
			{
				int cardSize = 1;
				int totalpages = 1;
				bool skip = false;
				if (!m_psx_mode)
				{
					int defaultsize = CGUIMcaMan::mce_memcards[m_oper_slot].totalPages * CGUIMcaMan::mce_memcards[m_oper_slot].pageSize / 1024 / 1024;
					CGUIMcaGetSize getCardSize(m_renderer, m_input, m_timer, 110, 106, defaultsize);//change to real size
					cardSize = getCardSize.display(true);

					if (cardSize != -1 && cardSize > defaultsize)
					{
						int resYesNo;
						CGUIMcaGetYesNo getYesNo(m_renderer, m_input, m_timer, 110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_SIZE_MISMATCH"), CGUIMcaGetYesNo::enresNo);
						if ((resYesNo = getYesNo.display(m_psx_mode ? true : false)) == CGUIMcaGetYesNo::enresNo)
							skip = true;
					}
				}
				if (cardSize != -1)
				{
					int resYesNo;
					if (!skip)
					{
						CGUIMcaGetYesNo getYesNo(m_renderer, m_input, m_timer, 110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_UNFORMAT_DATA_LOST"), CGUIMcaGetYesNo::enresNo);
						if ((resYesNo = getYesNo.display(m_psx_mode ? true : false)) == CGUIMcaGetYesNo::enresYes)
						{
							//here call format progress
							if (!m_psx_mode)
							{
								totalpages = (cardSize * 1024 * 1024) / CGUIMcaMan::mce_memcards[m_oper_slot].pageSize;
							}
							CGUIMcaOperProgress unformatProgress(m_renderer, m_input, m_timer, 110, 106);
							unformatProgress.doUnformat(m_oper_slot, m_psx_mode, totalpages);
							m_exit = true;
						}
					}
				}
				windowCalled = true;
				return windowCalled;
			}
			break;
			case 2: //create image
			{
				int cardSize = 1;
				int totalpages = 1;
				if (!m_psx_mode)
				{
					int defaultsize = CGUIMcaMan::mce_memcards[m_oper_slot].totalPages * CGUIMcaMan::mce_memcards[m_oper_slot].pageSize / 1024 / 1024;
					CGUIMcaGetSize getCardSize(m_renderer, m_input, m_timer, 110, 106, defaultsize);//change to real size
					cardSize = getCardSize.display(true);
				}
				if (cardSize != -1)
				{
					std::ostringstream operslot;
					operslot << m_oper_slot;

					std::string defname = "memorycard";
					defname += operslot.str();
					if (m_psx_mode)
						defname += ".mcr";
					else
						defname += ".bin";
					CGUIMcaGetPath getPath(m_renderer, m_input, m_timer, 67, 106, defname.c_str());
					if (m_psx_mode)
					{
						getPath.addMaskEntry(".mcr");
						getPath.addMaskEntry(".gme");
						getPath.addMaskEntry(".mem");
						getPath.addMaskEntry(".psx");
						getPath.addMaskEntry(".vmp");
					}
					else
					{
						getPath.addMaskEntry(".vmc");
						getPath.addMaskEntry(".bin");
					}
					getPath.enableMask(true);
					std::string result;
					getPath.doGetName(result, true, m_psx_mode ? true : false);

					//here call create image progress
					if (!result.empty())
					{
						//check if exists
						std::string chkpath = result;
						if (chkpath.substr(0, 5) == "hdd0:")
						{
							u32 pos = chkpath.find('/');
							if (pos != std::string::npos)
							{
								std::string pfsPath = "pfs0:";
								pfsPath += chkpath.substr(pos, chkpath.length() - pos);
								chkpath = pfsPath;
								sio_printf("Mounted path: '%s'\n", chkpath.c_str());
							}
						}
						int chkfd = fioOpen(chkpath.c_str(), O_RDONLY | O_BINARY);
						if (chkfd > 0)
						{
							fioClose(chkfd);
							int resYesNo;
							CGUIMcaGetYesNo getYesNo(m_renderer, m_input, m_timer, 110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_OVERWRITE"), CGUIMcaGetYesNo::enresNo);
							if ((resYesNo = getYesNo.display(m_psx_mode ? true : false)) == CGUIMcaGetYesNo::enresNo)
							{
								windowCalled = true;
								return windowCalled;
							}
						}
						//end check exists
						if (!m_psx_mode)
						{
							totalpages = (cardSize * 1024 * 1024) / CGUIMcaMan::mce_memcards[m_oper_slot].pageSize;
						}
						CGUIMcaOperProgress createProgress(m_renderer, m_input, m_timer, 110, 106);
						createProgress.doCreateImage(m_oper_slot, m_psx_mode, totalpages, result.c_str(), false);
						if (result.substr(0, 5) == "hdd0:") fileXioUmount("pfs0:");
						m_exit = true;
					}
				}
				windowCalled = true;
				return windowCalled;
			}
			break;
			case 3://restore image
			{
				std::string defname = "memorycard";
				defname += m_oper_slot;
				defname += ".bin";
				CGUIMcaGetPath getPath(m_renderer, m_input, m_timer, 67, 106, defname.c_str());
				if (m_psx_mode)
				{
					getPath.addMaskEntry(".mcr");
					getPath.addMaskEntry(".gme");
					getPath.addMaskEntry(".mem");
					getPath.addMaskEntry(".psx");
					getPath.addMaskEntry(".vmp");
				}
				else
				{
					getPath.addMaskEntry(".vmc");
					getPath.addMaskEntry(".bin");
				}
				getPath.enableMask(true);
				std::string result;
				getPath.doGetName(result, false, true);

				//here call create image progress
				if (!result.empty())
				{
					int resYesNo;
					CGUIMcaGetYesNo getYesNo(m_renderer, m_input, m_timer, 110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_RESTORE_DATA_LOST"), CGUIMcaGetYesNo::enresNo);
					if ((resYesNo = getYesNo.display(false)) == CGUIMcaGetYesNo::enresYes)
					{
						//here call restore progress
						CGUIMcaOperProgress restoreProgress(m_renderer, m_input, m_timer, 110, 106);
						restoreProgress.doRestoreImage(m_oper_slot, m_psx_mode, result.c_str(), false);
						if (result.substr(0, 5) == "hdd0:") fileXioUmount("pfs0:");
						m_exit = true;
					}
				}
				windowCalled = true;
				return windowCalled;
			}
			break;
			case 4://card info
			{
				CGUIMcaCardInfo myCardInfo(m_renderer, m_input, m_timer, 138, 166);
				myCardInfo.displayInfo(m_oper_slot, m_psx_mode);
				windowCalled = true;
				return windowCalled;
			}
		}
	}
	return windowCalled;
}

void CGUIMcaOperWnd::setPsxMode(bool psxmode)
{
	m_psx_mode = psxmode;
}

void CGUIMcaOperWnd::setOperSlot(int operslot)
{
	m_oper_slot = operslot;
}

CGUIMcaOperWnd::CGUIMcaOperWnd(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer)
	: CGUIMcaBaseWindow(renderer, input, timer, 0, 0)
	, m_menu_item(0)
	, m_menu_item_format(0)
	, m_psx_mode(false)
	, m_hover_menu(renderer, 70, 248, 450, 24, 0.1f, 600, 255, 255, 255, 32, 32, 32, 0.50f, 0.25f, true)
{
	CResources::m_bgimage.loadTextureBuffer(CResources::bgimg_tm2, CResources::size_bgimg_tm2, true);
	CResources::m_popup_pal.loadTextureBuffer(CResources::popup_pal_tm2, CResources::size_popup_pal_tm2, true);

	m_mca_logo.loadTextureBuffer(CResources::mca_logo_tm2, CResources::size_mca_logo_tm2, true);
	m_mc_bg.loadTextureBuffer(CResources::mc_bg_tm2, CResources::size_mc_bg_tm2, true);
}


CGUIMcaOperWnd::~CGUIMcaOperWnd(void)
{
}

void CGUIMcaOperWnd::drawBackground(float alpha)
{
	m_renderer->drawSpriteT(&CResources::m_bgimage, 0, 0, CResources::m_bgimage.getWidth(), CResources::m_bgimage.getHeight(), 0, 0, CResources::m_bgimage.getWidth(), CResources::m_bgimage.getHeight(), 128, 128, 128, alpha);
}

void CGUIMcaOperWnd::drawMenu(float alpha)
{
	m_hover_menu.drawHover(m_ticks, alpha);

	for (u32 i = 0; i < countof(m_menu_options); i++)
	{
		CResources::mediumFont.printUTF8Shadow(
			CResources::mainLang.getText(m_menu_options[i])
			, 82, 266 +i*26
			, 00.00f, 1.0f
			, 0, 0, 0, alpha
		);
	}

	CResources::mediumFont.printUTF8Shadow(
		CResources::mainLang.getText(m_menu_format_type[m_menu_item_format])
		, 342, 266
		, 00.00f, 1.0f
		, 0, 0, 0, alpha
	);
}

void CGUIMcaOperWnd::drawStatic(float alpha)
{
	m_renderer->drawSpriteT(
		&m_mc_bg
		, 70, 63
		, m_mc_bg.getWidth(), m_mc_bg.getHeight()
		, 0, 0
		, m_mc_bg.getWidth(), m_mc_bg.getHeight()
		, 128, 128, 128, alpha*0.5f
	);
	m_renderer->drawSpriteT(
		&m_mca_logo
		, 76, 110
		, m_mca_logo.getWidth(), m_mca_logo.getHeight()
		, 0, 0
		, m_mca_logo.getWidth(), m_mca_logo.getHeight()
		, 128, 128, 128, alpha
	);
	CResources::versionFont.printUTF8Box(
		VERSION_FULL
		, 62, 136
		, 500, 30
		, CIGUIFrameFont<CGUITexture>::etxAlignRight
		, 0, 1.0f
		, 40, 40, 40, alpha
		, 40, 40, 40, alpha
		, 0, 0, 0, alpha
		, 0, 0, 0, alpha
		, true
	);
	if (m_psx_mode)
	{
		CResources::mediumFont.printUTF8Shadow(
			CResources::mainLang.getText("LNG_OPER_PSX_MEMORY_CARD")
			, 8+74, 220
			, 26.00f, 1.0f
			, 40, 40, 40, alpha
			, 3, 3, alpha * 0.15f
			, 40, 40, 40, alpha
			, 0, 0, 0, alpha
			, 0, 0, 0, alpha
			, true
		);
	} else
	{
		CResources::mediumFont.printUTF8Shadow(
			CResources::mainLang.getText("LNG_OPER_PS2_MEMORY_CARD")
			, 8+74, 220
			, 26.00f, 1.0f
			, 40, 40, 40, alpha
			, 3, 3, alpha * 0.15f
			, 40, 40, 40, alpha
			, 0, 0, 0, alpha
			, 0, 0, 0, alpha
			, true
		);
	}
}

void CGUIMcaOperWnd::drawAll(CIGUIFrameTexture *prevBuffTex, float alpha)
{
	m_renderer->setAlpha(true);
	m_renderer->setTestAlpha(true);
	m_renderer->setScissor(false);
	m_renderer->clearFrontBuffer();
	if (prevBuffTex != NULL) m_renderer->restoreFrameTex(prevBuffTex);
	drawBackground(alpha);
	drawStatic(alpha);
	drawMenu(alpha);
}

int CGUIMcaOperWnd::display(bool blur)
{
	m_exit = false;
	m_input_state_new = 0;
	m_input_state_all = 0;

	m_menu_item = 0;
	m_menu_item_format = 0;
	m_hover_menu.setDest(70, 248 +26*m_menu_item, true);

	CIGUIFrameTexture* prevBuffTex = getFrameTexture();
	checkMessages();
	fadeInOut(prevBuffTex, 25000, false);
	drawLoop(prevBuffTex, CIGUIFrameInput::enInTriangle);
	fadeInOut(prevBuffTex, 25000, true);

	delete prevBuffTex;

	return 1;
}
