#ifndef _MCA_H
#define _MCA_H

#define printf(...)

#include "psxcdr_rpc.h"
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <iomanx.h>

typedef
struct t_memcardsOps
{
	u32 type; //0 - none, 1 - psx, 2 - ps2, 3 - pda
	u16 pageSize;
	u16 pagesPerBlock;
	u32 totalPages;
	u8  flags;
} memcardsOps;
memcardsOps mce_memcards[2];

typedef
struct t_progressBarData
{
	s32 promil;
	s32 finished;
	s32 error;
} d_progressBarData;
d_progressBarData progressBarData;

typedef
struct t_mciFile
{
	char magic[4];
	u16 pagesSize;
	u16 pagesPerBlock;
	u32 totalPages;
	u32 flags;
} d_mciFile;
d_mciFile mciFile;

volatile int threadCommand;
volatile int semaphoreCopy;
volatile int semaphoreProgress;
int getCardSpecThreadId;

void getCardSpecs();

#endif // _CDRIOP_H
