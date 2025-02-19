#ifndef FORMATTER_H
#define FORMATTER_H

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
#include "psxcdr_rpc.h"

typedef unsigned char  u_char;
typedef unsigned int   u_int;
typedef unsigned short u_short;
typedef unsigned long  u_long;

#define VOIDPTR(x)                    (*(void **)&(x))

/* memory card error codes */
#define sceMcResSucceed               (0)
#define sceMcResChangedCard           (-1)
#define sceMcResNoFormat              (-2)
#define sceMcResFullDevice            (-3)
#define sceMcResNoEntry               (-4)
#define sceMcResDeniedPermit          (-5)
#define sceMcResNotEmpty              (-6)
#define sceMcResUpLimitHandle         (-7)
#define sceMcResFailReplace           (-8)

/* memory card types */
#define sceMcTypeNoCard               (0)
#define sceMcTypePS1                  (1)
#define sceMcTypePS2                  (2)
#define sceMcTypePDA                  (3)

#define NO_RESIDENT_END               MODULE_NO_RESIDENT_END
#define SMEM_Low                      ALLOC_FIRST
#define LibInfo                       iop_library_t

void *QueryLibraryEntryTable(LibInfo *lib);


typedef
struct _McSpec
{
 u_short  PageSize; /* Page size in bytes (user data only) */
 u_short  BlockSize; /* Block size in pages */
 u_int    CardSize; /* Total number of pages */
} McSpec;

#define PS2_CARDINFO_SIGN           0x3249434D

typedef
struct _CardInfo
{
 u_int    sign;
 McSpec   spec;
 u_int    flag;
} CardInfo;

typedef
struct _fmtMCHeader
{
	char magic[40]; //"Sony PS2 Memory Card Format 1.2.0.0"
	u_short page_size; //512 usually - hardware restricted
	u_short pages_per_cluster; //2 usually, always has to be 1024bytes
	u_short pages_per_block; //16 usually - hw restr
	u_short unk1; //0xff00
	u_int clusters_on_card; //8192 on 8MB
	u_int first_allocatable; //cluster, 41 default 0x29 - po wszystkich, tam gdzie root //wszystko int ((((size_in_pages-1)*2)/1024)+1) +8 +((size_in_pages-1)/65536)+1
	u_int last_allocatable; //relative, cluster, 8135... 0x1fc7 : (clusters_on_card - first_allocatable) - ((pages_per_block/pages_per_cluster)*2)
	u_int root_cluster; //relative, must be zero
	u_int backup_block1; //usually last block on card; 1023 na 8MB
	u_int backup_block2; //usually last block on card - 1; 1022 on 8MB
	u_char unk_fill[8]; //zero
	u_int indir_fat_clusters[32]; //on 8MB-64MB it's 8,0,0,0,..., on 128MB 8,9,0,0,...
	u_int bad_block_list[32]; //0xff if end of list, number if wrong
	u_char mc_type; //2 for PS2MC
	u_char mc_flag; //0x2b usually
	u_short unk2; //zero
	u_int cluster_size; //1024 always, 0x400
	u_int unk3; //0x100
	u_int size_in_megs; //size in megabytes
	u_int unk4; //0xffffffff
	u_char unk5[12]; //zero
	u_int max_used; //97% of total clusters
	u_char unk6[8]; //zero
	u_int unk7; //0xffffffff
} fmtMCHeader;

typedef
struct _fmtMcTime
{
	u_char unk1; //0
	u_char sec; //0x0b
	u_char min; //0
	u_char hour; //0
	u_char day; //0
	u_char month; //0
	u_short year; //2000
} fmtMcTime;

typedef
struct _fmtMcEntry
{
	u_int attribs; //0x8427 for . dirs and 0xa426 for ..
	u_int length; //num of files for dirs
	fmtMcTime created; //yr 2000 12sec
	u_int firstCluster; //zero for . and .. in root
	u_int parentCluster; //only for . dirs, zero in root .
	fmtMcTime modified; //yr 2000 12sec
	u_char unk1[32]; //zero
	char name[32]; //ASCII
	u_char fill[416]; //zero
} fmtMcEntry;
/* internal routines */
void *GetLibraryEntry(char *libname, int version, int entryno);
int McGetDefSpec2(int port, int slot, u_short *pagesize, u_short *blocksize, u_int *cardsize, u_char *cardflags);
void *SysAlloc(u_long size);
int SysFree(void *area);
void LockMcman();
void FreeMcman();

int (*pMcGetIoSema)();
int (*pMcDetectCard)(int port, int slot);
int (*pMcGetSpec)(int port, int slot, u16 *pagesize, u16 *blocksize, u32 *cardsize, u8 *cardflags);
int (*pMcReadPage)(int port, int slot, int page, void *buf);
int (*pMcReadPagePsx)(int port, int slot, int page, void *buf);
int (*pMcWritePage)(int port, int slot, int page, void *buf, void *cbuf);
int (*pMcWritePagePsx)(int port, int slot, int page, void *buf, void *cbuf);
int (*pMcEraseBlock)(int port, int slot, int page, void* buf);
int (*pMcGetCardType)(int port, int slot);

void initLibEntries();
void CalculateECC(u_char *buf, u_char *chk);
void readMcPs2(d_iopMcaCommand *iopCommand);
void readMcPsx(d_iopMcaCommand *iopCommand);
void writeMcPs2(d_iopMcaCommand *iopCommand);
void writeMcPsx(d_iopMcaCommand *iopCommand);
void unformatPS2(d_iopMcaCommand *iopCommand);
void unformatPSX(d_iopMcaCommand *iopCommand);
void formatPS2(d_iopMcaCommand *iopCommand);
void formatPSX(d_iopMcaCommand *iopCommand);
void fmtEraseBlock(int port, int slot, int page, u32 pages_per_block);
void fmtEraseCard(int port, int slot, u32 size_in_pages, u32 pages_per_block);
void fmtWritePage(int port, int slot, int page, void *buf, u32 pages_per_block, u32 pageSize);
void fmtScanBad(int port, int slot, void *buf, u_int *badlist, u32 size_in_pages, u32 pages_per_block, u32 fast);
u_int fmtCheckInBadRev(u_int *badlist, u_int sector, u32 page_size, u32 pages_per_block);
u_int fmtCheckInBad(u_int *badlist, u_int sector, u32 page_size, u32 pages_per_block);
int fmtCheckInBadOnly(u_int *badlist, u_int sector, u32 page_size, u32 pages_per_block);

#endif
