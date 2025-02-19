#ifndef _GUIFRAMERENDERERPS2_H_
#define _GUIFRAMERENDERERPS2_H_ 1

#include "GUIFramePS2Includes.h"
#include "IGUIFrameRenderer.h"

#include "GUIFrameTexturePS2.h"

class CGUIFrameRendererPS2 :
	public CIGUIFrameRenderer
{
private:
	GSGLOBAL *m_gsGlobal;
	u32 m_textureStartPointer;
	float m_xscale;
	float m_yscale;
	float m_xmove;
	float m_ymove;
public:
	enum etFlagsPS2
	{
		etFLPS2enableZ		= 0x00000001,
		etFLPS2enableDith	= 0x00000002,
		etFLPS2automode		= 0x00000004,
		etFLPS2novsync		= 0x00000008,
		etFLPS2mode16_9		= 0x00000010
	};
	CGUIFrameRendererPS2(void);
	~CGUIFrameRendererPS2(void);

//interface implementation
public:
	bool initRenderer(u32 width, u32 height, u32 vwidth, u32 vheight, u32 bpp, u32 zbuf, u32 mode, u32 flags);
	bool deinitRenderer();

	void setAlpha(bool enable);
	void setTestAlpha(bool enable);
	void setScissor(bool enable = false, int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);
	void swapBuffers();
	void clearFrontBuffer();
	CIGUIFrameTexture *getFrameTex(int divfac = 0);
	void restoreFrameTex(CIGUIFrameTexture *tex);
	void screenshot(u8 *buff);
	void downloadVram(void *virtPage, int physPage, int addr, int bufw, int x, int y, int w, int h, int psm);

//2D drawing functions
	//not shaded line
	void drawLineF(const float x1, const float y1, const float x2, const float y2, const u8 r, const u8 g, const u8 b, float alpha);
	//shaded line
	void drawLineG(const float x1, const float y1, const float x2, const float y2, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, float alpha1, float alpha2);
	//flat triangle
	void drawTriangleF(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const u8 r, const u8 g, const u8 b, float alpha);
	//shaded triangle
	void drawTriangleG(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, float alpha1, float alpha2, float alpha3);
	//flat quad
	void drawQuadF(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float x4, const float y4, const u8 r, const u8 g, const u8 b, float alpha);
	//shaded quad
	void drawQuadG(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float x4, const float y4, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, const u8 r4, const u8 g4, const u8 b4, float alpha1, float alpha2, float alpha3, float alpha4);
	//textured flat triangle
	void drawTriangleFT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const u8 r, const u8 g, const u8 b, float alpha);
	//textured shaded triangle
	void drawTriangleGT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, float alpha1, float alpha2, float alpha3);
	//textured flat quad 
	void drawQuadFT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const float x4, const float y4, const float u4, const float v4, const u8 r, const u8 g, const u8 b, float alpha);
	//textured sprite
	void drawSpriteT(CIGUIFrameTexture *texture, const float x, const float y, const float w, const float h, const float u1, const float v1, const float u2, const float v2, const u8 r, const u8 g, const u8 b, float alpha);
	void drawSpriteGT(CIGUIFrameTexture *texture, const float x, const float y, const float w, const float h, const float u1, const float v1, const float u2, const float v2, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, const u8 r4, const u8 g4, const u8 b4, float alpha1, float alpha2, float alpha3, float alpha4);
	//textured shaded quad
	void drawQuadGT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const float x4, const float y4, const float u4, const float v4, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, const u8 r4, const u8 g4, const u8 b4, float alpha1, float alpha2, float alpha3, float alpha4);

//PS2 specific stuff
	void clearVram();
	bool checkVram(CGUIFrameTexturePS2 *texture);
	void uploadTexInline(CGUIFrameTexturePS2 *texture);
};

#endif //_GUIFRAMERENDERERPS2_H_
