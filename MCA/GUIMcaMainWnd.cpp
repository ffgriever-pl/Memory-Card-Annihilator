#include "GUIMcaMainWnd.h"
#include "res/resources.h"
#include "GUIMcaMan.h"
#include "GUIMcaAbout.h"
#include "GUIMcaGetPath.h"
#include "GUIMcaDisplayMessage.h"
#include "GUIFramePS2Modules.h"

bool CGUIMcaMainWnd::checkMessages()
{
	bool windowCalled = false;
	CGUIMcaMan::updateMca();
	if (m_slot_chosen != -1)
	{
		if (CGUIMcaMan::mce_memcards[m_slot_chosen].type == CGUIMcaMan::enctNone)
		{
			m_slot_chosen = -1;
			m_hover_slots.setVisibility(false);
		}
	}
	if (m_slot_chosen == -1)
	{
		if (CGUIMcaMan::mce_memcards[0].type != CGUIMcaMan::enctNone)
		{
			m_slot_chosen = 0;
			m_hover_slots.setVisibility(true);
			m_hover_slots.setDest(158, 368, true);
		} else if (CGUIMcaMan::mce_memcards[1].type != CGUIMcaMan::enctNone)
		{
			m_slot_chosen = 1;
			m_hover_slots.setVisibility(true);
			m_hover_slots.setDest(384, 368, true);
		}
	}

	if (
		m_input_state_all & CIGUIFrameInput::enInSwitch //select
		//m_input_state_all & CIGUIFrameInput::enInMenu //start
		//&& m_input_state_all & CIGUIFrameInput::enInSwitch //select
		//&& m_input_state_all & CIGUIFrameInput::enInTabBck //l1
		//&& m_input_state_all & CIGUIFrameInput::enInTabFwd //r1
		//&& m_input_state_all & CIGUIFrameInput::enInTab2Bck //l2
		//&& m_input_state_all & CIGUIFrameInput::enInTab2Fwd //r2
		)
	{
		CGUIMcaGetPath getPath(67,106,"mc0:/BOOT/BOOT.ELF");
		getPath.addMaskEntry(".elf");
		getPath.addMaskEntry(".ELF");

		getPath.enableMask(true);
		std::string result;
		getPath.doGetName(m_renderer, m_input, m_timer, result, false, true, CResources::mainLang.getText("LNG_EXIT_SELECT_ELF"));
						
		//here call create image progress
		if (!result.empty())
		{
		//check if exists
			std::string chkpath = result;
			std::string partition = "";
			if (chkpath.substr(0, 5) == "hdd0:")
			{
				u32 pos = chkpath.find('/');
				if (pos != std::string::npos)
				{
					fileXioUmount("pfs0:");
					sio_printf("Mounting '%s' as pfs0\n", chkpath.substr(0, pos).c_str());
					fileXioMount("pfs0:", chkpath.substr(0, pos).c_str(), FIO_MT_RDONLY);
					std::string pfsPath = "pfs0:";
					pfsPath += chkpath.substr(pos, chkpath.length()-pos);
					partition = chkpath.substr(0, pos);
					chkpath = pfsPath;
					sio_printf("Mounted path: '%s'\n", chkpath.c_str());
					//chkpath = /*chkpath.substr(0, revpos) + std::string("\n") + */"host:" + chkpath.substr(revpos+1, chkpath.length()-revpos-1);
				}
			}
			int chkfd = fioOpen(chkpath.c_str(), O_RDONLY | O_BINARY);
			if (chkfd > 0)
			{
				u32 magic;
				fioRead(chkfd, &magic, 4);
				fioClose(chkfd);
				if (magic != 0x464c457f)
				{
					CGUIMcaDisplayMessage myMessage(110, 106, CResources::mainLang.getText("LNG_EXIT_INVALID"), /*CResources::mainLang.getText("LNG_WARN_CAP")*/NULL, CGUIMcaDisplayMessage::enIcFail, CIGUIFrameFont<CGUITexture>::etxAlignCenter);
					myMessage.display(m_renderer, m_input, m_timer, false);
				} else
				{
					m_timer->deinitTimer();
					m_renderer->deinitRenderer();
					padPortClose(0, 0);
					if (/*chkpath.substr(0, 4) != "mass" && */CResources::iopreset)
					{
						CGUIFramePS2Modules::iopReset(false);
						CGUIFramePS2Modules::loadMcModules();
						
						CGUIFramePS2Modules::iopReset(false);
						CGUIFramePS2Modules::loadUsbModules();
						CGUIFramePS2Modules::loadMcModules();
						CGUIFramePS2Modules::loadCdvdModules();
						//sbv_patch_enable_lmb();
						//sbv_patch_disable_prefix_check();
						if (chkpath.substr(0, 5) == "pfs0:")
						{
							CGUIFramePS2Modules::loadHddModules();
							fileXioMount("pfs0:", partition.c_str(), FIO_MT_RDONLY);
							u32 revpos = chkpath.rfind('/');
							CGUIFramePS2Modules::loadFakehost(chkpath.substr(0, revpos+1).c_str());
							poweroffSetCallback((poweroff_callback)NULL, NULL);
						} else if (chkpath.substr(0, 4) == "mass")
						{
							fio_stat_t chk_stat;
							int currmass = 0;
							while (currmass < 10)
							{
								chkpath[4] = '0'+currmass;
								while(fioGetstat(chkpath.substr(0,7).c_str(), &chk_stat) < 0){nopdelay();nopdelay();}
								int fd = fioOpen(chkpath.c_str(), O_RDONLY | O_BINARY);
								if (fd > 1) { fioClose(fd); break;}
								currmass++;
							}
						}
					}
					/*if (chkpath.substr(0, 4) == "mc0:" || chkpath.substr(0, 4) == "mc1:")
					{
						//CResources::iopreset
						CGUIFramePS2Modules::iopReset();
						CGUIFramePS2Modules::loadMcModules();
						
						CGUIFramePS2Modules::iopReset();
						CGUIFramePS2Modules::loadMcModules();
					}*/
					u8 *boot_elf;
					elf_header_t *eh;
					elf_pheader_t *eph;
					void *pdata;
					static char *argv[2];

					boot_elf = (u8 *)&CResources::loader_elf;
					eh = (elf_header_t *)boot_elf;
					eph = (elf_pheader_t *)(boot_elf + eh->phoff);

					for (int i = 0; i < eh->phnum; i++)
					{
						if (eph[i].type != ELF_PT_LOAD)
						continue;

						pdata = (void *)(boot_elf + eph[i].offset);
						memcpy(eph[i].vaddr, pdata, eph[i].filesz);

						if (eph[i].memsz > eph[i].filesz)
						{
							memset( (void*)((u32)eph[i].vaddr + eph[i].filesz), 0,
									eph[i].memsz - eph[i].filesz);
						}
					}
					//m_timer->deinitTimer();
					//m_renderer->deinitRenderer();
					//padPortClose(0, 0);
					//fioExit();
					//SifInitRpc(0);
					//SifExitRpc();
					fioExit();
					//SifExitIopHeap();
					//SifLoadFileExit();
					SifExitRpc();
					//SifExitCmd();

					FlushCache(0);
					FlushCache(2);

					argv[0] = (char*)chkpath.c_str();
					if (partition == "") argv[1] = "\0";
					else argv[1] = (char*)partition.c_str();
					
					ExecPS2((void *)eh->entry, 0, 2, argv);
					while(1){;}
				}
			} else
			{
				CGUIMcaDisplayMessage myMessage(110, 106, CResources::mainLang.getText("LNG_EXIT_FAILED"), /*CResources::mainLang.getText("LNG_WARN_CAP")*/NULL, CGUIMcaDisplayMessage::enIcFail, CIGUIFrameFont<CGUITexture>::etxAlignCenter);
				myMessage.display(m_renderer, m_input, m_timer, false);
			}
		}
		windowCalled = true;
		return windowCalled;
	}

	if (m_input_state_new & CIGUIFrameInput::enInLeft)
	{
		m_slot_chosen = 0;
		if (CGUIMcaMan::mce_memcards[0].type != CGUIMcaMan::enctNone)
		{
			m_slot_chosen = 0;
			m_hover_slots.setDest(158, 368);
		}
	} else if (m_input_state_new & CIGUIFrameInput::enInRight)
	{
		if (CGUIMcaMan::mce_memcards[1].type != CGUIMcaMan::enctNone)
		{
			m_slot_chosen = 1;
			m_hover_slots.setDest(384, 368);
		}
	}

	if (m_input_state_new & CIGUIFrameInput::enInOk)
	{
		if (m_slot_chosen != -1)
		{
			if (CGUIMcaMan::mce_memcards[m_slot_chosen].type == CGUIMcaMan::enctPs2)
			{
				m_oper_wnd.setPsxMode(false);
				m_oper_wnd.setOperSlot(m_slot_chosen);
				m_oper_wnd.display(m_renderer, m_input, m_timer);
				windowCalled = true;
			} else if (CGUIMcaMan::mce_memcards[m_slot_chosen].type == CGUIMcaMan::enctPsx || CGUIMcaMan::mce_memcards[m_slot_chosen].type == CGUIMcaMan::enctPda)
			{
				m_oper_wnd.setPsxMode(true);
				m_oper_wnd.setOperSlot(m_slot_chosen);
				m_oper_wnd.display(m_renderer, m_input, m_timer);
				windowCalled = true;
			}
		}
	} else if (m_input_state_new & CIGUIFrameInput::enInMenu)
	{
		CGUIMcaAbout myAbout(110, 106);
		myAbout.display(m_renderer, m_input, m_timer, true);
		windowCalled = true;
	}
	
	return windowCalled;
}

CGUIMcaMainWnd::CGUIMcaMainWnd(void)
	: m_hover_slots(158, 368, 110, 19, 0.5f, 100, 255, 255, 255, 32, 32, 32, 0.50f, 0.25f, true)
{
	m_slot_chosen = -1;
	m_hover_slots.setVisibility(false);

	CResources::m_bgimage.loadTextureBuffer(CResources::bgimg_tm2, CResources::size_bgimg_tm2, true);
	CResources::m_popup_pal.loadTextureBuffer(CResources::popup_pal_tm2, CResources::size_popup_pal_tm2, true);
	
	m_slot[0].loadTextureBuffer(CResources::slot1_tm2, CResources::size_slot1_tm2, true);
	m_slot[1].loadTextureBuffer(CResources::slot2_tm2, CResources::size_slot2_tm2, true);
	m_wyb_slot.loadTextureBuffer(CResources::wyb_slot_tm2, CResources::size_wyb_slot_tm2, true);
	m_icon_ps2.loadTextureBuffer(CResources::ikona_mc_ps2_tm2, CResources::size_ikona_mc_ps2_tm2, true);
	m_icon_psx.loadTextureBuffer(CResources::ikona_mc_psx_tm2, CResources::size_ikona_mc_psx_tm2, true);
	m_icon_unk.loadTextureBuffer(CResources::ikona_mc_unk_tm2, CResources::size_ikona_mc_unk_tm2, true);
}


CGUIMcaMainWnd::~CGUIMcaMainWnd(void)
{
}

void CGUIMcaMainWnd::fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out)
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

void CGUIMcaMainWnd::drawBackground(float alpha)
{
	m_renderer->drawSpriteT(&CResources::m_bgimage, 0, 0, CResources::m_bgimage.getWidth(), CResources::m_bgimage.getHeight(), 0, 0, CResources::m_bgimage.getWidth(), CResources::m_bgimage.getHeight(), 128, 128, 128, alpha);
}

void CGUIMcaMainWnd::drawSlots(float alpha)
{
	m_hover_slots.drawHover(m_renderer, m_ticks, alpha);
	m_renderer->drawSpriteT(
		&m_slot[0]
		, 146, 369
		, m_slot[0].getWidth(), m_slot[0].getHeight()
		, 0, 0
		, m_slot[0].getWidth(), m_slot[0].getHeight()
		, 128, 128, 128, alpha
	);

	m_renderer->drawSpriteT(
		&m_slot[1]
		, 384, 369
		, m_slot[1].getWidth(), m_slot[1].getHeight()
		, 0, 0
		, m_slot[1].getWidth(), m_slot[1].getHeight()
		, 128, 128, 128, alpha
	);
}

void CGUIMcaMainWnd::drawMcIcons(float alpha)
{
	if (CGUIMcaMan::mce_memcards[0].type == CGUIMcaMan::enctPs2)
	{
		m_renderer->drawSpriteT(
			&m_icon_ps2
			, 126, 200
			, m_icon_ps2.getWidth(), m_icon_ps2.getHeight()
			, 0, 0
			, m_icon_ps2.getWidth(), m_icon_ps2.getHeight()
			, 128, 128, 128, alpha
		);
	} else if (CGUIMcaMan::mce_memcards[0].type == CGUIMcaMan::enctPsx || CGUIMcaMan::mce_memcards[0].type == CGUIMcaMan::enctPda)
	{
		m_renderer->drawSpriteT(
			&m_icon_psx
			, 126, 201
			, m_icon_psx.getWidth(), m_icon_psx.getHeight()
			, 0, 0
			, m_icon_psx.getWidth(), m_icon_psx.getHeight()
			, 128, 128, 128, alpha
		);
	} else
	{
		m_renderer->drawSpriteT(
			&m_icon_unk
			, 108, 212
			, m_icon_unk.getWidth(), m_icon_unk.getHeight()
			, 0, 0
			, m_icon_unk.getWidth(), m_icon_unk.getHeight()
			, 128, 128, 128, alpha
		);
	}

	if (CGUIMcaMan::mce_memcards[1].type == CGUIMcaMan::enctPs2)
	{
		m_renderer->drawSpriteT(
			&m_icon_ps2
			, 352, 200
			, m_icon_ps2.getWidth(), m_icon_ps2.getHeight()
			, 0, 0
			, m_icon_ps2.getWidth(), m_icon_ps2.getHeight()
			, 128, 128, 128, alpha
		);
	} else if (CGUIMcaMan::mce_memcards[1].type == CGUIMcaMan::enctPsx || CGUIMcaMan::mce_memcards[1].type == CGUIMcaMan::enctPda)
	{
		m_renderer->drawSpriteT(
			&m_icon_psx
			, 352, 201
			, m_icon_psx.getWidth(), m_icon_psx.getHeight()
			, 0, 0
			, m_icon_psx.getWidth(), m_icon_psx.getHeight()
			, 128, 128, 128, alpha
		);
	} else
	{
		m_renderer->drawSpriteT(
			&m_icon_unk
			, 336, 212
			, m_icon_unk.getWidth(), m_icon_unk.getHeight()
			, 0, 0
			, m_icon_unk.getWidth(), m_icon_unk.getHeight()
			, 128, 128, 128, alpha
		);
	}
}

void CGUIMcaMainWnd::drawChoseSlot(float alpha)
{
	/*m_renderer->drawSpriteT(
		&m_wyb_slot
		, 140, 120+40
		, m_wyb_slot.getWidth(), m_wyb_slot.getHeight()
		, 0, 0
		, m_wyb_slot.getWidth(), m_wyb_slot.getHeight()
		, 128, 128, 128, alpha
	);*/
	CResources::centurygoth38p.printUTF8Box(
		CResources::mainLang.getText("LNG_MWND_CHOOSE_SLOT")//"Wybierz slot"
		, 100+4, 112+4
		, 440, 60
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 0, 0, 0, alpha*0.25f
	);
	CResources::centurygoth38p.printUTF8Box(
		CResources::mainLang.getText("LNG_MWND_CHOOSE_SLOT")//"Wybierz slot"
		, 100, 112
		, 440, 60
		, CIGUIFrameFont<CGUITexture>::etxAlignCenter
		, 00.00f, 1.0f
		, 64, 64, 64, alpha
		, 64, 64, 64, alpha
		, 0, 0, 0, alpha
		, 0, 0, 0, alpha
		, true
	);
	/*CResources::verdana22.printUTF8("Test tylko...", 50+2, 90+2, 0, 1, 0, 0, 0, alpha*0.25f);
	CResources::verdana22.printUTF8("Test tylko...", 50, 90, 0, 1, 0, 0, 0, alpha);

	CResources::verdana22.printUTF8Box(
		"To jest następny przykładowy tekst napisany tylko po to, żeby sprawdzić, czy wszystko działa tak, jak działać powinno.\n"
		"W każdym razie wydaje mi się, że teraz jest już chyba lepiej, jak było wcześniej. Zarówno cyferki: 1, 2, 3, 9... Jak i literki wyświetlane są jak trzeba."
		, 30+2, 180+2
		, 580, 260
		, CIGUIFrameFont<CGUITexture>::etxAlignJustify
		, 0, 1
		, 0, 0, 0, alpha*0.25f
	);
	CResources::verdana22.printUTF8Box(
		"To jest następny przykładowy tekst napisany tylko po to, żeby sprawdzić, czy wszystko działa tak, jak działać powinno.\n"
		"W każdym razie wydaje mi się, że teraz jest już chyba lepiej, jak było wcześniej. Zarówno cyferki: 1, 2, 3, 9... Jak i literki wyświetlane są jak trzeba."
		, 30, 180
		, 580, 260
		, CIGUIFrameFont<CGUITexture>::etxAlignJustify
		, 0, 1
		, 16, 0, 16, alpha
		, 16, 0, 16, alpha
		, 0, 0, 0, alpha
		, 0, 0, 0, alpha
		, true
	);*/
}

void CGUIMcaMainWnd::drawAll(CIGUIFrameTexture *prevBuffTex, float alpha)
{
	m_renderer->setAlpha(true);
	m_renderer->setTestAlpha(true);
	m_renderer->setScissor(false);
	m_renderer->clearFrontBuffer();
	if (prevBuffTex != NULL) m_renderer->restoreFrameTex(prevBuffTex);
	drawBackground(alpha);
	drawMcIcons(alpha);
	drawChoseSlot(alpha);
	drawSlots(alpha);
}
#include "GUIMcaGetPath.h"
#include "GUIMcaVkbd.h"
int CGUIMcaMainWnd::display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur)
{
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_renderer = renderer;
	m_input = input;
	m_timer = timer;

	CIGUIFrameTexture *prevBuffTex = NULL;//renderer->getFrameTex();
	m_ticks = 0;
	checkMessages();
	fadeInOut(prevBuffTex, timer, 25000, false);
	/*CGUIMcaGetPath myPath(110,106,"dupa");
	std::string getpath;
	myPath.doGetName(renderer, input, timer, getpath, false, true);*/
	/*CGUIMcaVkbd myKbd(74, 153);
	std::string kbdret;
	myKbd.getEntry(m_renderer, m_input, m_timer, "test.bin", kbdret, 32, true);*/

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

		drawAll();
		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = timer->getTicks();

		/*if (m_input_state_new & CIGUIFrameInput::enInSwitch)
		{
			u64 head[] = {
				0x00010004324d4954, 0x0000000000000000,
				0x00000000000f0030, 0x00000030000f0000,
				0x0200028002000100, 0x0000000268100000,
				0x0000000000000260, 0x0000000000000000
			};
			CGUIFrameTexturePS2 *test = (CGUIFrameTexturePS2 *)renderer->getFrameTex();
			int fd = open("host:test.bin.tm2", O_WRONLY | O_CREAT | O_TRUNC | O_BINARY);
			write(fd, head, sizeof(head));
			write(fd, (void*)test->getTexPointer()->Mem, gsKit_texture_size_ee(test->getTexPointer()->Width, test->getTexPointer()->Height, test->getTexPointer()->PSM));
			close(fd);

			delete test;
		}*/

	} while (/*(m_input_state_new & CIGUIFrameInput::enInCancel) == 0*/1);
	fadeInOut(prevBuffTex, timer, 25000, true);

	//delete prevBuffTex;

	return 1;
}
