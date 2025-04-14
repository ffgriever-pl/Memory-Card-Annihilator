#ifndef _IGUIFRAMERENDERER_H_
#define _IGUIFRAMERENDERER_H_

#include "IGUIFrameIncludes.h"
#include "IGUIFrameTexture.h"

class CIGUIFrameRenderer
{
protected:
	bool m_bInitDone;
	int m_width;
	int m_height;
	int m_vwidth;
	int m_vheight;
public:
	u32 getFrameWidth()//virtual screen
	{
		return m_vwidth;
	}
	u32 getFrameHeight()//virtual screen
	{
		return m_vheight;
	}
	CIGUIFrameRenderer(void);
	virtual ~CIGUIFrameRenderer(void);
	virtual bool initRenderer(u32 width, u32 height, u32 vwidth, u32 vheight, u32 bpp, u32 zbuf, u32 mode, u32 flags) = 0;
	virtual bool deinitRenderer() = 0;

	virtual void setAlpha(bool enable) = 0;
	virtual void setTestAlpha(bool enable) = 0;
	virtual void setScissor(bool enable = false, int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0) = 0;
	virtual void swapBuffers() = 0;
	virtual void clearFrontBuffer() = 0;
	virtual CIGUIFrameTexture *getFrameTex(int divfac = 0) = 0;
	virtual void restoreFrameTex(CIGUIFrameTexture *tex) = 0;
//2D drawing functions
	//color is 0-128, above overbright
	//not shaded line
	virtual void drawLineF(const float x1, const float y1, const float x2, const float y2, const u8 r, const u8 g, const u8 b, float alpha) = 0;
	void drawLineF(const tVertex2 &start, const tVertex2 &end, const tRGBColor &color, float alpha);
	//shaded line
	virtual void drawLineG(const float x1, const float y1, const float x2, const float y2, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, float alpha1, float alpha2) = 0;
	void drawLineG(const tVertex2 &start, const tVertex2 &end, const tRGBColor &color1, const tRGBColor &color2, float alpha1, float alpha2);
	//flat triangle
	virtual void drawTriangleF(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const u8 r, const u8 g, const u8 b, float alpha) = 0;
	void drawTriangleF(const tVertex2 &p1, const tVertex2 &p2, const tVertex2 &p3, const tRGBColor &color, float alpha);
	//shaded triangle
	virtual void drawTriangleG(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, float alpha1, float alpha2, float alpha3) = 0;
	void drawTriangleG(const tVertex2 &p1, const tVertex2 &p2, const tVertex2 &p3, const tRGBColor &color1, const tRGBColor &color2, const tRGBColor &color3, float alpha1, float alpha2, float alpha3);
	//flat quad
	virtual void drawQuadF(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float x4, const float y4, const u8 r, const u8 g, const u8 b, float alpha) = 0;
	void drawQuadF(const tVertex2 &p1, const tVertex2 &p2, const tVertex2 &p3, const tVertex2 &p4, const tRGBColor &color, float alpha);
	//shaded quad
	virtual void drawQuadG(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float x4, const float y4, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, const u8 r4, const u8 g4, const u8 b4, float alpha1, float alpha2, float alpha3, float alpha4) = 0;
	void drawQuadG(const tVertex2 &p1, const tVertex2 &p2, const tVertex2 &p3, const tVertex2 &p4, const tRGBColor &color1, const tRGBColor &color2, const tRGBColor &color3, const tRGBColor &color4, float alpha1, float alpha2, float alpha3, float alpha4);
	//textured flat triangle
	virtual void drawTriangleFT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const u8 r, const u8 g, const u8 b, float alpha) = 0;
	void drawTriangleFT(CIGUIFrameTexture *texture, const tVertex2 &p1, const tVertex2 &m1, const tVertex2 &p2, const tVertex2 &m2, const tVertex2 &p3, const tVertex2 &m3, const tRGBColor &color, float alpha);
	//textured shaded triangle
	virtual void drawTriangleGT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, float alpha1, float alpha2, float alpha3) = 0;
	void drawTriangleGT(CIGUIFrameTexture *texture, const tVertex2 &p1, const tVertex2 &m1, const tVertex2 &p2, const tVertex2 &m2, const tVertex2 &p3, const tVertex2 &m3, const tRGBColor &color1, const tRGBColor &color2, const tRGBColor &color3, float alpha1, float alpha2, float alpha3);
	//textured flat quad 
	virtual void drawQuadFT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const float x4, const float y4, const float u4, const float v4, const u8 r, const u8 g, const u8 b, float alpha) = 0;
	void drawQuadFT(CIGUIFrameTexture *texture, const tVertex2 &p1, const tVertex2 &m1, const tVertex2 &p2, const tVertex2 &m2, const tVertex2 &p3, const tVertex2 &m3, const tVertex2 &p4, const tVertex2 &m4, const tRGBColor &color, float alpha);
	//textured sprite
	virtual void drawSpriteT(CIGUIFrameTexture *texture, const float x, const float y, const float w, const float h, const float u1, const float v1, const float u2, const float v2, const u8 r, const u8 g, const u8 b, float alpha) = 0;
	void drawSpriteT(CIGUIFrameTexture *texture, const tVertex2 &pos, const tVertex2 &size, const tVertex2 &m1, const tVertex2 &m2, const tRGBColor &color, float alpha);
	virtual void drawSpriteGT(CIGUIFrameTexture *texture, const float x, const float y, const float w, const float h, const float u1, const float v1, const float u2, const float v2, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, const u8 r4, const u8 g4, const u8 b4, float alpha1, float alpha2, float alpha3, float alpha4) = 0;
	void drawSpriteGT(CIGUIFrameTexture *texture, const tVertex2 &pos, const tVertex2 &size, const tVertex2 &m1, const tVertex2 &m2, const tRGBColor &color1, const tRGBColor &color2, const tRGBColor &color3, const tRGBColor &color4, float alpha1, float alpha2, float alpha3, float alpha4);
	//textured shaded quad
	virtual void drawQuadGT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const float x4, const float y4, const float u4, const float v4, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, const u8 r4, const u8 g4, const u8 b4, float alpha1, float alpha2, float alpha3, float alpha4) = 0;
	void drawQuadGT(CIGUIFrameTexture *texture, const tVertex2 &p1, const tVertex2 &m1, const tVertex2 &p2, const tVertex2 &m2, const tVertex2 &p3, const tVertex2 &m3, const tVertex2 &p4, const tVertex2 &m4, const tRGBColor &color1, const tRGBColor &color2, const tRGBColor &color3, const tRGBColor &color4, float alpha1, float alpha2, float alpha3, float alpha4);

//3D functions
};

#endif //_IGUIFRAMERENDERER_H_
