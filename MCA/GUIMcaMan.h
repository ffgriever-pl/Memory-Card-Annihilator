#ifndef _GUIMCAMAN_H_
#define _GUIMCAMAN_H_

#include "GUIFramePS2Includes.h"

#define MCA_BIND

#define MCANNIHMAGIC 0x8088554D

#define MCA_WAIT		0
#define MCA_NOWAIT		1

#define MCA_GET_CARD_SPEC 0x01
#define MCA_WRITE_IMAGE_PS2 0x02
#define MCA_WRITE_IMAGE_PSX 0x03
#define MCA_GET_PROGRESS 0x04
#define MCA_UNFORMAT_PS2 0x05
#define MCA_UNFORMAT_PSX 0x06
#define MCA_READ_IMAGE_PS2 0x07
#define MCA_READ_IMAGE_PSX 0x08
#define MCA_FORMAT_PS2 0x09
#define MCA_FORMAT_PSX 0x0A
#define MCA_PREPARE_EXIT 0xDD

class CGUIMcaMan //fully static! we need that... easier than singleton object
{
public:
	enum enCardType
	{
		enctNone = 0,
		enctPsx = 1,
		enctPs2 = 2,
		enctPda = 3
	};
	typedef
	struct t_memcardsOps
	{
		u32 type; //0 - none, 1 - psx, 2 - ps2, 3 - pda
		u16 pageSize;
		u16 pagesPerBlock;
		u32 totalPages;
		u8  flags;
	} memcardsOps;
	static memcardsOps mce_memcards[2] __attribute__((aligned (64)));

	typedef
	struct t_iopMcaCommand
	{
		u32 slot;
		u32 type;
		u32 special; //0 for default, non zero for other.
		u32 fast;
		char filePath[256];
		char filePathMci[256];
		u32 podslot;
		u32 force;
		u32 blockSize;
		u32 pageSize;
		u32 options;
	} d_iopMcaCommand;
	u32 iopMcaCommandReturn __attribute__((aligned (64)));
	static d_iopMcaCommand iopMcaCommand __attribute__((aligned (64)));

	static SifRpcClientData_t mca_cd0 __attribute__((aligned (64)));

	typedef
	struct t_progressBarData
	{
		s32 promil;
		s32 finished;
		s32 error;
		s32 padding;
	} d_progressBarData;
	static d_progressBarData progressBarData __attribute__((aligned (64)));


private:
	CGUIMcaMan(void);
	static unsigned int size_mca_irx;
	static unsigned char mca_irx[];
	static bool init_done;
	static bool init_failed;
public:
	~CGUIMcaMan(void);
	static void initMca();
	static void updateMca();
	static void getProgress();
	static void doFormat(int slot, bool psx, bool fast, int totalpages);
	static void doUnformat(int slot, bool psx, int totalpages);
	static void doCreateImage(int slot, bool psx, int totalpages, const char* path);
	static void doRestoreImage(int slot, bool psx, const char* path);
};

#endif //_GUIMCAMAN_H_
