#ifndef _MCA_RPC_H
#define _MCA_RPC_H
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

#include <types.h>

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
d_iopMcaCommand iopMcaCommand __attribute__((aligned (64)));

#endif
