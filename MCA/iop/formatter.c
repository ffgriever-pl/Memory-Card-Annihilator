#include "formatter.h"
#include "mca.h"

int (*pMcGetIoSema)() = NULL;

void *SysAlloc(u_long size)
{
 int oldstate;
 register void *p;
 
 CpuSuspendIntr(&oldstate);
 p = AllocSysMemory(SMEM_Low, size, NULL);
 CpuResumeIntr(oldstate);

 return p;
}

int SysFree(void *area)
{
 int oldstate;
 register int r;

 CpuSuspendIntr(&oldstate);
 r = FreeSysMemory(area);
 CpuResumeIntr(oldstate);

 return r;
}

/* locking MCMAN's internal mutex when possible */
void LockMcman()
{
 if (pMcGetIoSema != NULL) WaitSema(pMcGetIoSema());
}

/* freeing MCMAN's internal mutex when possible */
void FreeMcman()
{
 if (pMcGetIoSema != NULL) SignalSema(pMcGetIoSema());
}
void *GetLibraryEntry(char *libname, int version, int entryno)
{
 LibInfo lib;
 register void **exp;
 register int i;

 if (libname == NULL) return NULL;

 memset(&lib, 0, sizeof(LibInfo));
 lib.version = version & 0xFFFF;
 
 for (i = 0; (i < 8) && (libname[i]); i ++) lib.name[i] = libname[i];

 exp = (void **)QueryLibraryEntryTable(&lib);
 if (exp == NULL) return NULL;

 for (i = 0; exp[i]; i ++) {;}

 return (entryno < i) ? exp[entryno] : NULL;
}
void initLibEntries()
{
	int version;

	/* trying to use a new MCMAN module */
	version = 0x200;

	/* getting MCMAN's exports #21, #43, #53 */
	VOIDPTR(pMcDetectCard) = GetLibraryEntry("mcman", version, 21);
	VOIDPTR(pMcGetSpec) = GetLibraryEntry("mcman", version, 43);
	VOIDPTR(pMcGetIoSema) = GetLibraryEntry("mcman", version, 53);

	if ((pMcDetectCard == NULL) || (pMcGetSpec == NULL))
	{
		/* trying to use an old MCMAN module (BIOS) */
		version = 0x100;

		/* ___McGetSpec is not available within the old MCMAN modules */
		//pMcGetSpec = McGetDefSpec2;
		/* getting address of the alternate card detection routine */
		VOIDPTR(pMcDetectCard) = GetLibraryEntry("mcman", version, 5);   
	}
	/* getting MCMAN's ___McGetCardType */
	VOIDPTR(pMcGetCardType) = GetLibraryEntry("mcman", version, 39);

	/* getting MCMAN's routine to read card page */
	VOIDPTR(pMcReadPage) = GetLibraryEntry("mcman", version, 18);
	VOIDPTR(pMcReadPagePsx) = GetLibraryEntry("mcman", version, 29);
	VOIDPTR(pMcWritePage) = GetLibraryEntry("mcman", version, 19);
	VOIDPTR(pMcWritePagePsx) = GetLibraryEntry("mcman", version, 30);
	VOIDPTR(pMcEraseBlock) = GetLibraryEntry("mcman", version, 17);
}
u_char cmcbuffer[32];

const u_char xortable[256]= { /* Table for the ECC calculation */
     0x00, 0x87, 0x96, 0x11, 0xA5, 0x22, 0x33, 0xB4,
     0xB4, 0x33, 0x22, 0xA5, 0x11, 0x96, 0x87, 0x00,
     0xC3, 0x44, 0x55, 0xD2, 0x66, 0xE1, 0xF0, 0x77,
     0x77, 0xF0, 0xE1, 0x66, 0xD2, 0x55, 0x44, 0xC3,
     0xD2, 0x55, 0x44, 0xC3, 0x77, 0xF0, 0xE1, 0x66,
     0x66, 0xE1, 0xF0, 0x77, 0xC3, 0x44, 0x55, 0xD2,
     0x11, 0x96, 0x87, 0x00, 0xB4, 0x33, 0x22, 0xA5,
     0xA5, 0x22, 0x33, 0xB4, 0x00, 0x87, 0x96, 0x11,
     0xE1, 0x66, 0x77, 0xF0, 0x44, 0xC3, 0xD2, 0x55,
     0x55, 0xD2, 0xC3, 0x44, 0xF0, 0x77, 0x66, 0xE1,
     0x22, 0xA5, 0xB4, 0x33, 0x87, 0x00, 0x11, 0x96,
     0x96, 0x11, 0x00, 0x87, 0x33, 0xB4, 0xA5, 0x22,
     0x33, 0xB4, 0xA5, 0x22, 0x96, 0x11, 0x00, 0x87,
     0x87, 0x00, 0x11, 0x96, 0x22, 0xA5, 0xB4, 0x33,
     0xF0, 0x77, 0x66, 0xE1, 0x55, 0xD2, 0xC3, 0x44,
     0x44, 0xC3, 0xD2, 0x55, 0xE1, 0x66, 0x77, 0xF0,
     0xF0, 0x77, 0x66, 0xE1, 0x55, 0xD2, 0xC3, 0x44,
     0x44, 0xC3, 0xD2, 0x55, 0xE1, 0x66, 0x77, 0xF0,
     0x33, 0xB4, 0xA5, 0x22, 0x96, 0x11, 0x00, 0x87,
     0x87, 0x00, 0x11, 0x96, 0x22, 0xA5, 0xB4, 0x33,
     0x22, 0xA5, 0xB4, 0x33, 0x87, 0x00, 0x11, 0x96,
     0x96, 0x11, 0x00, 0x87, 0x33, 0xB4, 0xA5, 0x22,
     0xE1, 0x66, 0x77, 0xF0, 0x44, 0xC3, 0xD2, 0x55,
     0x55, 0xD2, 0xC3, 0x44, 0xF0, 0x77, 0x66, 0xE1,
     0x11, 0x96, 0x87, 0x00, 0xB4, 0x33, 0x22, 0xA5,
     0xA5, 0x22, 0x33, 0xB4, 0x00, 0x87, 0x96, 0x11,
     0xD2, 0x55, 0x44, 0xC3, 0x77, 0xF0, 0xE1, 0x66,
     0x66, 0xE1, 0xF0, 0x77, 0xC3, 0x44, 0x55, 0xD2,
     0xC3, 0x44, 0x55, 0xD2, 0x66, 0xE1, 0xF0, 0x77,
     0x77, 0xF0, 0xE1, 0x66, 0xD2, 0x55, 0x44, 0xC3,
     0x00, 0x87, 0x96, 0x11, 0xA5, 0x22, 0x33, 0xB4,
     0xB4, 0x33, 0x22, 0xA5, 0x11, 0x96, 0x87, 0x00,
     };

#define HDDCTL_STATUS			0x00004807
int checkHddReady()
{
	int rv;
	rv = devctl("hdd0:", HDDCTL_STATUS, NULL, 0, NULL, 0);

	if((rv >= 1) || (rv < 0))
		return -1;
	else
		return 0;	
}
void fmtWritePageS(int port, int slot, int page, void *buf, u32 pages_per_block, u32 pageSize)
{
	int retval;
	u_char *intBuf = (u_char*)buf;
	
	CalculateECC(&intBuf[  0], &cmcbuffer[0]);
	CalculateECC(&intBuf[128], &cmcbuffer[3]);
	CalculateECC(&intBuf[256], &cmcbuffer[6]);
	CalculateECC(&intBuf[384], &cmcbuffer[9]);
	cmcbuffer[12] = cmcbuffer[13] = cmcbuffer[14] = cmcbuffer[15] = 0;
	if (pageSize > 512)
	{
		CalculateECC(&intBuf[  0+512], &cmcbuffer[0+16]);
		CalculateECC(&intBuf[128+512], &cmcbuffer[3+16]);
		CalculateECC(&intBuf[256+512], &cmcbuffer[6+16]);
		CalculateECC(&intBuf[384+512], &cmcbuffer[9+16]);
		cmcbuffer[12+16] = cmcbuffer[13+16] = cmcbuffer[14+16] = cmcbuffer[15+16] = 0;
	}
	retval = pMcWritePage(port, slot, page, buf, cmcbuffer);
	if (retval < 0) printf("WritePage %d failed with %d\n", page, retval);
	//realWrite;
}
void fmtWritePage(int port, int slot, int page, void *buf, u32 pages_per_block, u32 pageSize)
{
	int retval;
	u_char *intBuf = (u_char*)buf;
	if ((page%pages_per_block) == 0) fmtEraseBlock(port, slot, page, pages_per_block);
	//write(fd, buf, page_size);
	
	CalculateECC(&intBuf[  0], &cmcbuffer[0]);
	CalculateECC(&intBuf[128], &cmcbuffer[3]);
	CalculateECC(&intBuf[256], &cmcbuffer[6]);
	CalculateECC(&intBuf[384], &cmcbuffer[9]);
	cmcbuffer[12] = cmcbuffer[13] = cmcbuffer[14] = cmcbuffer[15] = 0;
	if (pageSize > 512)
	{
		CalculateECC(&intBuf[  0+512], &cmcbuffer[0+16]);
		CalculateECC(&intBuf[128+512], &cmcbuffer[3+16]);
		CalculateECC(&intBuf[256+512], &cmcbuffer[6+16]);
		CalculateECC(&intBuf[384+512], &cmcbuffer[9+16]);
		cmcbuffer[12+16] = cmcbuffer[13+16] = cmcbuffer[14+16] = cmcbuffer[15+16] = 0;
	}
	retval = pMcWritePage(port, slot, page, buf, cmcbuffer);
	if (retval < 0) printf("WritePage %d failed with %d\n", page, retval);
	//realWrite;
}
void CalculateECC(u_char *buf, u_char *chk)
{
 register u_char *ptr;
 register int i, c1, c2, b, c3;

 ptr = buf;
 i = 0;
 c1 = c2 = c3 = 0;

 /* calculating ECC for a 0x80-bytes buffer */
 do
 {
  b = xortable[*ptr ++];
  c1 ^= b;  
  if (b & 0x80)
  {
   c2 ^= ~i;
   c3 ^= i;
  }
 } while (++ i < 0x80);

 /* storing ECC */
 ptr = (u_char*)chk;
 ptr[0] = ~c1 & 0x77;
 ptr[1] = ~c2 & 0x7F;
 ptr[2] = ~c3 & 0x7F;
}

void writeMcPs2(d_iopMcaCommand *iopCommand)
{
	u8* pageData = NULL;
	u32 i, k, retval, cachedPages, pageInCache;
	getCardSpecs();
	u32 type; //0 - none, 1 - psx, 2 - ps2, 3 - pda
	u16 pageSize;
	u16 pagesPerBlock;
	u32 totalPages;
	u8  flags;
	u32 slot = iopCommand->slot;
	u32 podslot = iopCommand->podslot;
	u8 erasebyte;
	int fd, fdmci;

	while (semaphoreCopy) {DelayThread(10011);}

	semaphoreCopy = 1;
		type = mce_memcards[slot].type;
		pageSize = (iopCommand->force == 1 ? iopCommand->pageSize : mce_memcards[slot].pageSize);
		pagesPerBlock = (iopCommand->force == 1 ? iopCommand->blockSize : mce_memcards[slot].pagesPerBlock);
		totalPages = mce_memcards[slot].totalPages;
		flags = (iopCommand->force == 1 ? iopCommand->options : mce_memcards[slot].flags);
	//semaphoreCopy = 0;
	fd = open(iopCommand->filePath, O_WRONLY | O_CREAT | O_TRUNC, 0);
	fdmci = open(iopCommand->filePathMci, O_WRONLY | O_CREAT | O_TRUNC, 0);

// user defined size
	erasebyte = (flags & 0x10) ? 0x0 : 0xFF;
	if (iopCommand->special) totalPages = iopCommand->special;

	if (((type == 0 || type != iopCommand->type) && iopCommand->force == 0) || pMcReadPage == NULL || fd <= 0 || fdmci <= 0)
	{
		if (fd) close(fd);
		if (fdmci) close(fdmci);

		while (semaphoreProgress) {DelayThread(1000);}

		semaphoreProgress = 1;
			progressBarData.error = 1;
			progressBarData.finished = 1;
		semaphoreProgress = 0;

	} else
	{
		mciFile.magic[0] = 'M';
		mciFile.magic[1] = 'C';
		mciFile.magic[2] = 'I';
		mciFile.magic[3] = '2';
		mciFile.flags = flags;
		mciFile.pagesPerBlock = pagesPerBlock;
		mciFile.pagesSize = pageSize;
		mciFile.totalPages = totalPages;
		write(fdmci, &mciFile, sizeof(mciFile));
		close(fdmci);

		pageData = SysAlloc(32768);
		cachedPages = 32768/pageSize;
		pageInCache = 0;
		for (i = 0; i < totalPages; i++)
		{
			k = 0;
			while ((retval = pMcReadPage(slot, podslot, i, &pageData[pageInCache*pageSize]) != sceMcResSucceed) && (k++ < 4)){;}
			if (retval != sceMcResSucceed)
			{
				memset(&pageData[pageInCache*pageSize], erasebyte, pageSize);
				printf("IOP: problem reading page %d\n", (int)i);
			}
			
			pageInCache++;
			if (pageInCache == cachedPages)
			{
				retval = write(fd, pageData, pageSize*pageInCache);
				if (retval < pageSize)
				{
					close(fd);
					SysFree(pageData);
					while (semaphoreProgress) {DelayThread(1000);}
					semaphoreProgress = 1;
						progressBarData.promil = (i*1000)/totalPages;
						progressBarData.error = 1;
						progressBarData.finished = 1;
					semaphoreProgress = 0;

					semaphoreCopy = 0;
					return;
				}
				pageInCache = 0;
			}
			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (i*1000)/totalPages;
			semaphoreProgress = 0;
		}
		if (pageInCache)
		{
			retval = write(fd, pageData, pageSize*pageInCache);
			if (retval < pageSize)
			{
				close(fd);
				SysFree(pageData);
				while (semaphoreProgress) {DelayThread(1000);}
				semaphoreProgress = 1;
					progressBarData.promil = 1000;
					progressBarData.error = 1;
					progressBarData.finished = 1;
				semaphoreProgress = 0;
				semaphoreCopy = 0;
				return;
			}
		}
		close(fd);
		SysFree(pageData);
		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = 1000;
			progressBarData.error = 0;
			progressBarData.finished = 1;
		semaphoreProgress = 0;
	}
	semaphoreCopy = 0;
}

void readMcPs2(d_iopMcaCommand *iopCommand)
{
	u8* pageData = NULL;
	u32 i, retval;
	getCardSpecs();
	u32 type; //0 - none, 1 - psx, 2 - ps2, 3 - pda
	u16 pageSize;
	u16 pagesPerBlock;
	u32 totalPages;
	u8  flags;
	u32 slot = iopCommand->slot;
	u32 podslot = iopCommand->podslot;
	u8 erasebyte;
	int fd;

	while (semaphoreCopy) {DelayThread(10011);}

	semaphoreCopy = 1;
		type = mce_memcards[slot].type;
		pageSize = (iopCommand->force == 1 ? iopCommand->pageSize : mce_memcards[slot].pageSize);
		pagesPerBlock = (iopCommand->force == 1 ? iopCommand->blockSize : mce_memcards[slot].pagesPerBlock);
		totalPages = mce_memcards[slot].totalPages;
		flags = (iopCommand->force == 1 ? iopCommand->options : mce_memcards[slot].flags);
	//semaphoreCopy = 0;

	fd = open(iopCommand->filePath, O_RDONLY, 0);

// user defined size
	erasebyte = (flags & 0x10) ? 0x0 : 0xFF;
	if (iopCommand->special) totalPages = iopCommand->special;

	if (((type == 0 || type != iopCommand->type) && iopCommand->force == 0) || pMcWritePage == NULL || pMcEraseBlock == NULL || fd <= 0)
	{
		while (semaphoreProgress) {DelayThread(1000);}

		semaphoreProgress = 1;
			progressBarData.error = 1;
			progressBarData.finished = 1;
		semaphoreProgress = 0;

	} else
	{
		for (i = 0; i < totalPages/pagesPerBlock; i++)
		{
			fmtEraseBlock(slot, podslot, i*pagesPerBlock, pagesPerBlock);

			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (i*1000)/(totalPages/pagesPerBlock);
			semaphoreProgress = 0;
		}
		pageData = SysAlloc(pageSize);
		for (i = 0; i < totalPages; i++)
		{
			memset(pageData, erasebyte, pageSize);
			retval = read(fd, pageData, pageSize);
			if (!retval)
			{printf("Can't read, retval = %d\n", retval);
				close(fd);
				SysFree(pageData);
				while (semaphoreProgress) {DelayThread(1000);}
				semaphoreProgress = 1;
					progressBarData.promil = (i*1000)/totalPages;
					progressBarData.error = 1;
					progressBarData.finished = 1;
				semaphoreProgress = 0;

				semaphoreCopy = 0;
				return;
			}
			fmtWritePage(slot, podslot, i, pageData, pagesPerBlock, pageSize);
			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (i*1000)/totalPages;
			semaphoreProgress = 0;
		}
		close(fd);
		SysFree(pageData);
		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = 1000;
			progressBarData.error = 0;
			progressBarData.finished = 1;
		semaphoreProgress = 0;
	}
	semaphoreCopy = 0;
}

void readMcPsx(d_iopMcaCommand *iopCommand)
{
	u8* pageData = NULL;
	u32 i, retval;
	getCardSpecs();
	u32 type; //0 - none, 1 - psx, 2 - ps2, 3 - pda
	u16 pageSize;
	u16 pagesPerBlock;
	u32 totalPages;
	u8  flags;
	u32 slot = iopCommand->slot;
	u32 podslot = iopCommand->podslot;
	u8 erasebyte;
	int fd;

	while (semaphoreCopy) {DelayThread(10011);}

	semaphoreCopy = 1;
		type = mce_memcards[slot].type;
		pageSize = (iopCommand->force == 1 ? 128 : mce_memcards[slot].pageSize);
		pagesPerBlock = (iopCommand->force == 1 ? 128 : mce_memcards[slot].pagesPerBlock);
		totalPages = (iopCommand->force == 1 ? 1024 : mce_memcards[slot].totalPages);
		flags = (iopCommand->force == 1 ? 0 : mce_memcards[slot].flags);
	//semaphoreCopy = 0;

	fd = open(iopCommand->filePath, O_RDONLY, 0);

// user defined size
	erasebyte = 0;

	if (((type == 0 || type != iopCommand->type) && iopCommand->force == 0) || pMcWritePagePsx == NULL || fd <= 0)
	{
		while (semaphoreProgress) {DelayThread(1000);}

		semaphoreProgress = 1;
			progressBarData.error = 1;
			progressBarData.finished = 1;
		semaphoreProgress = 0;

	} else
	{
		pageData = SysAlloc(pageSize);
	//check for type
		memset(pageData, erasebyte, pageSize);
		retval = read(fd, pageData, pageSize);
		if (strncmp(pageData, "MC", 2) == 0)
		{
			printf("IOP: RAW formatted image detected\n");
			lseek(fd, 0, SEEK_SET);
		} else if (strncmp(pageData, "123-456-STD", 11) == 0)
		{
			printf("IOP: DexDrive (GME) image detected\n");
			lseek(fd, 3904, SEEK_SET);
		} else if (strncmp(pageData, "VgsM", 4) == 0)
		{
			printf("IOP: VGS (MEM) image detected\n");
			lseek(fd, 64, SEEK_SET);
		} else if (strncmp(pageData, "PSV", 3) == 0)
		{
			printf("IOP: PSMag (PSX) image detected\n");
			lseek(fd, 256, SEEK_SET);
		} else
		{
			printf("IOP: Unformatted or unknown image detected\n");
			lseek(fd, 0, SEEK_SET);
		}
	//write pages
		for (i = 0; i < totalPages; i++)
		{
			memset(pageData, erasebyte, pageSize);
			retval = read(fd, pageData, pageSize);
			if (!retval)
			{
				close(fd);
				SysFree(pageData);
				while (semaphoreProgress) {DelayThread(1000);}
				semaphoreProgress = 1;
					progressBarData.promil = (i*1000)/totalPages;
					progressBarData.error = 1;
					progressBarData.finished = 1;
				semaphoreProgress = 0;

				semaphoreCopy = 0;
				return;
			}
			pMcWritePagePsx(slot, podslot, i, pageData, NULL);
			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (i*1000)/totalPages;
			semaphoreProgress = 0;
		}
		close(fd);
		SysFree(pageData);
		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = 1000;
			progressBarData.error = 0;
			progressBarData.finished = 1;
		semaphoreProgress = 0;
	}
	semaphoreCopy = 0;
}

void writeMcPsx(d_iopMcaCommand *iopCommand)
{
	u8* pageData = NULL;
	u32 i, k, retval;
	getCardSpecs();
	u32 type; //0 - none, 1 - psx, 2 - ps2, 3 - pda
	u16 pageSize;
	u16 pagesPerBlock;
	u32 totalPages;
	u8  flags;
	u32 slot = iopCommand->slot;
	u32 podslot = iopCommand->podslot;
	u8 erasebyte;
	int fd;

	while (semaphoreCopy) {DelayThread(10011);}

	semaphoreCopy = 1;
		type = mce_memcards[slot].type;
		pageSize = (iopCommand->force == 1 ? 128 : mce_memcards[slot].pageSize);
		pagesPerBlock = (iopCommand->force == 1 ? 128 : mce_memcards[slot].pagesPerBlock);
		totalPages = (iopCommand->force == 1 ? 1024 : mce_memcards[slot].totalPages);
		flags = (iopCommand->force == 1 ? 0 : mce_memcards[slot].flags);
	//semaphoreCopy = 0;

	fd = open(iopCommand->filePath, O_WRONLY | O_CREAT | O_TRUNC, 0);

// user defined size
	erasebyte = 0x0;

	if (((type == 0 || type != iopCommand->type) && iopCommand->force == 0) || pMcReadPagePsx == NULL || fd <= 0)
	{
		while (semaphoreProgress) {DelayThread(1000);}

		semaphoreProgress = 1;
			progressBarData.error = 1;
			progressBarData.finished = 1;
		semaphoreProgress = 0;

	} else
	{
		pageData = SysAlloc(pageSize);
		for (i = 0; i < totalPages; i++)
		{
			k = 0;
			while ((retval = pMcReadPagePsx(slot, podslot, i, pageData) != sceMcResSucceed) && (k++ < 4)){;}
			if (retval != sceMcResSucceed)
			{
				memset(pageData, erasebyte, pageSize);
			}
			retval = write(fd, pageData, pageSize);
			if (retval < pageSize)
			{
				close(fd);
				SysFree(pageData);
				while (semaphoreProgress) {DelayThread(1000);}
				semaphoreProgress = 1;
					progressBarData.promil = (i*1000)/totalPages;
					progressBarData.error = 1;
					progressBarData.finished = 1;
				semaphoreProgress = 0;

				semaphoreCopy = 0;
				return;
			}
			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (i*1000)/totalPages;
			semaphoreProgress = 0;
		}
		close(fd);
		SysFree(pageData);
		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = 1000;
			progressBarData.error = 0;
			progressBarData.finished = 1;
		semaphoreProgress = 0;
	}
	semaphoreCopy = 0;
}
void unformatPS2(d_iopMcaCommand *iopCommand)
{
	u32 i;
	getCardSpecs();
	u32 type; //0 - none, 1 - psx, 2 - ps2, 3 - pda
	u16 pageSize;
	u16 pagesPerBlock;
	u32 totalPages;
	u8  flags;
	u32 slot = iopCommand->slot;
	u32 podslot = iopCommand->podslot;

	while (semaphoreCopy) {DelayThread(10011);}

	semaphoreCopy = 1;
		type = mce_memcards[slot].type;
		pageSize = (iopCommand->force == 1 ? iopCommand->pageSize : mce_memcards[slot].pageSize);
		pagesPerBlock = (iopCommand->force == 1 ? iopCommand->blockSize : mce_memcards[slot].pagesPerBlock);
		totalPages = mce_memcards[slot].totalPages;
		flags = (iopCommand->force == 1 ? iopCommand->options : mce_memcards[slot].flags);
	//semaphoreCopy = 0;

	if (iopCommand->special) totalPages = iopCommand->special;

	if (type == 0 || type != iopCommand->type || pMcEraseBlock == NULL)
	{
		while (semaphoreProgress) {DelayThread(1000);}

		semaphoreProgress = 1;
			progressBarData.error = 1;
			progressBarData.finished = 1;
		semaphoreProgress = 0;

	} else
	{
		for (i = 0; i < totalPages/pagesPerBlock; i++)
		{
			fmtEraseBlock(slot, podslot, i*pagesPerBlock, pagesPerBlock);

			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (i*1000)/(totalPages/pagesPerBlock);
			semaphoreProgress = 0;
		}
		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = 1000;
			progressBarData.error = 0;
			progressBarData.finished = 1;
		semaphoreProgress = 0;
	}
	semaphoreCopy = 0;
}
void unformatPSX(d_iopMcaCommand *iopCommand)
{
	u32 i;
	getCardSpecs();
	u32 type; //0 - none, 1 - psx, 2 - ps2, 3 - pda
	u16 pageSize;
	u16 pagesPerBlock;
	u32 totalPages;
	u8  flags;
	u8* pageData = NULL;
	int retval = 0;
	u32 slot = iopCommand->slot;
	u32 podslot = iopCommand->podslot;

	while (semaphoreCopy) {DelayThread(10011);}

	semaphoreCopy = 1;
		type = mce_memcards[slot].type;
		pageSize = (iopCommand->force == 1 ? 128 : mce_memcards[slot].pageSize);
		pagesPerBlock = (iopCommand->force == 1 ? 128 : mce_memcards[slot].pagesPerBlock);
		totalPages = (iopCommand->force == 1 ? 1024 : mce_memcards[slot].totalPages);
		flags = (iopCommand->force == 1 ? 0 : mce_memcards[slot].flags);
	//semaphoreCopy = 0;

	if (((type == 0 || type != iopCommand->type) && iopCommand->force == 0) || pMcWritePagePsx == NULL)
	{
		while (semaphoreProgress) {DelayThread(1000);}

		semaphoreProgress = 1;
			progressBarData.error = 1;
			progressBarData.finished = 1;
		semaphoreProgress = 0;

	} else
	{
		pageData = SysAlloc(pageSize);
		for (i = 0; i < totalPages; i++)
		{
			memset(pageData, 0, pageSize);
			retval = pMcWritePagePsx(slot, podslot, i, pageData, NULL);
			if (retval != sceMcResSucceed) printf("IOP: psx write page %d, value %d\n", (int)i, retval);
			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (i*1000)/totalPages;
			semaphoreProgress = 0;
		}
		SysFree(pageData);

		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = 1000;
			progressBarData.error = 0;
			progressBarData.finished = 1;
		semaphoreProgress = 0;
	}
	semaphoreCopy = 0;
}
void formatPSX(d_iopMcaCommand *iopCommand)
{
	u32 i;
	getCardSpecs();
	u32 type; //0 - none, 1 - psx, 2 - ps2, 3 - pda
	u16 pageSize;
	u16 pagesPerBlock;
	u32 totalPages;
	u8  flags;
	u8* pageData = NULL;
	int retval = 0;
	u32 slot = iopCommand->slot;
	u32 podslot = iopCommand->podslot;

	while (semaphoreCopy) {DelayThread(10011);}

	semaphoreCopy = 1;
		type = mce_memcards[slot].type;
		pageSize = (iopCommand->force == 1 ? 128 : mce_memcards[slot].pageSize);
		pagesPerBlock = (iopCommand->force == 1 ? 128 : mce_memcards[slot].pagesPerBlock);
		totalPages = (iopCommand->force == 1 ? 1024 : mce_memcards[slot].totalPages);
		flags = (iopCommand->force == 1 ? 0 : mce_memcards[slot].flags);
	//semaphoreCopy = 0;

	if (((type == 0 || type != iopCommand->type) && iopCommand->force == 0) || pMcWritePagePsx == NULL)
	{
		while (semaphoreProgress) {DelayThread(1000);}

		semaphoreProgress = 1;
			progressBarData.error = 1;
			progressBarData.finished = 1;
		semaphoreProgress = 0;

	} else
	{
		if (iopCommand->fast != 0) totalPages = 64;
		i = 0;
		pageData = SysAlloc(pageSize);
	//superblock
		memset(pageData, 0, pageSize);
		pageData[0] = 'M';
		pageData[1] = 'C';
		pageData[127] = 0x0E;
		retval = pMcWritePagePsx(slot, podslot, i, pageData, NULL);
		if (retval != sceMcResSucceed) printf("IOP: psx write page %d, value %d\n", (int)i, retval);
		//i++;
		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = (i*1000)/totalPages;
		semaphoreProgress = 0;
	//data info blocks
		for (i = 1; i < 16; i++)
		{
			memset(pageData, 0, pageSize);
			pageData[0] = 0xA0;
			pageData[8] = 0xFF;
			pageData[9] = 0xFF;
			pageData[127] = 0xA0;
			retval = pMcWritePagePsx(slot, podslot, i, pageData, NULL);
			if (retval != sceMcResSucceed) printf("IOP: psx write page %d, value %d\n", (int)i, retval);

			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (i*1000)/totalPages;
			semaphoreProgress = 0;
		}
		for (i = 16; i < 36; i++)
		{
			memset(pageData, 0, pageSize);
			pageData[0] = 0xFF;
			pageData[1] = 0xFF;
			pageData[2] = 0xFF;
			pageData[3] = 0xFF;
			retval = pMcWritePagePsx(slot, podslot, i, pageData, NULL);
			if (retval != sceMcResSucceed) printf("IOP: psx write page %d, value %d\n", (int)i, retval);

			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (i*1000)/totalPages;
			semaphoreProgress = 0;
		}
		for (i = 36; i < totalPages; i++)
		{
			memset(pageData, 0, pageSize);
			retval = pMcWritePagePsx(slot, podslot, i, pageData, NULL);
			if (retval != sceMcResSucceed) printf("IOP: psx write page %d, value %d\n", (int)i, retval);
			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (i*1000)/totalPages;
			semaphoreProgress = 0;
		}
		SysFree(pageData);

		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = 1000;
			progressBarData.error = 0;
			progressBarData.finished = 1;
		semaphoreProgress = 0;
	}
	semaphoreCopy = 0;
}
void fmtScanBad(int port, int slot, void *buf, u_int *badlist, u32 size_in_pages, u32 pages_per_block, u32 fast)
{
	register int i, j, retval, retry, numbad, foundbad;
	printf("IOP: Scanning for bad blocks...\n");
	memset(badlist, 0xff, 32*4);
	numbad = 0;
	while (semaphoreProgress) {DelayThread(1000);}
	semaphoreProgress = 1;
		progressBarData.promil = 0;
	semaphoreProgress = 0;
	for (i = 0; i < size_in_pages/pages_per_block; i++)
	{
		foundbad = 0;
		for (j = 0; j < pages_per_block; j++)
		{
			retry = 0;
			while (((retval = pMcReadPage(port, slot, ((i * pages_per_block) + j), buf)) != sceMcResSucceed) && (retry++ < 4)){;}
			if (retval != sceMcResSucceed)
			{
				printf("Found bad block 0x%x\n", i);
				badlist[numbad++] = i;
				foundbad = 1;
				break;
			}
			if (fast) break;
		}
		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = (i*1000)/(size_in_pages/pages_per_block);
		semaphoreProgress = 0;
		if (foundbad) continue;
	}
}
u_int fmtCheckInBadRev(u_int *badlist, u_int sector, u32 page_size, u32 pages_per_block)
{
	register int i, block, foundbad;
	block = (sector*(1024/page_size))/pages_per_block;
	foundbad = 0;
	i = 0;
	while ((badlist[i] != 0xffffffff) && (i < 32))
	{
		if (badlist[i] == block)
		{
			block--;
			i = 0;
			foundbad = 1;
			continue;
		}
		i++;
	}
	if (foundbad) sector = (block*pages_per_block)/(1024/page_size);
	return sector;
}
u_int fmtCheckInBad(u_int *badlist, u_int sector, u32 page_size, u32 pages_per_block)
{
	register int i, block, foundbad;
	block = (sector*(1024/page_size))/pages_per_block;
	foundbad = 0;
	i = 0;
	while ((badlist[i] != 0xffffffff) && (i < 32))
	{
		if (badlist[i] == block)
		{
			block++;
			i = 0;
			foundbad = 1;
			continue;
		}
		i++;
	}
	if (foundbad) sector = (block*pages_per_block)/(1024/page_size);
	return sector;
}
int fmtCheckInBadOnly(u_int *badlist, u_int sector, u32 page_size, u32 pages_per_block)
{
	register int i, block;
	block = (sector*(1024/page_size))/pages_per_block;
	i = 0;
	while ((badlist[i] != 0xffffffff) && (i < 32))
	{
		if (badlist[i] == block)
		{
			return 1;
		}
		i++;
	}
	return 0;
}
void formatPS2(d_iopMcaCommand *iopCommand)
{
	int all_sector;
	fmtMCHeader myMcHeader;
	fmtMcEntry *myMcEntry;
	
	u32 i, j, k, x, y, last_blk_sector;
	u32 type; //0 - none, 1 - psx, 2 - ps2, 3 - pda
	u32 totalClusters2Write, writtenCluster;
	u16 pageSize;
	u16 pagesPerBlock;
	u32 totalPages;
	u8  flags, erasebyte;
	u32 slot = iopCommand->slot;
	u32 podslot = iopCommand->podslot;
	u32 write_page = 0, write_page2 = 0;
	char *mcbuffer;
	char *mcbuffer2;
	getCardSpecs();

	
	while (semaphoreCopy) {DelayThread(10011);}

	semaphoreCopy = 1;
		type = mce_memcards[slot].type;
		pageSize = (iopCommand->force == 1 ? iopCommand->pageSize : mce_memcards[slot].pageSize);
		pagesPerBlock = (iopCommand->force == 1 ? iopCommand->blockSize : mce_memcards[slot].pagesPerBlock);
		totalPages = mce_memcards[slot].totalPages;
		flags = (iopCommand->force == 1 ? iopCommand->options : mce_memcards[slot].flags);
	//semaphoreCopy = 0;

	if (iopCommand->special) totalPages = iopCommand->special;

	if (((type == 0 || type != iopCommand->type) && iopCommand->force == 0) || pMcEraseBlock == NULL || pMcReadPage == NULL || pMcWritePage == NULL)
	{
		while (semaphoreProgress) {DelayThread(1000);}

		semaphoreProgress = 1;
			progressBarData.error = 1;
			progressBarData.finished = 1;
		semaphoreProgress = 0;

	} else
	{
		for (i = 0; i < totalPages/pagesPerBlock; i++)
		{
			fmtEraseBlock(slot, podslot, i*pagesPerBlock, pagesPerBlock);

			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (i*1000)/(totalPages/pagesPerBlock);
			semaphoreProgress = 0;
		}
// format start
		erasebyte = (flags & 0x10) ? 0x0 : 0xFF;
		mcbuffer = (char*)SysAlloc((pageSize + 0xFF) & ~(u32)0xFF);
		mcbuffer2 = (char*)SysAlloc((pageSize + 0xFF) & ~(u32)0xFF);
		myMcEntry = (fmtMcEntry*)SysAlloc(sizeof(fmtMcEntry)*2);
		memset(mcbuffer, erasebyte, pageSize);

		memset(&myMcHeader, 0, sizeof(fmtMCHeader));
	//scan for bad
		fmtScanBad(slot, podslot, mcbuffer, myMcHeader.bad_block_list, totalPages, pagesPerBlock, iopCommand->fast);
	//format header - superblock
		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = 0;
		semaphoreProgress = 0;
		writtenCluster = 0;
		

		strcpy(myMcHeader.magic, "Sony PS2 Memory Card Format 1.2.0.0");
		myMcHeader.page_size = pageSize;
		myMcHeader.pages_per_cluster = 1024/pageSize;
		myMcHeader.pages_per_block = pagesPerBlock;
		myMcHeader.unk1 = 0xff00;
		myMcHeader.clusters_on_card = (totalPages*pageSize)/1024;
		//poprawic tego
		myMcHeader.first_allocatable = fmtCheckInBad(myMcHeader.bad_block_list, ((((totalPages-1)*(1024/pageSize))/1024)+1) + fmtCheckInBad(myMcHeader.bad_block_list, 8, pageSize, pagesPerBlock) +((totalPages-1)/(32768*(1024/pageSize)))+1, pageSize, pagesPerBlock);
		myMcHeader.last_allocatable = (myMcHeader.clusters_on_card - myMcHeader.first_allocatable) - ((myMcHeader.pages_per_block/myMcHeader.pages_per_cluster)*2);
		myMcHeader.root_cluster = 0;

		//sector = (((myMcHeader.backup_block1-1)*pages_per_block)/(1024/page_size));
		//block = ((((myMcHeader.backup_block1-1)*pages_per_block)/(1024/page_size))*(1024/page_size))/pages_per_block;
		
		myMcHeader.backup_block1 = ((fmtCheckInBadRev(myMcHeader.bad_block_list, (((totalPages/pagesPerBlock)-1)*pagesPerBlock)/(1024/pageSize), pageSize, pagesPerBlock))*(1024/pageSize))/pagesPerBlock;//(size_in_pages/pages_per_block)-1;
		myMcHeader.backup_block2 = ((((myMcHeader.backup_block1-1)*pagesPerBlock)/(1024/pageSize))*(1024/pageSize))/pagesPerBlock;
		memset(myMcHeader.unk_fill, 0, 8);

		totalClusters2Write = 1 //superblock
							+ 1 //root dir
							+ ((totalPages-1)/65536)+1 //fat indir clusters
							+ ((myMcHeader.last_allocatable-myMcHeader.first_allocatable)/256) //fat table
							;

		last_blk_sector = fmtCheckInBad(myMcHeader.bad_block_list, 8, pageSize, pagesPerBlock);
		for (i = 0; i <= ((totalPages-1)/65536); i++)
		{
			if ((((last_blk_sector+i)*(1024/pageSize))%pagesPerBlock) == 0)
			{
				last_blk_sector = fmtCheckInBad(myMcHeader.bad_block_list, last_blk_sector+i, pageSize, pagesPerBlock);
			}
			myMcHeader.indir_fat_clusters[i] = last_blk_sector+i;
		}
		//memset(myMcHeader.bad_block_list, 0xff, 32*4);
		myMcHeader.mc_type = 2;
		myMcHeader.mc_flag = flags;
		myMcHeader.unk2 = 0;
		myMcHeader.cluster_size = 1024;
		myMcHeader.unk3 = 0x100;
		myMcHeader.size_in_megs = (totalPages*pageSize)/(1024*1024);
		myMcHeader.unk4 = 0xffffffff;
		memset(myMcHeader.unk5, 0, 12);
		myMcHeader.max_used = (97*myMcHeader.clusters_on_card)/100;
		memset(myMcHeader.unk6, 0, 8);
		myMcHeader.unk7 = 0xffffffff;

		write_page = 0;

		memcpy(mcbuffer, &myMcHeader, sizeof(myMcHeader));
		fmtWritePage(slot, podslot, write_page, mcbuffer, pagesPerBlock, pageSize);
	//update progress
		writtenCluster++;
		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = (writtenCluster*1000)/totalClusters2Write;
		semaphoreProgress = 0;
	//end update progress
		write_page++;
	//format - indir fat clusters
		for (i=1; i<(myMcHeader.indir_fat_clusters[0]*myMcHeader.pages_per_cluster); i++, write_page++){}

		last_blk_sector = fmtCheckInBad(myMcHeader.bad_block_list, myMcHeader.indir_fat_clusters[0], pageSize, pagesPerBlock);
		all_sector = 0;
	//format - fat table
		for (x = 0; myMcHeader.indir_fat_clusters[x] != 0; x++)
		{
			last_blk_sector = fmtCheckInBad(myMcHeader.bad_block_list, myMcHeader.indir_fat_clusters[0]+x, pageSize, pagesPerBlock);
			write_page = last_blk_sector*(1024/pageSize);
			for (i = x*(1024/pageSize); (i < ((x+1)*(1024/pageSize))) && (i <= ((totalPages-1)/65536)); i++, write_page++)
			{
				memset(mcbuffer, erasebyte, pageSize);
				for (j = i*(256/myMcHeader.pages_per_cluster), k = 0; (j < i*(256/myMcHeader.pages_per_cluster)+(256/myMcHeader.pages_per_cluster)) && (j<((myMcHeader.last_allocatable/256)+1)); j++, k++)
				{
					((u32*)mcbuffer)[k] = ((totalPages-1)/65536)+last_blk_sector+1+j;
					
					memset(mcbuffer2, erasebyte, pageSize);
					write_page2 = ((u32*)mcbuffer)[k]*(1024/pageSize);
					for (y = 0; (y < (256/myMcHeader.pages_per_cluster)) && (all_sector < myMcHeader.last_allocatable); y++, all_sector++)
					{
						if ((y == 0) && (j == 0) && (i == 0))
						{
							((u32*)mcbuffer2)[y] = 0xffffffff;
						} else
						{
							((u32*)mcbuffer2)[y] = fmtCheckInBadOnly(myMcHeader.bad_block_list, all_sector+myMcHeader.first_allocatable, pageSize, pagesPerBlock) ? 0xfffffffd : 0x7fffffff;
						}
					}
					fmtWritePage(slot, podslot, write_page2, mcbuffer2, pagesPerBlock, pageSize);
				//update progress
					writtenCluster++;
					while (semaphoreProgress) {DelayThread(1000);}
					semaphoreProgress = 1;
						progressBarData.promil = (writtenCluster*1000)/totalClusters2Write;
					semaphoreProgress = 0;
				//end update progress
					write_page2++;
					memset(mcbuffer2, erasebyte, pageSize);
					for (y = 0; (y < (256/myMcHeader.pages_per_cluster)) && (all_sector < myMcHeader.last_allocatable); y++, all_sector++)
					{
						((u_int*)mcbuffer2)[y] = fmtCheckInBadOnly(myMcHeader.bad_block_list, all_sector+myMcHeader.first_allocatable, pageSize, pagesPerBlock) ? 0xfffffffd : 0x7fffffff;
					}
					fmtWritePage(slot, podslot, write_page2, mcbuffer2, pagesPerBlock, pageSize);

				//update progress
					writtenCluster++;
					while (semaphoreProgress) {DelayThread(1000);}
					semaphoreProgress = 1;
						progressBarData.promil = (writtenCluster*1000)/totalClusters2Write;
					semaphoreProgress = 0;
				//end update progress

				}
				fmtWritePageS(slot, podslot, write_page, mcbuffer, pagesPerBlock, pageSize);

			//update progress
				writtenCluster++;
				while (semaphoreProgress) {DelayThread(1000);}
				semaphoreProgress = 1;
					progressBarData.promil = (writtenCluster*1000)/totalClusters2Write;
				semaphoreProgress = 0;
			//end update progress
			}
		}
	//root dir
		write_page = myMcHeader.first_allocatable*(1024/pageSize);
	
		memset(myMcEntry, 0, sizeof(fmtMcEntry)*2);
		myMcEntry[0].attribs = 0x8427;
		myMcEntry[0].length = 2;
		myMcEntry[0].created.sec = 11;
		myMcEntry[0].created.year = 2000;
		myMcEntry[0].modified.sec = 11;
		myMcEntry[0].modified.year = 2000;
		myMcEntry[0].name[0] = '.';

		myMcEntry[1].attribs = 0xa426;
		myMcEntry[1].length = 0;
		myMcEntry[1].created.sec = 11;
		myMcEntry[1].created.year = 2000;
		myMcEntry[1].modified.sec = 11;
		myMcEntry[1].modified.year = 2000;
		myMcEntry[1].name[0] = '.';
		myMcEntry[1].name[1] = '.';


		if (myMcHeader.pages_per_cluster == 1)
		{
			memset(mcbuffer, erasebyte, pageSize);
			memcpy(mcbuffer, &myMcEntry[0], 1024);
			fmtWritePage(slot, podslot, write_page, mcbuffer, pagesPerBlock, pageSize);
		//update progress
			writtenCluster++;
			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (writtenCluster*1000)/totalClusters2Write;
			semaphoreProgress = 0;
		//end update progress
			write_page++;
		} else
		{
			memset(mcbuffer, erasebyte, pageSize);
			memcpy(mcbuffer, &myMcEntry[0], 512);
			fmtWritePage(slot, podslot, write_page, mcbuffer, pagesPerBlock, pageSize);
		//update progress
			writtenCluster++;
			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (writtenCluster*1000)/totalClusters2Write;
			semaphoreProgress = 0;
		//end update progress
			write_page++;

			memset(mcbuffer, erasebyte, pageSize);
			memcpy(mcbuffer, &myMcEntry[1], 512);
			fmtWritePage(slot, podslot, write_page, mcbuffer, pagesPerBlock, pageSize);
		//update progress
			writtenCluster++;
			while (semaphoreProgress) {DelayThread(1000);}
			semaphoreProgress = 1;
				progressBarData.promil = (writtenCluster*1000)/totalClusters2Write;
			semaphoreProgress = 0;
		//end update progress
			write_page++;
		}

		SysFree(myMcEntry);
		SysFree(mcbuffer);
		SysFree(mcbuffer2);
// format end
		while (semaphoreProgress) {DelayThread(1000);}
		semaphoreProgress = 1;
			progressBarData.promil = 1000;
			progressBarData.error = 0;
			progressBarData.finished = 1;
		semaphoreProgress = 0;
	}
	semaphoreCopy = 0;
}
void fmtEraseBlock(int port, int slot, int page, u32 pages_per_block)
{
	int retval = 0;
	//printf("IOP: EraseBlock %d\n", (int)(page/pages_per_block));
	retval = pMcEraseBlock(port, slot, (int)(page/pages_per_block), 0);
	if (retval < 0) printf("IOP: EraseBlock %d failed with %d\n", (int)(page/pages_per_block), retval);
	//realErase;
}
void fmtEraseCard(int port, int slot, u32 size_in_pages, u32 pages_per_block)
{
	register int i;
	printf("IOP: Erasing card...\n");
	for (i = 0; i < size_in_pages/pages_per_block; i++)
	{
		fmtEraseBlock(port, slot, i*pages_per_block, pages_per_block);
	}
}

