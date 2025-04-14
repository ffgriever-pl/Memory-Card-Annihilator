#include "GUIMcaMan.h"
#include "iop/mca_irx.h"
#include "GUIFramePS2Modules.h"
#include <string>


CGUIMcaMan::CGUIMcaMan(void)
{
}


CGUIMcaMan::~CGUIMcaMan(void)
{
}

CGUIMcaMan::memcardsOps CGUIMcaMan::mce_memcards[2] __attribute__((aligned (64))) = {
	{0,0,0,0,0},
	{0,0,0,0,0}
};

CGUIMcaMan::d_iopMcaCommand CGUIMcaMan::iopMcaCommand __attribute__((aligned (64)));
CGUIMcaMan::d_progressBarData CGUIMcaMan::progressBarData __attribute__((aligned (64)));

SifRpcClientData_t CGUIMcaMan::mca_cd0 __attribute__((aligned (64)));

bool CGUIMcaMan::init_done = false;
bool CGUIMcaMan::init_failed = false;

void CGUIMcaMan::initMca()
{
	if (init_done) return;

	CGUIFramePS2Modules::loadMcModules();
	int ret, id;
	id = SifExecModuleBuffer(mca_irx, size_mca_irx, 0, NULL, &ret);
	IRX_REPORT("mca", id, ret);

	while((ret = SifBindRpc(&mca_cd0, MCANNIHMAGIC, 0)))
		nopdelay();

	if((ret < 0) || (mca_cd0.server == 0))
	{
		printf("mcaEE: Problem binding RPC, ret = %d\n", ret);
		init_failed = true;
	}

	init_done = true;
}

void CGUIMcaMan::updateMca()
{
	if (!init_done || init_failed) return;
	SifCallRpc(&mca_cd0, MCA_GET_CARD_SPEC, MCA_WAIT, mce_memcards, sizeof(mce_memcards), mce_memcards, sizeof(mce_memcards), 0, 0);
}

void CGUIMcaMan::getProgress()
{
	if (!init_done || init_failed) return;
	SifCallRpc(&mca_cd0, MCA_GET_PROGRESS, MCA_WAIT, &progressBarData, sizeof(progressBarData), &progressBarData, sizeof(progressBarData), 0, 0);
	_InvalidDCache(&progressBarData, &progressBarData + sizeof(progressBarData));
}

void CGUIMcaMan::doFormat(int slot, bool psx, bool fast, int totalpages)
{
	progressBarData.error = 0;
	progressBarData.finished = 0;
	progressBarData.promil = 0;

	if (!init_done || init_failed)
	{
		progressBarData.error = 1;
		progressBarData.finished = 1;
		return;
	}
	memset(&iopMcaCommand, 0, sizeof(iopMcaCommand));
	iopMcaCommand.slot = slot;
	iopMcaCommand.type = (psx ? 1 : 2);
	iopMcaCommand.fast = (fast ? 1 : 0);
	if (totalpages == 1) totalpages = 0;
	iopMcaCommand.special = totalpages;
	iopMcaCommand.podslot = 0;
	SifWriteBackDCache(&iopMcaCommand, sizeof(iopMcaCommand));
	if (psx) SifCallRpc(&mca_cd0, MCA_FORMAT_PSX, MCA_WAIT, &iopMcaCommand, sizeof(iopMcaCommand), NULL, 0, 0, 0);
	else SifCallRpc(&mca_cd0, MCA_FORMAT_PS2, MCA_WAIT, &iopMcaCommand, sizeof(iopMcaCommand), NULL, 0, 0, 0);
}

void CGUIMcaMan::doUnformat(int slot, bool psx, int totalpages)
{
	progressBarData.error = 0;
	progressBarData.finished = 0;
	progressBarData.promil = 0;
	
	if (!init_done || init_failed)
	{
		progressBarData.error = 1;
		progressBarData.finished = 1;
		return;
	}
	memset(&iopMcaCommand, 0, sizeof(iopMcaCommand));
	iopMcaCommand.slot = slot;
	iopMcaCommand.type = (psx ? 1 : 2);
	iopMcaCommand.fast = 0;
	if (totalpages == 1) totalpages = 0;
	iopMcaCommand.special = totalpages;
	iopMcaCommand.podslot = 0;
	SifWriteBackDCache(&iopMcaCommand, sizeof(iopMcaCommand));
	if (psx) SifCallRpc(&mca_cd0, MCA_UNFORMAT_PSX, MCA_WAIT, &iopMcaCommand, sizeof(iopMcaCommand), NULL, 0, 0, 0);
	else SifCallRpc(&mca_cd0, MCA_UNFORMAT_PS2, MCA_WAIT, &iopMcaCommand, sizeof(iopMcaCommand), NULL, 0, 0, 0);
}

void CGUIMcaMan::doCreateImage(int slot, bool psx, int totalpages, const char* path)
{
	progressBarData.error = 0;
	progressBarData.finished = 0;
	progressBarData.promil = 0;
	
	if (!init_done || init_failed)
	{
		progressBarData.error = 1;
		progressBarData.finished = 1;
		return;
	}
	memset(&iopMcaCommand, 0, sizeof(iopMcaCommand));
	iopMcaCommand.slot = slot;
	iopMcaCommand.type = (psx ? 1 : 2);
	if (totalpages == 1) totalpages = 0;
	iopMcaCommand.special = totalpages;
	iopMcaCommand.podslot = 0;
	std::string filePath(path);
	if (filePath.substr(0, 5) == "hdd0:")
	{
		u32 pos = filePath.find('/');
		if (pos == std::string::npos)
		{
			progressBarData.error = 1;
			progressBarData.finished = 1;
			return;
		}
		fileXioUmount("pfs0:");
		sio_printf("Mounting '%s' as pfs0\n", filePath.substr(0, pos).c_str());
		fileXioMount("pfs0:", filePath.substr(0, pos).c_str(), FIO_MT_RDWR);
		std::string pfsPath = "pfs0:";
		pfsPath += filePath.substr(pos, filePath.length()-pos);
		filePath = pfsPath;
		sio_printf("Mounted path: '%s'\n", filePath.c_str());
	}
	strncpy(iopMcaCommand.filePath, filePath.c_str(), 255);
	if (!psx)
	{
		std::string mciPath;
		u32 posslash = filePath.rfind('/');
		u32 posdot = filePath.rfind('.');
		if (posdot == std::string::npos)
		{
			mciPath = filePath + ".mci";
		} else if (posslash == std::string::npos)
		{
			mciPath = filePath.substr(0, posdot) + ".mci";
		} else if (posdot < posslash)
		{
			mciPath = filePath + ".mci";
		} else
		{
			mciPath = filePath.substr(0, posdot) + ".mci";
		}
		strncpy(iopMcaCommand.filePathMci, mciPath.c_str(), 255);
	}
	SifWriteBackDCache(&iopMcaCommand, sizeof(iopMcaCommand));
	if (psx) SifCallRpc(&mca_cd0, MCA_WRITE_IMAGE_PSX, MCA_WAIT, &iopMcaCommand, sizeof(iopMcaCommand), NULL, 0, 0, 0);
	else SifCallRpc(&mca_cd0, MCA_WRITE_IMAGE_PS2, MCA_WAIT, &iopMcaCommand, sizeof(iopMcaCommand), NULL, 0, 0, 0);
}

void CGUIMcaMan::doRestoreImage(int slot, bool psx, const char* path)
{
	progressBarData.error = 0;
	progressBarData.finished = 0;
	progressBarData.promil = 0;
	
	if (!init_done || init_failed)
	{
		progressBarData.error = 1;
		progressBarData.finished = 1;
		return;
	}
	memset(&iopMcaCommand, 0, sizeof(iopMcaCommand));
	iopMcaCommand.slot = slot;
	iopMcaCommand.type = (psx ? 1 : 2);
	iopMcaCommand.podslot = 0;
	std::string filePath(path);
	if (filePath.substr(0, 5) == "hdd0:")
	{
		u32 pos = filePath.find('/');
		if (pos == std::string::npos)
		{
			progressBarData.error = 1;
			progressBarData.finished = 1;
			return;
		}
		fileXioUmount("pfs0:");
		fileXioMount("pfs0:", filePath.substr(0, pos).c_str(), FIO_MT_RDONLY);
		std::string pfsPath = "pfs0:";
		pfsPath += filePath.substr(pos, filePath.length()-pos);
		filePath = pfsPath;
	}
	strncpy(iopMcaCommand.filePath, filePath.c_str(), 255);

	SifWriteBackDCache(&iopMcaCommand, sizeof(iopMcaCommand));
	if (psx) SifCallRpc(&mca_cd0, MCA_READ_IMAGE_PSX, MCA_WAIT, &iopMcaCommand, sizeof(iopMcaCommand), NULL, 0, 0, 0);
	else SifCallRpc(&mca_cd0, MCA_READ_IMAGE_PS2, MCA_WAIT, &iopMcaCommand, sizeof(iopMcaCommand), NULL, 0, 0, 0);
}
