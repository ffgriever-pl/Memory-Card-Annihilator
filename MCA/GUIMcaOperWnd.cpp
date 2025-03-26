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

	//if (CGUIMcaMan::mce_memcards[m_oper_slot].type == CGUIMcaMan::enctNone || m_input_state_new & CIGUIFrameInput::enInMenu)
	if ((m_psx_mode && (CGUIMcaMan::mce_memcards[m_oper_slot].type != CGUIMcaMan::enctPsx && CGUIMcaMan::mce_memcards[m_oper_slot].type != CGUIMcaMan::enctPda)) || (!m_psx_mode && CGUIMcaMan::mce_memcards[m_oper_slot].type != CGUIMcaMan::enctPs2))
	{
		//display warrning
		CGUIMcaWarrningNoCard myWarn(110, 106, m_oper_slot);
		myWarn.display(m_renderer, m_input, m_timer, true);
		m_exit_now = true;
		windowCalled = true;
		return windowCalled;
	}
	if (m_input_state_new & CIGUIFrameInput::enInDown)
	{
		m_menu_item++;
		if (m_menu_item > 4)
		{
			m_menu_item = 0;
			//m_hover_menu.setDest(70, 248 +26*m_menu_item);
			m_hover_menu.setDest(70, 248 + 26 * m_menu_item, true);
		}
		//m_hover_menu.setVisibility(true);
		m_hover_menu.setDest(70, 248 + 26 * m_menu_item);
	}
	else if (m_input_state_new & CIGUIFrameInput::enInUp)
	{
		m_menu_item--;
		if (m_menu_item < 0)
		{
			m_menu_item = 4;
			//m_hover_menu.setDest(70, 248 +26*m_menu_item);
			m_hover_menu.setDest(70, 248 + 26 * m_menu_item, true);
		}
		//m_hover_menu.setVisibility(true);
		m_hover_menu.setDest(70, 248 + 26 * m_menu_item);
	}

	if (m_menu_item == 0)
	{
		if (m_input_state_new & CIGUIFrameInput::enInLeft || m_input_state_new & CIGUIFrameInput::enInRight) m_menu_item_format ^= 1;
	}


	if (m_input_state_new & CIGUIFrameInput::enInSwitch)
	{
		/*std::string content = CResources::mainLang.getText("LNG_INFO_ALL");
		CResources::mainLang.replace(&content, "{SLOTNUM}", m_oper_slot+1);
		CResources::mainLang.replace(&content, "{MCTYPE}", m_psx_mode ? CResources::mainLang.getText("LNG_INFO_MC_PSX") : CResources::mainLang.getText("LNG_INFO_MC_PS2"));
		CResources::mainLang.replace(&content, "{PAGESIZE}", CGUIMcaMan::mce_memcards[m_oper_slot].pageSize);
		CResources::mainLang.replace(&content, "{PAGESINBLOCK}", CGUIMcaMan::mce_memcards[m_oper_slot].pagesPerBlock);
		CResources::mainLang.replace(&content, "{PAGESTOTAL}", CGUIMcaMan::mce_memcards[m_oper_slot].totalPages);
		if (m_psx_mode)
		{
			CResources::mainLang.replace(&content, "{CARDSIZE}", CGUIMcaMan::mce_memcards[m_oper_slot].totalPages*CGUIMcaMan::mce_memcards[m_oper_slot].pageSize/1024);
			CResources::mainLang.replace(&content, "{UNIT}", CResources::mainLang.getText("LNG_INFO_UNIT_KB"));
		} else
		{
			CResources::mainLang.replace(&content, "{CARDSIZE}", CGUIMcaMan::mce_memcards[m_oper_slot].totalPages*CGUIMcaMan::mce_memcards[m_oper_slot].pageSize/1024/1024);
			CResources::mainLang.replace(&content, "{UNIT}", CResources::mainLang.getText("LNG_INFO_UNIT_MB"));
		}

		CGUIMcaDisplayMessage myMessage(110, 106
			, content.c_str()
			, CResources::mainLang.getText("LNG_INFO_CAPTION")
			, CGUIMcaDisplayMessage::enIcNone, CIGUIFrameFont<CGUITexture>::etxAlignCenter);
		myMessage.display(m_renderer, m_input, m_timer, true);*/
		CGUIMcaCardInfo myCardInfo(138, 166);
		myCardInfo.displayInfo(m_renderer, m_input, m_timer, true, m_oper_slot, m_psx_mode);
		windowCalled = true;
		return windowCalled;
	}
	else if (m_input_state_new & CIGUIFrameInput::enInMenu)
	{
		CGUIMcaAbout myAbout(110, 106);
		myAbout.display(m_renderer, m_input, m_timer, true);
		windowCalled = true;
	}


	if (m_input_state_new & CIGUIFrameInput::enInOk)
	{
		switch (m_menu_item)
		{
			case 0://format
				//m_menu_item_format
			{
				int cardSize = 1;
				int totalpages = 1;
				bool skip = false;
				if (!m_psx_mode)
				{
					int defaultsize = CGUIMcaMan::mce_memcards[m_oper_slot].totalPages * CGUIMcaMan::mce_memcards[m_oper_slot].pageSize / 1024 / 1024;
					CGUIMcaGetSize getCardSize(110, 106, defaultsize);//change to real size
					cardSize = getCardSize.display(m_renderer, m_input, m_timer, true);

					if (cardSize != -1 && cardSize > defaultsize)
					{
						int resYesNo;
						CGUIMcaGetYesNo getYesNo(110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_SIZE_MISMATCH"), CGUIMcaGetYesNo::enresNo);
						if ((resYesNo = getYesNo.display(m_renderer, m_input, m_timer, m_psx_mode ? true : false)) == CGUIMcaGetYesNo::enresNo)
							skip = true;
					}
				}
				if (cardSize != -1)
				{
					int resYesNo;
					if (!skip)
					{
						CGUIMcaGetYesNo getYesNo(110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_FORMAT_DATA_LOST"), CGUIMcaGetYesNo::enresNo);
						if ((resYesNo = getYesNo.display(m_renderer, m_input, m_timer, m_psx_mode ? true : false)) == CGUIMcaGetYesNo::enresYes)
						{
							//here call format progress
							if (!m_psx_mode)
							{
								totalpages = (cardSize * 1024 * 1024) / CGUIMcaMan::mce_memcards[m_oper_slot].pageSize;
							}
							CGUIMcaOperProgress formatProgress(110, 106);
							formatProgress.doFormat(m_renderer, m_input, m_timer, m_oper_slot, m_menu_item_format == 0 ? true : false, m_psx_mode, totalpages);
							m_exit_now = true;
						}
					}
				}
				windowCalled = true;
				return windowCalled;
			}
			break;
			case 1: //unformat
				//m_menu_item_format
			{
				int cardSize = 1;
				int totalpages = 1;
				bool skip = false;
				if (!m_psx_mode)
				{
					int defaultsize = CGUIMcaMan::mce_memcards[m_oper_slot].totalPages * CGUIMcaMan::mce_memcards[m_oper_slot].pageSize / 1024 / 1024;
					CGUIMcaGetSize getCardSize(110, 106, defaultsize);//change to real size
					cardSize = getCardSize.display(m_renderer, m_input, m_timer, true);

					if (cardSize != -1 && cardSize > defaultsize)
					{
						int resYesNo;
						CGUIMcaGetYesNo getYesNo(110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_SIZE_MISMATCH"), CGUIMcaGetYesNo::enresNo);
						if ((resYesNo = getYesNo.display(m_renderer, m_input, m_timer, m_psx_mode ? true : false)) == CGUIMcaGetYesNo::enresNo)
							skip = true;
					}
				}
				if (cardSize != -1)
				{
					int resYesNo;
					if (!skip)
					{
						CGUIMcaGetYesNo getYesNo(110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_UNFORMAT_DATA_LOST"), CGUIMcaGetYesNo::enresNo);
						if ((resYesNo = getYesNo.display(m_renderer, m_input, m_timer, m_psx_mode ? true : false)) == CGUIMcaGetYesNo::enresYes)
						{
							//here call format progress
							if (!m_psx_mode)
							{
								totalpages = (cardSize * 1024 * 1024) / CGUIMcaMan::mce_memcards[m_oper_slot].pageSize;
							}
							CGUIMcaOperProgress unformatProgress(110, 106);
							unformatProgress.doUnformat(m_renderer, m_input, m_timer, m_oper_slot, m_psx_mode, totalpages);
							m_exit_now = true;
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
					CGUIMcaGetSize getCardSize(110, 106, defaultsize);//change to real size
					cardSize = getCardSize.display(m_renderer, m_input, m_timer, true);
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
					CGUIMcaGetPath getPath(67, 106, defname.c_str());
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
					getPath.doGetName(m_renderer, m_input, m_timer, result, true, m_psx_mode ? true : false);

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
								//fileXioUmount("pfs0:");
								//sio_printf("Mounting '%s' as pfs0\n", chkpath.substr(0, pos).c_str());
								//fileXioMount("pfs0:", chkpath.substr(0, pos).c_str(), FIO_MT_RDWR);
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
							CGUIMcaGetYesNo getYesNo(110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_OVERWRITE"), CGUIMcaGetYesNo::enresNo);
							if ((resYesNo = getYesNo.display(m_renderer, m_input, m_timer, m_psx_mode ? true : false)) == CGUIMcaGetYesNo::enresNo)
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
						CGUIMcaOperProgress createProgress(110, 106);
						createProgress.doCreateImage(m_renderer, m_input, m_timer, m_oper_slot, m_psx_mode, totalpages, result.c_str(), false);
						if (result.substr(0, 5) == "hdd0:") fileXioUmount("pfs0:");
						m_exit_now = true;
					}
				}
				windowCalled = true;
				return windowCalled;
			}
			break;
			case 3://restore image
				//m_menu_item_format
			{
				std::string defname = "memorycard";
				defname += m_oper_slot;
				defname += ".bin";
				CGUIMcaGetPath getPath(67, 106, defname.c_str());
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
				getPath.doGetName(m_renderer, m_input, m_timer, result, false, true);

				//here call create image progress
				if (!result.empty())
				{
					int resYesNo;
					CGUIMcaGetYesNo getYesNo(110, 106, CResources::mainLang.getText("LNG_OPER_QUESTION_RESTORE_DATA_LOST"), CGUIMcaGetYesNo::enresNo);
					if ((resYesNo = getYesNo.display(m_renderer, m_input, m_timer, false)) == CGUIMcaGetYesNo::enresYes)
					{
						//here call restore progress
						CGUIMcaOperProgress restoreProgress(110, 106);
						restoreProgress.doRestoreImage(m_renderer, m_input, m_timer, m_oper_slot, m_psx_mode, result.c_str(), false);
						if (result.substr(0, 5) == "hdd0:") fileXioUmount("pfs0:");
						m_exit_now = true;
					}
				}
				windowCalled = true;
				return windowCalled;
			}
			break;
			case 4://card info
			{
				/*std::string content = CResources::mainLang.getText("LNG_INFO_ALL");
				CResources::mainLang.replace(&content, "{SLOTNUM}", m_oper_slot+1);
				CResources::mainLang.replace(&content, "{MCTYPE}", m_psx_mode ? CResources::mainLang.getText("LNG_INFO_MC_PSX") : CResources::mainLang.getText("LNG_INFO_MC_PS2"));
				CResources::mainLang.replace(&content, "{PAGESIZE}", CGUIMcaMan::mce_memcards[m_oper_slot].pageSize);
				CResources::mainLang.replace(&content, "{PAGESINBLOCK}", CGUIMcaMan::mce_memcards[m_oper_slot].pagesPerBlock);
				CResources::mainLang.replace(&content, "{PAGESTOTAL}", CGUIMcaMan::mce_memcards[m_oper_slot].totalPages);
				if (m_psx_mode)
				{
					CResources::mainLang.replace(&content, "{CARDSIZE}", CGUIMcaMan::mce_memcards[m_oper_slot].totalPages*CGUIMcaMan::mce_memcards[m_oper_slot].pageSize/1024);
					CResources::mainLang.replace(&content, "{UNIT}", CResources::mainLang.getText("LNG_INFO_UNIT_KB"));
				} else
				{
					CResources::mainLang.replace(&content, "{CARDSIZE}", CGUIMcaMan::mce_memcards[m_oper_slot].totalPages*CGUIMcaMan::mce_memcards[m_oper_slot].pageSize/1024/1024);
					CResources::mainLang.replace(&content, "{UNIT}", CResources::mainLang.getText("LNG_INFO_UNIT_MB"));
				}

				CGUIMcaDisplayMessage myMessage(110, 106
					, content.c_str()
					, CResources::mainLang.getText("LNG_INFO_CAPTION")
					, CGUIMcaDisplayMessage::enIcNone, CIGUIFrameFont<CGUITexture>::etxAlignCenter);
				myMessage.display(m_renderer, m_input, m_timer, true);*/
				CGUIMcaCardInfo myCardInfo(138, 166);
				myCardInfo.displayInfo(m_renderer, m_input, m_timer, true, m_oper_slot, m_psx_mode);
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

CGUIMcaOperWnd::CGUIMcaOperWnd(void)
	: m_psx_mode(false)
	, m_exit_now(false)
	, m_hover_menu(70, 248, 450, 24, 0.1f, 600, 255, 255, 255, 32, 32, 32, 0.50f, 0.25f, true)
{
	m_menu_item = 0;
	m_menu_item_format = 0;

	CResources::m_bgimage.loadTextureBuffer(CResources::bgimg_tm2, CResources::size_bgimg_tm2, true);
	CResources::m_popup_pal.loadTextureBuffer(CResources::popup_pal_tm2, CResources::size_popup_pal_tm2, true);

	m_mca_logo.loadTextureBuffer(CResources::mca_logo_tm2, CResources::size_mca_logo_tm2, true);
	m_mc_bg.loadTextureBuffer(CResources::mc_bg_tm2, CResources::size_mc_bg_tm2, true);
}


CGUIMcaOperWnd::~CGUIMcaOperWnd(void)
{
}

void CGUIMcaOperWnd::fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out)
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

void CGUIMcaOperWnd::drawBackground(float alpha)
{
	m_renderer->drawSpriteT(&CResources::m_bgimage, 0, 0, CResources::m_bgimage.getWidth(), CResources::m_bgimage.getHeight(), 0, 0, CResources::m_bgimage.getWidth(), CResources::m_bgimage.getHeight(), 128, 128, 128, alpha);
}

void CGUIMcaOperWnd::drawMenu(float alpha)
{
	m_hover_menu.drawHover(m_renderer, m_ticks, alpha);

	for (u32 i = 0; i < sizeof(m_menu_options)/sizeof(m_menu_options[0]); i++)
	{
		CResources::verdana22.printUTF8(
			CResources::mainLang.getText(m_menu_options[i])
			, 82+2, 266+2 +i*26
			, 00.00f, 1.0f
			, 0, 0, 0, alpha*0.15f
		);
		CResources::verdana22.printUTF8(
			CResources::mainLang.getText(m_menu_options[i])
			, 82, 266 +i*26
			, 00.00f, 1.0f
			, 0, 0, 0, alpha
		);
	}

	CResources::verdana22.printUTF8(
		CResources::mainLang.getText(m_menu_format_type[m_menu_item_format])
		, 342+2, 266+2
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.15f
	);
	CResources::verdana22.printUTF8(
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
	CResources::urwmed20.printUTF8Box(
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
		CResources::verdana22.printUTF8(
			CResources::mainLang.getText("LNG_OPER_PSX_MEMORY_CARD")
			, 8+74+3, 220+3
			, 26.00f, 1.0f
			, 0, 0, 0, alpha*0.15f
		);
		CResources::verdana22.printUTF8(
			CResources::mainLang.getText("LNG_OPER_PSX_MEMORY_CARD")
			, 8+74, 220
			, 26.00f, 1.0f
			, 40, 40, 40, alpha
			, 40, 40, 40, alpha
			, 0, 0, 0, alpha
			, 0, 0, 0, alpha
			, true
		);
	} else
	{
		CResources::verdana22.printUTF8(
			CResources::mainLang.getText("LNG_OPER_PS2_MEMORY_CARD")
			, 8+74+3, 220+3
			, 26.00f, 1.0f
			, 0, 0, 0, alpha*0.15f
		);
		CResources::verdana22.printUTF8(
			CResources::mainLang.getText("LNG_OPER_PS2_MEMORY_CARD")
			, 8+74, 220
			, 26.00f, 1.0f
			, 40, 40, 40, alpha
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

int CGUIMcaOperWnd::display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur)
{
	m_exit_now = false;
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_renderer = renderer;
	m_input = input;
	m_timer = timer;

	m_menu_item = 0;
	m_menu_item_format = 0;
	m_hover_menu.setDest(70, 248 +26*m_menu_item, true);

	CIGUIFrameTexture *prevBuffTex = renderer->getFrameTex();
	m_ticks = 0;
	checkMessages();
	fadeInOut(prevBuffTex, timer, 25000, false);
	u32 currTick = 0, oldTick = 0;
	currTick = oldTick = timer->getTicks();
	do
	{
		if (m_exit_now) break;
		m_ticks = currTick - oldTick;
		input->update();
		m_input_state_new = input->getNew(m_ticks);
		m_input_state_all = input->getAll();

		if (checkMessages())
		{
			currTick = oldTick = timer->getTicks();
			continue;
		}

		drawAll();

		/*static char msg[30];
		if (m_ticks == 0) m_ticks = 1;
		sprintf(msg, "FPS %0.1f", 100000.0f/(float)(m_ticks));
		CResources::verdana22.printUTF8(msg, 10, 26, 0, 0, 128, 128, 0, 1);*/

		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = timer->getTicks();

	} while ((m_input_state_new & CIGUIFrameInput::enInCancel) == 0);
	fadeInOut(prevBuffTex, timer, 25000, true);

	delete prevBuffTex;

	return 1;
}
