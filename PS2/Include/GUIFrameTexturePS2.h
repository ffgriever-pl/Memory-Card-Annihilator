#ifndef _GUIFRAMETEXTUREPS2_H_
#define _GUIFRAMETEXTUREPS2_H_ 1

#include "GUIFramePS2Includes.h"
#include "IGUIFrameTexture.h"
#include <list>

#define TX_GAUSSIAN_RADIUS (2) 
//min 1, max 248!

#define txmax(a,b) (a > b ? a : b)
#define txmin(a,b) (a < b ? a : b)

class CGUIFrameTexturePS2 :
	public CIGUIFrameTexture
{
private:
	struct t_tim2header
	{
		unsigned int magic; //"TIM2" lub "TIM3"
		char unk1;
		char mangled;//0x06 - pokrojony, inne ok
		unsigned char layers;
		char unnk2[9];
		unsigned int size; //total - 16
		unsigned short clutSize;
		unsigned short unk3;
		unsigned int imagesize;//samego obrazu
		short headerLen; //gdzie zaczyna sie data
		char unk5[5];
		char bpp; //1 - 16bit, 2 - 24, 3 - 32, 4 - 4, 5 - 8
		short width;
		short height;
		u8 unk6[0x18];
	};
	GSTEXTURE m_Texture;
	u128 *m_texbuf;
	u8   *m_texbuf8;
	bool m_loadedVram;

	static int radius;
	static int kernelSize;
	static int kernel[TX_GAUSSIAN_RADIUS*2 +1];
	static int mult[TX_GAUSSIAN_RADIUS*2 +1][256];
	static u8 dv[256*3]; //radius 1
	static bool done;
	void setRadius(int sz);
	void blur24(u8 *pix, int x, int y,int w,int h, int imw, int imh);
	void blur24st(u8 *pix, int x, int y,int w,int h, int imw, int imh);
	void fastblur24(u8 *pix,int radius, int w, int h);
public:
	CGUIFrameTexturePS2(void);
	~CGUIFrameTexturePS2(void);
//interface implementation
public:
	eTextureResult loadTextureBuffer(u8 *buffer, u32 size, bool staticBuffer);
	eTextureResult loadTextureFile(char filename[]);
	void setTexfilter(eTextureFilter filter);
	void blur(int size);
//specific PS2 functions
public:
	static std::list<CGUIFrameTexturePS2*> m_tex_list;
	GSTEXTURE *getTexPointer();
	bool getVramStatus();
	void setVramStatus(bool status);
	bool loadTextureBufferTim2(u8 *res_data);
	CGUIFrameTexturePS2::eTextureResult loadTextureBufferRaw(int PSM, u8 *buffer, int width, int height, int size);
};

#endif //_GUIFRAMETEXTUREPS2_H_
