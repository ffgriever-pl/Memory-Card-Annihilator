#ifndef _GUIFRAMEPS2MODULES_H_
#define _GUIFRAMEPS2MODULES_H_

#include "GUIFramePS2Includes.h"

// for IRX success load: (id > 0 && ret != 1)
#define IRX_REPORT(irx, id, ret) printf("%s: id:%d ret:%d\n", irx, id, ret)

class CGUIFramePS2Modules
{
private:
	CGUIFramePS2Modules(void); //we don't want any objects
	static bool m_modules_padman;
	static bool m_modules_sio2man;
	static bool m_modules_mcman;
	static bool m_modules_mcserv;
	static bool m_modules_poweroff;
	static bool m_modules_filexio;
	static bool m_modules_dev9;
	static bool m_modules_fs;
	static bool m_modules_hdd;
	static bool m_modules_atad;
	static bool m_modules_usbd;
	static bool m_modules_usbhdfsd;
	static bool m_modules_cdvd;
	static bool m_modules_cdvd_init;
	static bool m_modules_fakehost;
	static bool m_use_xmodules;

	static unsigned int size_usbd_irx;
	static unsigned char usbd_irx[];
	static unsigned int size_usbhdfsd_irx;
	static unsigned char usbhdfsd_irx[];
	static unsigned int size_iomanx_irx;
	static unsigned char iomanx_irx[];

	static unsigned int size_poweroff_irx;
	static unsigned char poweroff_irx[];
	static unsigned int size_ps2atad_irx;
	static unsigned char ps2atad_irx[];
	static unsigned int size_ps2dev9_irx;
	static unsigned char ps2dev9_irx[];
	static unsigned int size_ps2fs_irx;
	static unsigned char ps2fs_irx[];
	static unsigned int size_ps2hdd_irx;
	static unsigned char ps2hdd_irx[];

	static unsigned int size_filexio_irx;
	static unsigned char filexio_irx[];

	static unsigned int size_cdvd_irx;
	static unsigned char cdvd_irx[];

	static unsigned int size_fakehost_irx;
	static unsigned char fakehost_irx[];
public:
	~CGUIFramePS2Modules(void);
	static void iopReset(bool xmodules = false);
	static bool resetFlags();
	static bool loadSio2Man();
	static bool loadFileXio();
	static bool loadPadModules();
	static bool loadMcModules();
	static bool loadUsbModules();
	static bool loadHddModules();
	static bool loadCdvdModules();
	static bool loadFakehost(const char *path);
	static void initPS2Iop(bool reset, bool xmodules);
	static void poweroffHandler(int i);
	static void umountAll();
};

#endif //_GUIFRAMEPS2MODULES_H_
