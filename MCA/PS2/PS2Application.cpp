#include <osd_config.h>
#include "PS2Application.h"
#include "../Include/GUIFrameTimerPS2.h"
#include "../Include/GUIFrameRendererPS2.h"
#include "../Include/GUIFrameInputPS2.h"
#include "../Include/GUIFramePS2Modules.h"
#include "IGUIFrameFont.h"
#include "../res/resources.h"
#include "../GUIMcaMainWnd.h"
#include "../GUIMcaMan.h"

CPS2Application::CPS2Application(void)
{
}

CPS2Application::~CPS2Application(void)
{
}

CPS2Application* CPS2Application::getInstance()
{
	if (m_pInstance == NULL)
		m_pInstance = new CPS2Application;

	return m_pInstance;
}

void CPS2Application::delInstance()
{
	if (m_pInstance != NULL)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}
#include <fstream>
int CPS2Application::main(int argc, char *argv[])
{
	CResources::iopreset = true;
	for (int i = 1; i < argc; i++)
	{
		std::string curarg = argv[i];
	/// El_isra: if mechaemu is enabled, ignore iop reboot disable as it will break the feature
	/// We need to replace secrman for that feature to work
#ifndef MGMODE
		if (curarg == "-noiopreset") CResources::iopreset = false;
#endif
	}
	
	setBootPath(argv[0]);
	printf("BOOT path: %s\n", CResources::boot_path.c_str());
	std::string langfile = CResources::boot_path + "lang.lng";
	if (!loadLanguage(langfile)){
		std::vector<std::string> languageFilesNames = {
			"lang.lng", // Japanese has no support in the font
			"lang_en.lng",
			"lang_fr.lng",
			"lang_es.lng",
			"lang_de.lng",
			"lang_it.lng",
			"lang_du.lng",
			"lang_pt.lng",
			"lang_ru.lng",	// Requires XEB+ 2024 onwards to be detected, else, it will default to 1 (English)
			"lang.lng",	// Korean has no support in the font
			"lang.lng", // Chinese has no support in the font
			"lang.lng" // Chinese has no support in the font
		}
		if (systemLanguage >= 1 && systemLanguage <= 8) {
			std::string langfile = CResources::boot_path + languageFilesNames[systemLanguage];
			loadLanguage(langfile);
		} else {
			// Handle unsupported language case (optional)
			std::string langfile = CResources::boot_path + languageFilesNames[0]; // Defaults to lang_ru.lng
			loadLanguage(langfile);
		}
	}

	ResetEE(0xffffffff);
	CGUIFramePS2Modules::initPS2Iop(CResources::iopreset, true);
	CGUIFramePS2Modules::loadMcModules();
	CGUIFramePS2Modules::loadUsbModules();
	CGUIFramePS2Modules::loadCdvdModules();
	CGUIMcaMan::initMca();

	CGUIFrameTimerPS2 ps2Timer;
	CGUIFrameRendererPS2 ps2renderer;
	CGUIFrameInputPS2 ps2input;
	ps2Timer.initTimer();

	ps2input.update();
	u32 input_state_all = ps2input.getAll();
	if (input_state_all & CGUIFrameInputPS2::enInLeft)
	{//NTSC
		ps2renderer.initRenderer(640, 448, 640, 512, GS_PSM_CT24, 0, GS_MODE_NTSC, 0);
	} else if (input_state_all & CGUIFrameInputPS2::enInRight)
	{//PAL
		ps2renderer.initRenderer(640, 512, 640, 512, GS_PSM_CT24, 0, GS_MODE_PAL, 0);
	} else if (input_state_all & CGUIFrameInputPS2::enInUp)
	{//VGA 640x480@75
		ps2renderer.initRenderer(640, 480, 640, 512, GS_PSM_CT24, 0, GS_MODE_VGA_640_75, 0);
	} else
	{//automatic
		ps2renderer.initRenderer(0, 0, 640, 512, GS_PSM_CT24, 0, GS_MODE_PAL, CGUIFrameRendererPS2::etFLPS2automode);
	}

	if (!CResources::verdana18s.loadFontBuffer(CResources::verdana18s_bfnk, CResources::size_verdana18s_bfnk, &ps2renderer))
	{
		printf("Couldn't load main small font. Exiting...\n");
		return -1;
	}
	if (!CResources::verdana22.loadFontBuffer(CResources::verdana22_bfnk, CResources::size_verdana22_bfnk, &ps2renderer))
	{
		printf("Couldn't load main font. Exiting...\n");
		return -1;
	}
	if (!CResources::centurygoth38p.loadFontBuffer(CResources::centurygoth38p_bfnk, CResources::size_centurygoth38p_bfnk, &ps2renderer))
	{
		printf("Couldn't load main font. Exiting...\n");
		return -1;
	}
	if (!CResources::urwmed20.loadFontBuffer(CResources::urwmed20_bfnk, CResources::size_urwmed20_bfnk, &ps2renderer))
	{
		printf("Couldn't load version font. Exiting...\n");
		return -1;
	}

	CGUIMcaMainWnd mainWindow;

	ps2renderer.setAlpha(false);
	ps2renderer.setTestAlpha(false);

	mainWindow.display(&ps2renderer, &ps2input, &ps2Timer);

	ps2renderer.deinitRenderer();
	ps2Timer.deinitTimer();
	return 0;
}

bool CPS2Application::loadLanguage(const std::string& langfile)
{
	int fd = fioOpen(langfile.c_str(), O_BINARY | O_RDONLY);
	if (fd > 0)
	{
		size_t filesize = fioLseek(fd, 0, SEEK_END);
		fioLseek(fd, 0, SEEK_SET);
		if (filesize > 0)
		{
			char *buff = new char[filesize+1];
			fioRead(fd, buff, filesize);
			buff[filesize] = 0;

			CResources::mainLang.initLang(buff);
			delete [] buff;
			return true;
		}
		fioClose(fd);
	}
	return false;
}

void CPS2Application::setBootPath(const char* path)
{
	CResources::boot_path = path;
	if (CResources::boot_path.empty())
	{
		CResources::boot_path = "host:";
	}
	u32 stpos = 0;
	if ( (stpos = CResources::boot_path.rfind('/')) == std::string::npos)
	{
		if ( (stpos = CResources::boot_path.rfind('\\')) == std::string::npos)
		{
			stpos = CResources::boot_path.rfind(':');
		}
	}

	if	(stpos != std::string::npos)
		CResources::boot_path = CResources::boot_path.substr(0, stpos+1);
		
}

CPS2Application *CPS2Application::m_pInstance = NULL;
