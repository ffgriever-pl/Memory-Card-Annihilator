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
#include "../helpers.h"

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
	bool languageLoaded = initLanguage(CResources::boot_path); //allow to load before iopreset (potentially unsupported devices with already loaded modules)

	ResetEE(0xffffffff);
	CGUIFramePS2Modules::initPS2Iop(CResources::iopreset);
	CGUIFramePS2Modules::loadMcModules();
	CGUIFramePS2Modules::loadUsbModules();
	CGUIFramePS2Modules::loadCdvdModules();
	CGUIMcaMan::initMca();

	if (!languageLoaded) //let it try after iopreset and modules load
		initLanguage(processBootPath(CResources::boot_path));

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

	if (!CResources::smallFont.loadFontBuffer(CResources::meiryo18_bfnk, CResources::size_meiryo18_bfnk, &ps2renderer))
	{
		printf("Couldn't load small font. Exiting...\n");
		return -1;
	}
	if (!CResources::mediumFont.loadFontBuffer(CResources::meiryo22_bfnk, CResources::size_meiryo22_bfnk, &ps2renderer))
	{
		printf("Couldn't load medium font. Exiting...\n");
		return -1;
	}
	if (!CResources::headerFont.loadFontBuffer(CResources::mplus1p42_bfnk, CResources::size_mplus1p42_bfnk, &ps2renderer))
	{
		printf("Couldn't load header font. Exiting...\n");
		return -1;
	}
	if (!CResources::versionFont.loadFontBuffer(CResources::urwmed20_bfnk, CResources::size_urwmed20_bfnk, &ps2renderer))
	{
		printf("Couldn't load version font. Exiting...\n");
		return -1;
	}

	CGUIMcaMainWnd mainWindow(&ps2renderer, &ps2input, &ps2Timer);

	ps2renderer.setAlpha(false);
	ps2renderer.setTestAlpha(false);

	mainWindow.display();

	ps2renderer.deinitRenderer();
	ps2Timer.deinitTimer();
	return 0;
}

bool CPS2Application::initLanguage(const std::string& bootPath)
{
	static const char* languageFiles[] = {
		"lang_jp.lng",	// Japanese does not have a valid font yet
		"lang_en.lng",
		"lang_fr.lng",
		"lang_es.lng",
		"lang_de.lng",
		"lang_it.lng",
		"lang_du.lng",
		"lang_pt.lng",
		"lang_ru.lng", // Russian and further languages require XEB+ 2024 or newer in order to be detected. Else, they will default to English.
		// Korean, Traditional and Simplified Chinese do not have a valid font yet
	};
	
	std::string defaultLangFile = bootPath + "lang.lng";
	if (loadLanguage(defaultLangFile))
		return true;

	int systemLanguage = configGetLanguage();
	if (systemLanguage < 0 || systemLanguage >= static_cast<int>(countof(languageFiles)))
		return false;

	std::string systemLanguageFile = bootPath + languageFiles[systemLanguage];
	return loadLanguage(systemLanguageFile);
}

std::string CPS2Application::processHddBootPath(const std::string& bootPath) 
{
	const std::string hddPrefix = "hdd0:";
	if (bootPath.substr(0, hddPrefix.length()) != hddPrefix)
		return bootPath;
		
	const size_t pfsPos = bootPath.find(":pfs", hddPrefix.length());
	if (pfsPos == std::string::npos)
		return bootPath;
		
	const std::string partition = bootPath.substr(0, pfsPos);
	const std::string pfs = bootPath.substr(pfsPos + 1);

	const size_t pathPos = pfs.find(":");
	if (pathPos == std::string::npos)
		return bootPath;
		
	const std::string path = pfs.substr(pathPos + 1);

	CGUIFramePS2Modules::loadHddModules();
	if (hddCheckFormatted() != 0)
		return bootPath;
		
	fileXioUmount("pfs0:");
	if (fileXioMount("pfs0:", partition.c_str(), FIO_MT_RDWR) != 0)
		return bootPath;
		
	return "pfs0:" + path;
}

std::string CPS2Application::processMassBootPath(const std::string& bootPath)
{
	if (bootPath.length() < 5) // "mass:"
		return bootPath;

	if (bootPath.substr(0, 4) != "mass")
		return bootPath;

	if (bootPath[4] != ':' && (bootPath[4] < '0' || bootPath[4] > '9' || bootPath[5] != ':'))
		return bootPath;

	CGUIFramePS2Modules::loadUsbModules();

	if (waitForDisk(bootPath, 50))
		return bootPath;

	// It seems that the mass device has changed number, let's try to find it
	const char massNumber = bootPath[4] == ':' ? 0 : bootPath[4] - '0';
	std::string path = "mass0:"+ bootPath.substr(bootPath.find(":") + 1); //at this point, we know it exists

	for (int i = 0; i < 10; i++)
	{
		if (i == massNumber)
			continue;

		path[4] = '0' + i;
		if (waitForDisk(path, 1)) //at this point it should be already detected
			return path;
	}
	return bootPath;
}

std::string CPS2Application::processBootPath(const std::string& bootPath)
{
	if (bootPath.substr(0, 4) == "hdd0")
	{
		return processHddBootPath(bootPath);
	}
	else if (bootPath.substr(0, 4) == "mass")
	{
		return processMassBootPath(bootPath);
	}
	return bootPath;
}

bool CPS2Application::loadLanguage(const std::string& langfile)
{
	int fd = fioOpen(langfile.c_str(), O_BINARY | O_RDONLY);
	if (fd <= 0)
		return false;

	size_t filesize = fioLseek(fd, 0, SEEK_END);
	fioLseek(fd, 0, SEEK_SET);
	if (filesize <= 0)
	{
		fioClose(fd);
		return false;
	}

	char *buff = new char[filesize + 1];
	fioRead(fd, buff, filesize);
	buff[filesize] = 0;

	CResources::mainLang.initLang(buff);
	delete[] buff;
	fioClose(fd);
	return true;
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

bool CPS2Application::waitForDisk(const std::string path, int delay)
{
	iox_stat_t chk_stat;
	int ret;
	while ((ret = fileXioGetStat(path.c_str(), &chk_stat)) < 0 && --delay > 0) { nopdelay(); }
	return ret >= 0;
}

CPS2Application *CPS2Application::m_pInstance = NULL;
