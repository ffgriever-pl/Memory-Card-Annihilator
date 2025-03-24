#ifndef _CDVD_RPC_H
#define _CDVD_RPC_H

#ifdef __cplusplus
extern "C" {
#endif

#define CDVD_IRX 0xB001337
#define CDVD_FINDFILE 0x01
#define CDVD_GETDIR 0x02
#define CDVD_STOP 0x04
#define CDVD_TRAYREQ 0x05
#define CDVD_DISKREADY 0x06
#define CDVD_FLUSHCACHE 0x07
#define CDVD_GETSIZE 0x08

struct TocEntry
{
	u32 fileLBA;
	u32 fileSize;
	u8 fileProperties;
	u8 padding1[3];
	char filename[128 + 1];
	u8 padding2[3];
} __attribute__((packed));


enum CDVD_getMode {
	CDVD_GET_FILES_ONLY = 1,
	CDVD_GET_DIRS_ONLY = 2,
	CDVD_GET_FILES_AND_DIRS = 3
};

// Macros for TrayReq
#define CdTrayOpen 0
#define CdTrayClose 1
#define CdTrayCheck 2

// Macros for DiskReady
#define CdComplete 0x02
#define CdNotReady 0x06
#define CdBlock 0x00
#define CdNonBlock 0x01

int CDVD_Init();
int CDVD_DiskReady(int mode);
int CDVD_FindFile(const char *fname, struct TocEntry *tocEntry);
void CDVD_Stop();
int CDVD_TrayReq(int mode);
int CDVD_DiskReady(int mode);
int CDVD_GetDir(const char *pathname, const char *extensions, enum CDVD_getMode getMode, struct TocEntry tocEntry[], unsigned int req_entries, char *new_pathname);
void CDVD_FlushCache();
unsigned int CDVD_GetSize();

#define CDVD_TYPE_DETECT            SCECdDETCT
#define CDVD_TYPE_DETECT_CD         SCECdDETCTCD
#define CDVD_TYPE_DETECT_DVDSINGLE  SCECdDETCTDVDS
#define CDVD_TYPE_DETECT_DVDDUAL    SCECdDETCTDVDD
#define CDVD_TYPE_PS1CD             SCECdPSCD
#define CDVD_TYPE_PS1CDDA           SCECdPSCDDA
#define CDVD_TYPE_PS2CD             SCECdPS2CD
#define CDVD_TYPE_PS2CDDA           SCECdPS2CDDA
#define CDVD_TYPE_PS2DVD            SCECdPS2DVD
#define CDVD_TYPE_UNKNOWN           SCECdUNKNOWN
#define CDVD_TYPE_NODISK            SCECdNODISC
#define CDVD_TYPE_ILLEGAL           SCECdIllegalMedia
#define CDVD_TYPE_DVDVIDEO          SCECdDVDV
#define CDVD_TYPE_CDDA              SCECdCDDA


#ifdef __cplusplus
}
#endif


#endif  // _CDVD_H
