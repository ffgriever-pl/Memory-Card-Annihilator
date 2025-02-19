
#define DEBUG


#include <types.h>
#include <stdio.h>
#include <sysclib.h>
#include <thbase.h>
#include <thsemap.h>
#include <intrman.h>
#include <sysmem.h>
#include <sifman.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include <loadcore.h>

#include "mca.h"
#include "formatter.h"

#define MODNAME "mcannihilator"
IRX_ID(MODNAME, 0x01, 0x00);


#define RWSIZE	(2048)
static u8 buffer[RWSIZE] __attribute__((aligned (128))); // RPC send/receive buffer
static struct t_SifRpcDataQueue qd __attribute__((aligned (128)));
static struct t_SifRpcServerData sd0 __attribute__((aligned (128)));

volatile int semaphoreCopy = 0;
volatile int semaphoreProgress = 0;
int getCardSpecThreadId = 0;

void* mca_rpc_server(int fno, void *data, int size);
void mca_Thread(void* param);
void getCardSpecs()
{
	int r, i;
	while (semaphoreCopy) {DelayThread(1023);}
	semaphoreCopy = 1;
	memset(mce_memcards, 0, sizeof(mce_memcards));
	if (pMcDetectCard == NULL || pMcGetCardType == NULL)
	{
		semaphoreCopy = 0;
		printf("mcaIOP: Function problem!\n");
		return;
	}

	for (i = 0; i < 2; i++)
	{
		LockMcman();
		/* detecting a memory card */
		r = pMcDetectCard(i, 0);

		/* handling non-fatal errors */
		if ((r >= sceMcResNoFormat) && (r != sceMcResSucceed))
		{
			/* trying to detect a memory card again */
			r = pMcDetectCard(i, 0);
		}
		FreeMcman();

		if (r != sceMcResSucceed) continue;

		/* checking the memory card type */
		r = pMcGetCardType(i, 0);
		mce_memcards[i].type = r;

		if (mce_memcards[i].type == 2)
		{
			LockMcman();
			r = pMcGetSpec(i, 0, &mce_memcards[i].pageSize, &mce_memcards[i].pagesPerBlock, &mce_memcards[i].totalPages, &mce_memcards[i].flags);
			FreeMcman();

			if (r != sceMcResSucceed)
			{
				memset(&mce_memcards[i], 0, sizeof(memcardsOps));
			}
		} else if (mce_memcards[i].type == 1 || mce_memcards[i].type == 3)
		{
			mce_memcards[i].pageSize = 128; //bytes
			mce_memcards[i].pagesPerBlock = 128; //pages
			mce_memcards[i].totalPages = 1024; //pages
			mce_memcards[i].flags = 0;
		} else
		{
			memset(&mce_memcards[i], 0, sizeof(memcardsOps));
		}

	}
	semaphoreCopy = 0;
}
int prepareExit = 0;
void getCardSpecThread()
{
	while (1)
	{
		if (prepareExit == 1) break;
		getCardSpecs();
		DelayThread(1000001);
	}
	ExitDeleteThread();
}
void writePS2imageThread(d_iopMcaCommand* commandData)
{
	printf("IOP: writePS2imageThread();\n");
LockMcman();
	writeMcPs2(commandData);
	//umount("pfs0:");
FreeMcman();
	threadCommand = 0;
	ExitDeleteThread();
}
void readPS2imageThread(d_iopMcaCommand* commandData)
{
	printf("IOP: readPS2imageThread();\n");
LockMcman();
	readMcPs2(commandData);
	//umount("pfs0:");
FreeMcman();
	threadCommand = 0;
	ExitDeleteThread();
}
void readPSXimageThread(d_iopMcaCommand* commandData)
{
	printf("IOP: readPSXimageThread();\n");
LockMcman();
	readMcPsx(commandData);
	//umount("pfs0:");
FreeMcman();
	threadCommand = 0;
	ExitDeleteThread();
}
void writePSXimageThread(d_iopMcaCommand* commandData)
{
	printf("IOP: writePSXimageThread();\n");
LockMcman();
	writeMcPsx(commandData);
	//umount("pfs0:");
FreeMcman();
	threadCommand = 0;
	ExitDeleteThread();
}
void unformatPS2Thread(d_iopMcaCommand* commandData)
{
	printf("IOP: unformatPS2Thread();\n");
LockMcman();
	unformatPS2(commandData);
FreeMcman();
	threadCommand = 0;
	ExitDeleteThread();
}
void unformatPSXThread(d_iopMcaCommand* commandData)
{
	printf("IOP: unformatPSXThread();\n");
LockMcman();
	unformatPSX(commandData);
FreeMcman();
	threadCommand = 0;
	ExitDeleteThread();
}
void formatPS2Thread(d_iopMcaCommand* commandData)
{
	printf("IOP: formatPS2Thread();\n");
LockMcman();
	formatPS2(commandData);
FreeMcman();
	threadCommand = 0;
	ExitDeleteThread();
}
void formatPSXThread(d_iopMcaCommand* commandData)
{
	printf("IOP: formatPSXThread();\n");
LockMcman();
	formatPSX(commandData);
FreeMcman();
	threadCommand = 0;
	ExitDeleteThread();
}
void* getCardSpec_RPC(unsigned int *data)
{
	while (semaphoreCopy) {DelayThread(1011);}
	semaphoreCopy = 1;
	memcpy(data, mce_memcards, sizeof(mce_memcards));
	semaphoreCopy = 0;
	return data;
}
void* getProgress_RPC(d_progressBarData *progressData)
{
	while (semaphoreProgress) {DelayThread(1011);}
	semaphoreProgress = 1;

	if (threadCommand <= 0 && progressBarData.finished == 0)
	{
		progressBarData.error = 1;
		progressBarData.promil = 0;
		progressBarData.finished = 1;
	}
	memcpy(progressData, &progressBarData, sizeof(progressBarData));

	semaphoreProgress = 0;
	return progressData;
}
void* writePS2image_RPC(unsigned int *data)
{
	struct _iop_thread param;
	printf("IOP: writePS2image_RPC();\n");
	while (threadCommand) {DelayThread(10011);}
	threadCommand = 1;
	memset(&progressBarData, 0, sizeof(progressBarData));
	memcpy(&iopMcaCommand, data, sizeof(iopMcaCommand));

	param.attr         = 0x02000000;
	param.thread       = (void*)writePS2imageThread;
	param.priority 	  = 40;
	param.stacksize    = 0x10000;
	param.option      = 0;

	threadCommand = CreateThread(&param);

	if (threadCommand > 0)
	{
		StartThread(threadCommand, &iopMcaCommand);
		iopMcaCommandReturn = 0;
		return &iopMcaCommandReturn;
	}
	else
	{
		iopMcaCommandReturn = 1;
		return &iopMcaCommandReturn;
	}
}
void* readPS2image_RPC(unsigned int *data)
{
	struct _iop_thread param;
	printf("IOP: readPS2image_RPC();\n");
	while (threadCommand) {DelayThread(10011);}
	threadCommand = 1;
	memset(&progressBarData, 0, sizeof(progressBarData));
	memcpy(&iopMcaCommand, data, sizeof(iopMcaCommand));

	param.attr         = 0x02000000;
	param.thread       = (void*)readPS2imageThread;
	param.priority 	  = 40;
	param.stacksize    = 0x10000;
	param.option      = 0;

	threadCommand = CreateThread(&param);

	if (threadCommand > 0)
	{
		StartThread(threadCommand, &iopMcaCommand);
		iopMcaCommandReturn = 0;
		return &iopMcaCommandReturn;
	}
	else
	{
		iopMcaCommandReturn = 1;
		return &iopMcaCommandReturn;
	}
}
void* readPSXimage_RPC(unsigned int *data)
{
	struct _iop_thread param;
	printf("IOP: readPSXimage_RPC();\n");
	while (threadCommand) {DelayThread(10011);}
	threadCommand = 1;
	memset(&progressBarData, 0, sizeof(progressBarData));
	memcpy(&iopMcaCommand, data, sizeof(iopMcaCommand));

	param.attr         = 0x02000000;
	param.thread       = (void*)readPSXimageThread;
	param.priority 	  = 40;
	param.stacksize    = 0x10000;
	param.option      = 0;

	threadCommand = CreateThread(&param);

	if (threadCommand > 0)
	{
		StartThread(threadCommand, &iopMcaCommand);
		iopMcaCommandReturn = 0;
		return &iopMcaCommandReturn;
	}
	else
	{
		iopMcaCommandReturn = 1;
		return &iopMcaCommandReturn;
	}
}
void* writePSXimage_RPC(unsigned int *data)
{
	struct _iop_thread param;
	printf("IOP: writePSXimage_RPC();\n");
	while (threadCommand) {DelayThread(10011);}
	threadCommand = 1;
	memset(&progressBarData, 0, sizeof(progressBarData));
	memcpy(&iopMcaCommand, data, sizeof(iopMcaCommand));

	param.attr         = 0x02000000;
	param.thread       = (void*)writePSXimageThread;
	param.priority 	  = 40;
	param.stacksize    = 0x10000;
	param.option      = 0;

	threadCommand = CreateThread(&param);

	if (threadCommand > 0)
	{
		StartThread(threadCommand, &iopMcaCommand);
		iopMcaCommandReturn = 0;
		return &iopMcaCommandReturn;
	}
	else
	{
		iopMcaCommandReturn = 1;
		return &iopMcaCommandReturn;
	}
}
void* unformatPS2_RPC(unsigned int *data)
{
	struct _iop_thread param;
	printf("IOP: unformatPS2_RPC();\n");
	while (threadCommand) {DelayThread(10011);}
	threadCommand = 1;
	memset(&progressBarData, 0, sizeof(progressBarData));
	memcpy(&iopMcaCommand, data, sizeof(iopMcaCommand));

	param.attr         = 0x02000000;
	param.thread       = (void*)unformatPS2Thread;
	param.priority 	  = 40;
	param.stacksize    = 0x10000;
	param.option      = 0;

	threadCommand = CreateThread(&param);

	if (threadCommand > 0)
	{
		StartThread(threadCommand, &iopMcaCommand);
		iopMcaCommandReturn = 0;
		return &iopMcaCommandReturn;
	}
	else
	{
		iopMcaCommandReturn = 1;
		return &iopMcaCommandReturn;
	}
}
void* unformatPSX_RPC(unsigned int *data)
{
	struct _iop_thread param;
	printf("IOP: unformatPSX_RPC();\n");
	while (threadCommand) {DelayThread(10011);}
	threadCommand = 1;
	memset(&progressBarData, 0, sizeof(progressBarData));
	memcpy(&iopMcaCommand, data, sizeof(iopMcaCommand));

	param.attr         = 0x02000000;
	param.thread       = (void*)unformatPSXThread;
	param.priority 	  = 40;
	param.stacksize    = 0x10000;
	param.option      = 0;

	threadCommand = CreateThread(&param);

	if (threadCommand > 0)
	{
		StartThread(threadCommand, &iopMcaCommand);
		iopMcaCommandReturn = 0;
		return &iopMcaCommandReturn;
	}
	else
	{
		iopMcaCommandReturn = 1;
		return &iopMcaCommandReturn;
	}
}
void* formatPS2_RPC(unsigned int *data)
{
	struct _iop_thread param;
	printf("IOP: formatPS2_RPC();\n");
	while (threadCommand) {DelayThread(10011);}
	threadCommand = 1;
	memset(&progressBarData, 0, sizeof(progressBarData));
	memcpy(&iopMcaCommand, data, sizeof(iopMcaCommand));

	param.attr         = 0x02000000;
	param.thread       = (void*)formatPS2Thread;
	param.priority 	  = 40;
	param.stacksize    = 0x10000;
	param.option      = 0;

	threadCommand = CreateThread(&param);

	if (threadCommand > 0)
	{
		StartThread(threadCommand, &iopMcaCommand);
		iopMcaCommandReturn = 0;
		return &iopMcaCommandReturn;
	}
	else
	{
		iopMcaCommandReturn = 1;
		return &iopMcaCommandReturn;
	}
}
void* formatPSX_RPC(unsigned int *data)
{
	struct _iop_thread param;
	printf("IOP: formatPSX_RPC();\n");
	while (threadCommand) {DelayThread(10011);}
	threadCommand = 1;
	memset(&progressBarData, 0, sizeof(progressBarData));
	memcpy(&iopMcaCommand, data, sizeof(iopMcaCommand));

	param.attr         = 0x02000000;
	param.thread       = (void*)formatPSXThread;
	param.priority 	  = 40;
	param.stacksize    = 0x10000;
	param.option      = 0;

	threadCommand = CreateThread(&param);

	if (threadCommand > 0)
	{
		StartThread(threadCommand, &iopMcaCommand);
		iopMcaCommandReturn = 0;
		return &iopMcaCommandReturn;
	}
	else
	{
		iopMcaCommandReturn = 1;
		return &iopMcaCommandReturn;
	}
}
int _start( int argc, char **argv)
{
	struct _iop_thread param;
	int th;
	threadCommand = 0;

	param.attr         = 0x02000000;
	param.thread       = (void*)mca_Thread;
	param.priority 	  = 39;
	param.stacksize    = 0x10000;
	param.option      = 0;

	th = CreateThread(&param);

	if (th > 0)
	{
		StartThread(th,0);
		return 0;
	}
	else
		return 1;

	return 0;
}

void mca_Thread(void* param)
{
	struct _iop_thread thparam;

	SifInitRpc(0);

	initLibEntries();

	thparam.attr         = 0x02000000;
	thparam.thread       = (void*)getCardSpecThread;
	thparam.priority 	  = 40;
	thparam.stacksize    = 0x1000;
	thparam.option      = 0;

	getCardSpecThreadId = CreateThread(&thparam);

	if (getCardSpecThreadId > 0)
	{
		StartThread(getCardSpecThreadId,0);
	}
	memset(mce_memcards, 0, sizeof(mce_memcards));

	SifSetRpcQueue(&qd, GetThreadId());
	SifRegisterRpc(&sd0, MCANNIHMAGIC,mca_rpc_server,(void *)buffer,0,0,&qd);

	SifRpcLoop(&qd);
}

void* mca_rpc_server(int fno, void *data, int size)
{
	switch(fno) {
		case MCA_GET_CARD_SPEC:
			return getCardSpec_RPC(data);
		case MCA_WRITE_IMAGE_PS2:
			writePS2image_RPC(data);
			return NULL;
		case MCA_WRITE_IMAGE_PSX:
			writePSXimage_RPC(data);
			return NULL;
		case MCA_GET_PROGRESS:
			return getProgress_RPC((d_progressBarData *)data);
		case MCA_UNFORMAT_PS2:
			unformatPS2_RPC(data);
			return NULL;
		case MCA_UNFORMAT_PSX:
			unformatPSX_RPC(data);
			return NULL;
		case MCA_READ_IMAGE_PS2:
			readPS2image_RPC(data);
			return NULL;
		case MCA_READ_IMAGE_PSX:
			readPSXimage_RPC(data);
			return NULL;
		case MCA_FORMAT_PS2:
			formatPS2_RPC(data);
			return NULL;
		case MCA_FORMAT_PSX:
			formatPSX_RPC(data);
			return NULL;
		case MCA_PREPARE_EXIT:
			//
			prepareExit = 1;
			DelayThread(3000000);
			return NULL;
	}
	return data;
}
