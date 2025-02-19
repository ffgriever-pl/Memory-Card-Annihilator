#include "Include\IGUIFrameRenderer.h"


CIGUIFrameRenderer::CIGUIFrameRenderer(void)
	: m_bInitDone(false)
{
}


CIGUIFrameRenderer::~CIGUIFrameRenderer(void)
{
}

void CIGUIFrameRenderer::drawLineF(const tVertex2 &start, const tVertex2 &end, const tRGBColor &color, float alpha)
{
	drawLineF(start.x, start.y, end.x, end.y, color.r, color.g, color.b, alpha);
}
void CIGUIFrameRenderer::drawLineG(const tVertex2 &start, const tVertex2 &end, const tRGBColor &color1, const tRGBColor &color2, float alpha1, float alpha2)
{
	drawLineG(start.x, start.y, end.x, end.y, color1.r, color1.g, color1.b, color2.r, color2.g, color2.b, alpha1, alpha2);
}
void CIGUIFrameRenderer::drawTriangleF(const tVertex2 &p1, const tVertex2 &p2, const tVertex2 &p3, const tRGBColor &color, float alpha)
{
	drawTriangleF(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color.r, color.g, color.b, alpha);
}
void CIGUIFrameRenderer::drawTriangleG(const tVertex2 &p1, const tVertex2 &p2, const tVertex2 &p3, const tRGBColor &color1, const tRGBColor &color2, const tRGBColor &color3, float alpha1, float alpha2, float alpha3)
{
	drawTriangleG(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color1.r, color1.g, color1.b, color2.r, color2.g, color2.b, color3.r, color3.g, color3.b, alpha1, alpha2, alpha3);
}
void CIGUIFrameRenderer::drawQuadF(const tVertex2 &p1, const tVertex2 &p2, const tVertex2 &p3, const tVertex2 &p4, const tRGBColor &color, float alpha)
{
	drawQuadF(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, color.r, color.g, color.b, alpha);
}
void CIGUIFrameRenderer::drawQuadG(const tVertex2 &p1, const tVertex2 &p2, const tVertex2 &p3, const tVertex2 &p4, const tRGBColor &color1, const tRGBColor &color2, const tRGBColor &color3, const tRGBColor &color4, float alpha1, float alpha2, float alpha3, float alpha4)
{
	drawQuadG(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, color1.r, color1.g, color1.b, color2.r, color2.g, color2.b, color3.r, color3.g, color3.b, color4.r, color4.g, color4.b, alpha1, alpha2, alpha3, alpha4);
}
void CIGUIFrameRenderer::drawTriangleFT(CIGUIFrameTexture *texture, const tVertex2 &p1, const tVertex2 &m1, const tVertex2 &p2, const tVertex2 &m2, const tVertex2 &p3, const tVertex2 &m3, const tRGBColor &color, float alpha)
{
	drawTriangleFT(texture, p1.x, p1.y, m1.x, m1.y, p2.x, p2.y, m2.x, m2.y, p3.x, p3.y, m3.x, m3.y, color.r, color.g, color.b, alpha);
}
void CIGUIFrameRenderer::drawTriangleGT(CIGUIFrameTexture *texture, const tVertex2 &p1, const tVertex2 &m1, const tVertex2 &p2, const tVertex2 &m2, const tVertex2 &p3, const tVertex2 &m3, const tRGBColor &color1, const tRGBColor &color2, const tRGBColor &color3, float alpha1, float alpha2, float alpha3)
{
	drawTriangleGT(texture, p1.x, p1.y, m1.x, m1.y, p2.x, p2.y, m2.x, m2.y, p3.x, p3.y, m3.x, m3.y, color1.r, color1.g, color1.b, color2.r, color2.g, color2.b, color3.r, color3.g, color3.b, alpha1, alpha2, alpha3);
}
void CIGUIFrameRenderer::drawQuadFT(CIGUIFrameTexture *texture, const tVertex2 &p1, const tVertex2 &m1, const tVertex2 &p2, const tVertex2 &m2, const tVertex2 &p3, const tVertex2 &m3, const tVertex2 &p4, const tVertex2 &m4, const tRGBColor &color, float alpha)
{
	drawQuadFT(texture, p1.x, p1.y, m1.x, m1.y, p2.x, p2.y, m2.x, m2.y, p3.x, p3.y, m3.x, m3.y, p4.x, p4.y, m4.x, m4.y, color.r, color.g, color.b, alpha);
}
void CIGUIFrameRenderer::drawQuadGT(CIGUIFrameTexture *texture, const tVertex2 &p1, const tVertex2 &m1, const tVertex2 &p2, const tVertex2 &m2, const tVertex2 &p3, const tVertex2 &m3, const tVertex2 &p4, const tVertex2 &m4, const tRGBColor &color1, const tRGBColor &color2, const tRGBColor &color3, const tRGBColor &color4, float alpha1, float alpha2, float alpha3, float alpha4)
{
	drawQuadGT(texture, p1.x, p1.y, m1.x, m1.y, p2.x, p2.y, m2.x, m2.y, p3.x, p3.y, m3.x, m3.y, p4.x, p4.y, m4.x, m4.y, color1.r, color1.g, color1.b, color2.r, color2.g, color2.b, color3.r, color3.g, color3.b, color4.r, color4.g, color4.b, alpha1, alpha2, alpha3, alpha4);
}
void CIGUIFrameRenderer::drawSpriteT(CIGUIFrameTexture *texture, const tVertex2 &pos, const tVertex2 &size, const tVertex2 &m1, const tVertex2 &m2, const tRGBColor &color, float alpha)
{
	drawSpriteT(texture, pos.x, pos.y, size.x, size.y, m1.x, m1.y, m2.x, m2.y, color.r, color.g, color.b, alpha);
}
void CIGUIFrameRenderer::drawSpriteGT(CIGUIFrameTexture *texture, const tVertex2 &pos, const tVertex2 &size, const tVertex2 &m1, const tVertex2 &m2, const tRGBColor &color1, const tRGBColor &color2, const tRGBColor &color3, const tRGBColor &color4, float alpha1, float alpha2, float alpha3, float alpha4)
{
	drawSpriteGT(texture, pos.x, pos.y, size.x, size.y, m1.x, m1.y, m2.x, m2.y, color1.r, color1.g, color1.b, color2.r, color2.g, color2.b, color3.r, color3.g, color3.b, color4.r, color4.g, color4.b, alpha1, alpha2, alpha3, alpha4);
}
