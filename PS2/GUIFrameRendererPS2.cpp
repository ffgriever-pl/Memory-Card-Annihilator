#include "Include/GUIFrameRendererPS2.h"


CGUIFrameRendererPS2::CGUIFrameRendererPS2(void)
	: m_textureStartPointer(0)
{
}


CGUIFrameRendererPS2::~CGUIFrameRendererPS2(void)
{
	deinitRenderer();
}

bool CGUIFrameRendererPS2::deinitRenderer()
{
	if (m_bInitDone)
	{
		gsKit_deinit_global(m_gsGlobal);
		m_bInitDone = false;
	}
	return true;
}

bool CGUIFrameRendererPS2::initRenderer(u32 width, u32 height, u32 vwidth, u32 vheight, u32 bpp, u32 zbuf, u32 mode, u32 flags)
{
	if (!m_bInitDone)
	{
		dmaKit_init(D_CTRL_RELE_OFF,D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC, D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);

		dmaKit_chan_init(DMA_CHANNEL_GIF);
		dmaKit_chan_init(DMA_CHANNEL_FROMSPR);
		dmaKit_chan_init(DMA_CHANNEL_TOSPR);
		dmaKit_chan_init(DMA_CHANNEL_VIF1);
		//FIXME: Add vif0 and vif1?

		m_gsGlobal = gsKit_init_global();
		if (!(flags & etFLPS2automode))
		{
			m_gsGlobal->Mode = mode;
			m_gsGlobal->Aspect = (flags&etFLPS2mode16_9) ? GS_ASPECT_16_9 : GS_ASPECT_4_3;
			m_gsGlobal->Width = width;
			m_gsGlobal->Height = height;
		}
		m_width = m_gsGlobal->Width;
		m_height = m_gsGlobal->Height;
		m_vwidth = vwidth;
		m_vheight = vheight;

		m_xmove = 0.0f;
		m_ymove = 0.0f;
		m_xscale = (float)m_width/(float)m_vwidth;
		m_yscale = (float)m_height/(float)m_vheight;

		m_gsGlobal->PSM = bpp;
		m_gsGlobal->Dithering = GS_SETTING_OFF;
		if (bpp == GS_PSMZ_16 || bpp == GS_PSM_CT16S)
		{
			m_gsGlobal->Dithering = (flags&etFLPS2enableDith) ? GS_SETTING_ON : GS_SETTING_OFF;
		}
		if (flags&etFLPS2enableZ)
		{
			m_gsGlobal->ZBuffering = GS_SETTING_ON;
			m_gsGlobal->PSMZ = GS_PSMZ_16;
		} else
		{
			m_gsGlobal->ZBuffering = GS_SETTING_OFF;
		}
		m_gsGlobal->PrimAAEnable = GS_SETTING_OFF;
		m_gsGlobal->PrimAlphaEnable = GS_SETTING_ON;

		gsKit_init_screen(m_gsGlobal);
		gsKit_mode_switch(m_gsGlobal, GS_ONESHOT);
		gsKit_set_dither(m_gsGlobal);

		gsKit_clear(m_gsGlobal, GS_SETREG_RGBAQ(0x00,0x00,0x00,0x80,0x00));
		gsKit_sync_flip(m_gsGlobal);
		gsKit_queue_exec(m_gsGlobal);
		gsKit_clear(m_gsGlobal, GS_SETREG_RGBAQ(0x00,0x00,0x00,0x80,0x00));
		gsKit_sync_flip(m_gsGlobal);
		gsKit_queue_exec(m_gsGlobal);

		m_textureStartPointer = m_gsGlobal->CurrentPointer;

		m_bInitDone = true;
	}
	return true;
}

void CGUIFrameRendererPS2::clearVram()
{
	if (!m_bInitDone) return;
	m_gsGlobal->CurrentPointer = m_textureStartPointer;

	std::list<CGUIFrameTexturePS2*>::iterator k;

	for (k = CGUIFrameTexturePS2::m_tex_list.begin(); k != CGUIFrameTexturePS2::m_tex_list.end(); k++)
	{
		(*k)->setVramStatus(false);
	}
}

void CGUIFrameRendererPS2::uploadTexInline(CGUIFrameTexturePS2 *texture)
{
	if (!m_bInitDone) return;
	u32 size = 0, sizecl = 0;
	GSTEXTURE *tmpTex = texture->getTexPointer();
	
	size = gsKit_texture_size(tmpTex->Width, tmpTex->Height, tmpTex->PSM);
	size = (-GS_VRAM_BLOCKSIZE_256)&(size+GS_VRAM_BLOCKSIZE_256-1);

	if (tmpTex->PSM == GS_PSM_T8 || tmpTex->PSM == GS_PSM_T4)
	{
		sizecl = gsKit_texture_size((tmpTex->PSM == GS_PSM_T8 ? 16 : 8), (tmpTex->PSM == GS_PSM_T8 ? 16 : 2), tmpTex->ClutPSM);
		sizecl = (-GS_VRAM_BLOCKSIZE_256)&(sizecl+GS_VRAM_BLOCKSIZE_256-1);
	}

	if((m_gsGlobal->CurrentPointer + size + sizecl)>= 4194304)
	{
		clearVram();
	}

	tmpTex->Vram = gsKit_vram_alloc(m_gsGlobal, gsKit_texture_size(tmpTex->Width, tmpTex->Height, tmpTex->PSM), GSKIT_ALLOC_USERBUFFER);
	
	if (tmpTex->PSM == GS_PSM_T8 || tmpTex->PSM == GS_PSM_T4)
	{
		tmpTex->VramClut = gsKit_vram_alloc(m_gsGlobal, gsKit_texture_size((tmpTex->PSM == GS_PSM_T8 ? 16 : 8), (tmpTex->PSM == GS_PSM_T8 ? 16 : 2), tmpTex->ClutPSM), GSKIT_ALLOC_USERBUFFER);
		gsKit_setup_tbw(tmpTex);
		gsKit_texture_send_inline(m_gsGlobal, tmpTex->Mem, tmpTex->Width, tmpTex->Height, tmpTex->Vram, tmpTex->PSM, tmpTex->TBW, GS_CLUT_TEXTURE);
		gsKit_texture_send_inline(m_gsGlobal, tmpTex->Clut, (tmpTex->PSM == GS_PSM_T8 ? 16 : 8), (tmpTex->PSM == GS_PSM_T8 ? 16 : 2), tmpTex->VramClut, tmpTex->ClutPSM, 1, GS_CLUT_PALLETE);
	} else
	{
		gsKit_setup_tbw(tmpTex);
		gsKit_texture_send_inline(m_gsGlobal, tmpTex->Mem, tmpTex->Width, tmpTex->Height, tmpTex->Vram, tmpTex->PSM, tmpTex->TBW, GS_CLUT_NONE);
	}
	texture->setVramStatus(true);
}

bool CGUIFrameRendererPS2::checkVram(CGUIFrameTexturePS2 *texture)
{
	if (!m_bInitDone) return false;
	if (!texture->getLoadedStatus()) return false;
	if (!texture->getVramStatus())
	{
		uploadTexInline(texture);
	}
	return true;
}

void CGUIFrameRendererPS2::setAlpha(bool enable)
{
	if (!m_bInitDone) return;
	if (enable)
	{
		m_gsGlobal->PrimAlphaEnable = GS_SETTING_ON;
		gsKit_set_primalpha(m_gsGlobal, GS_SETREG_ALPHA(0,1,0,1,0), 0);
	} else
	{
		m_gsGlobal->PrimAlphaEnable = GS_SETTING_OFF;
		gsKit_set_primalpha(m_gsGlobal, GS_BLEND_BACK2FRONT, 0);
	}
}
void CGUIFrameRendererPS2::setTestAlpha(bool enable)
{
	if (!m_bInitDone) return;
	if (enable)
	{
		gsKit_set_test(m_gsGlobal, GS_ATEST_ON);
	} else
	{
		gsKit_set_test(m_gsGlobal, GS_ATEST_OFF);
	}
}

void CGUIFrameRendererPS2::setScissor(bool enable, int x1, int y1, int x2, int y2)
{
	if (!m_bInitDone) return;
	if (enable)
	{
		if (x1 < 0) x1 = 0;
		if (x2 < 0) x2 = 0;
		if (y1 < 0) y1 = 0;
		if (y2 < 0) y2 = 0;
		if (x1 > m_vwidth) x1 = m_vwidth;
		if (x2 > m_vwidth) x2 = m_vwidth;
		if (y1 > m_vheight) y1 = m_vheight;
		if (y2 > m_vheight) y2 = m_vheight;

		int cx1 = (int)(((float)x1*m_xscale)+m_xmove+0.5f);
		int cx2 = (int)(((float)x2*m_xscale)+m_xmove);
		int cy1 = (int)(((float)y1*m_yscale)+m_ymove+0.5f);
		int cy2 = (int)(((float)y2*m_yscale)+m_ymove);

		if (cx1 < 0) cx1 = 0;
		if (cx2 < 0) cx2 = 0;
		if (cy1 < 0) cy1 = 0;
		if (cy2 < 0) cy2 = 0;
		if (cx1 > m_gsGlobal->Width) cx1 = m_gsGlobal->Width;
		if (cx2 > m_gsGlobal->Width) cx2 = m_gsGlobal->Width;
		if (cy1 > m_gsGlobal->Height) cy1 = m_gsGlobal->Height;
		if (cy2 > m_gsGlobal->Height) cy2 = m_gsGlobal->Height;

		gsKit_set_scissor(m_gsGlobal, GS_SETREG_SCISSOR(cx1, cx2, cy1, cy2));
	} else
	{
		int ymove = (int)(m_ymove+0.5f);
		if (ymove < 0) ymove = 0;
		gsKit_set_scissor(m_gsGlobal, GS_SETREG_SCISSOR(0, m_gsGlobal->Width, ymove, m_gsGlobal->Height-ymove));
	}
}
void CGUIFrameRendererPS2::swapBuffers()
{
	if (!m_bInitDone) return;
	gsKit_sync_flip(m_gsGlobal);
	gsKit_queue_exec(m_gsGlobal);
	gsKit_clear(m_gsGlobal, GS_SETREG_RGBAQ(0x00,0x00,0x00,0x80,0x00));
}

void CGUIFrameRendererPS2::clearFrontBuffer()
{
	gsKit_set_scissor(m_gsGlobal, GS_SETREG_SCISSOR(0, m_gsGlobal->Width, 0, m_gsGlobal->Height));
	gsKit_clear(m_gsGlobal, GS_SETREG_RGBAQ(0x00,0x00,0x00,0x80,0x00));
	setScissor(false);
}

void CGUIFrameRendererPS2::drawLineF(const float x1, const float y1, const float x2, const float y2, const u8 r, const u8 g, const u8 b, float alpha)
{
	if (!m_bInitDone) return;
	gsKit_prim_line(
		m_gsGlobal,
		((float)x1*m_xscale)+m_xmove, ((float)y1*m_yscale)+m_ymove,
		((float)x2*m_xscale)+m_xmove, ((float)y2*m_yscale)+m_ymove,
		0, 
		GS_SETREG_RGBAQ(r,g,b,(u32)(((float)0x80*alpha)+0.5f),0x00)
	);
}

void CGUIFrameRendererPS2::drawLineG(const float x1, const float y1, const float x2, const float y2, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, float alpha1, float alpha2)
{
	if (!m_bInitDone) return;
	gsKit_prim_line_goraud(
		m_gsGlobal,
		((float)x1*m_xscale)+m_xmove, ((float)y1*m_yscale)+m_ymove,
		((float)x2*m_xscale)+m_xmove, ((float)y2*m_yscale)+m_ymove,
		0,
		GS_SETREG_RGBAQ(r1,g1,b1,(u32)(((float)0x80*alpha1)+0.5f),0x00),
		GS_SETREG_RGBAQ(r2,g2,b2,(u32)(((float)0x80*alpha2)+0.5f),0x00)
	);
}

void CGUIFrameRendererPS2::drawTriangleF(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const u8 r, const u8 g, const u8 b, float alpha)
{
	if (!m_bInitDone) return;
	gsKit_prim_triangle(
		m_gsGlobal,
		((float)x1*m_xscale)+m_xmove, ((float)y1*m_yscale)+m_ymove,
		((float)x2*m_xscale)+m_xmove, ((float)y2*m_yscale)+m_ymove,
		((float)x3*m_xscale)+m_xmove, ((float)y3*m_yscale)+m_ymove,
		0,
		GS_SETREG_RGBAQ(r,g,b,(u32)(((float)0x80*alpha)+0.5f),0x00)
	);
}

void CGUIFrameRendererPS2::drawTriangleG(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, float alpha1, float alpha2, float alpha3)
{
	if (!m_bInitDone) return;
	gsKit_prim_triangle_gouraud(
		m_gsGlobal,
		((float)x1*m_xscale)+m_xmove, ((float)y1*m_yscale)+m_ymove,
		((float)x2*m_xscale)+m_xmove, ((float)y2*m_yscale)+m_ymove,
		((float)x3*m_xscale)+m_xmove, ((float)y3*m_yscale)+m_ymove,
		0,
		GS_SETREG_RGBAQ(r1,g1,b1,(u32)(((float)0x80*alpha1)+0.5f),0x00),
		GS_SETREG_RGBAQ(r2,g2,b2,(u32)(((float)0x80*alpha2)+0.5f),0x00),
		GS_SETREG_RGBAQ(r3,g3,b3,(u32)(((float)0x80*alpha3)+0.5f),0x00)
	);
}

void CGUIFrameRendererPS2::drawQuadF(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float x4, const float y4, const u8 r, const u8 g, const u8 b, float alpha)
{
	if (!m_bInitDone) return;
	gsKit_prim_quad(
		m_gsGlobal,
		((float)x1*m_xscale)+m_xmove, ((float)y1*m_yscale)+m_ymove,
		((float)x2*m_xscale)+m_xmove, ((float)y2*m_yscale)+m_ymove,
		((float)x3*m_xscale)+m_xmove, ((float)y3*m_yscale)+m_ymove,
		((float)x4*m_xscale)+m_xmove, ((float)y4*m_yscale)+m_ymove,
		0,
		GS_SETREG_RGBAQ(r,g,b,(u32)(((float)0x80*alpha)+0.5f),0x00)
	);
}

void CGUIFrameRendererPS2::drawQuadG(const float x1, const float y1, const float x2, const float y2, const float x3, const float y3, const float x4, const float y4, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, const u8 r4, const u8 g4, const u8 b4, float alpha1, float alpha2, float alpha3, float alpha4)
{
	if (!m_bInitDone) return;

	gsKit_prim_quad_gouraud(
		m_gsGlobal,
		((float)x1*m_xscale)+m_xmove, ((float)y1*m_yscale)+m_ymove,
		((float)x2*m_xscale)+m_xmove, ((float)y2*m_yscale)+m_ymove,
		((float)x3*m_xscale)+m_xmove, ((float)y3*m_yscale)+m_ymove,
		((float)x4*m_xscale)+m_xmove, ((float)y4*m_yscale)+m_ymove,
		0,
		GS_SETREG_RGBAQ(r1,g1,b1,(u32)(((float)0x80*alpha1)+0.5f),0x00),
		GS_SETREG_RGBAQ(r2,g2,b2,(u32)(((float)0x80*alpha2)+0.5f),0x00),
		GS_SETREG_RGBAQ(r3,g3,b3,(u32)(((float)0x80*alpha3)+0.5f),0x00),
		GS_SETREG_RGBAQ(r4,g4,b4,(u32)(((float)0x80*alpha4)+0.5f),0x00)
	);
}

void CGUIFrameRendererPS2::drawTriangleFT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const u8 r, const u8 g, const u8 b, float alpha)
{
	if (!m_bInitDone) return;
	if (!checkVram((CGUIFrameTexturePS2*)texture)) return;
	gsKit_prim_triangle_texture(
		m_gsGlobal,
		((CGUIFrameTexturePS2*)texture)->getTexPointer(),
		((float)x1*m_xscale)+m_xmove, ((float)y1*m_yscale)+m_ymove,
		u1, v1,
		((float)x2*m_xscale)+m_xmove, ((float)y2*m_yscale)+m_ymove,
		u2, v2,
		((float)x3*m_xscale)+m_xmove, ((float)y3*m_yscale)+m_ymove,
		u3, v3,
		0,
		GS_SETREG_RGBAQ(r,g,b,(u32)(((float)0x80*alpha)+0.5f),0x00)
	);
}

void CGUIFrameRendererPS2::drawTriangleGT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, float alpha1, float alpha2, float alpha3)
{
	if (!m_bInitDone) return;
	if (!checkVram((CGUIFrameTexturePS2*)texture)) return;
	gsKit_prim_triangle_goraud_texture(
		m_gsGlobal,
		((CGUIFrameTexturePS2*)texture)->getTexPointer(),
		((float)x1*m_xscale)+m_xmove, ((float)y1*m_yscale)+m_ymove,
		u1, v1,
		((float)x2*m_xscale)+m_xmove, ((float)y2*m_yscale)+m_ymove,
		u2, v2,
		((float)x3*m_xscale)+m_xmove, ((float)y3*m_yscale)+m_ymove,
		u3, v3,
		0,
		GS_SETREG_RGBAQ(r1,g1,b1,(u32)(((float)0x80*alpha1)+0.5f),0x00),
		GS_SETREG_RGBAQ(r2,g2,b2,(u32)(((float)0x80*alpha2)+0.5f),0x00),
		GS_SETREG_RGBAQ(r3,g3,b3,(u32)(((float)0x80*alpha3)+0.5f),0x00)
	);
}

void CGUIFrameRendererPS2::drawQuadFT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const float x4, const float y4, const float u4, const float v4, const u8 r, const u8 g, const u8 b, float alpha)
{
	if (!m_bInitDone) return;
	if (!checkVram((CGUIFrameTexturePS2*)texture)) return;
	gsKit_prim_quad_texture(
		m_gsGlobal,
		((CGUIFrameTexturePS2*)texture)->getTexPointer(),
		((float)x1*m_xscale)+m_xmove, ((float)y1*m_yscale)+m_ymove,
		u1, v1,
		((float)x2*m_xscale)+m_xmove, ((float)y2*m_yscale)+m_ymove,
		u2, v2,
		((float)x3*m_xscale)+m_xmove, ((float)y3*m_yscale)+m_ymove,
		u3, v3,
		((float)x4*m_xscale)+m_xmove, ((float)y4*m_yscale)+m_ymove,
		u4, v4,
		0,
		GS_SETREG_RGBAQ(r,g,b,(u32)(((float)0x80*alpha)+0.5f),0x00)
	);
}

void CGUIFrameRendererPS2::drawQuadGT(CIGUIFrameTexture *texture, const float x1, const float y1, const float u1, const float v1, const float x2, const float y2, const float u2, const float v2, const float x3, const float y3, const float u3, const float v3, const float x4, const float y4, const float u4, const float v4, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, const u8 r4, const u8 g4, const u8 b4, float alpha1, float alpha2, float alpha3, float alpha4)
{
	if (!m_bInitDone) return;
	if (!checkVram((CGUIFrameTexturePS2*)texture)) return;
	gsKit_prim_quad_goraud_texture(
		m_gsGlobal,
		((CGUIFrameTexturePS2*)texture)->getTexPointer(),
		((float)x1*m_xscale)+m_xmove, ((float)y1*m_yscale)+m_ymove,
		u1, v1,
		((float)x2*m_xscale)+m_xmove, ((float)y2*m_yscale)+m_ymove,
		u2, v2,
		((float)x3*m_xscale)+m_xmove, ((float)y3*m_yscale)+m_ymove,
		u3, v3,
		((float)x4*m_xscale)+m_xmove, ((float)y4*m_yscale)+m_ymove,
		u4, v4,
		0,
		GS_SETREG_RGBAQ(r1,g1,b1,(u32)(((float)0x80*alpha1)+0.5f),0x00),
		GS_SETREG_RGBAQ(r2,g2,b2,(u32)(((float)0x80*alpha2)+0.5f),0x00),
		GS_SETREG_RGBAQ(r3,g3,b3,(u32)(((float)0x80*alpha3)+0.5f),0x00),
		GS_SETREG_RGBAQ(r4,g4,b4,(u32)(((float)0x80*alpha4)+0.5f),0x00)
	);
}

#define CENTER_XY2 0.0625f
#define CENTER_UV1 0.5f
#define CENTER_UV2 0.375f

void CGUIFrameRendererPS2::drawSpriteT(CIGUIFrameTexture *texture, const float x, const float y, const float w, const float h, const float u1, const float v1, const float u2, const float v2, const u8 r, const u8 g, const u8 b, float alpha)
{
	if (!m_bInitDone) return;
	if (!checkVram((CGUIFrameTexturePS2*)texture)) return;
	gsKit_prim_sprite_texture(
		m_gsGlobal,
		((CGUIFrameTexturePS2*)texture)->getTexPointer(),
		((float)x*m_xscale)+m_xmove, ((float)y*m_yscale)+m_ymove,
		u1+CENTER_UV1, v1+CENTER_UV1,
		((float)x*m_xscale)+m_xmove+((float)w*m_xscale)-1.0f+CENTER_XY2, ((float)y*m_yscale)+m_ymove+((float)h*m_yscale)-1.0f+CENTER_XY2,
		u2-CENTER_UV2, v2-CENTER_UV2,
		0,
		GS_SETREG_RGBAQ(r,g,b,(u32)(((float)0x80*alpha)+0.5f),0x00)
	);
}

void CGUIFrameRendererPS2::drawSpriteGT(CIGUIFrameTexture *texture, const float x, const float y, const float w, const float h, const float u1, const float v1, const float u2, const float v2, const u8 r1, const u8 g1, const u8 b1, const u8 r2, const u8 g2, const u8 b2, const u8 r3, const u8 g3, const u8 b3, const u8 r4, const u8 g4, const u8 b4, float alpha1, float alpha2, float alpha3, float alpha4)
{
	if (!m_bInitDone) return;
	if (!checkVram((CGUIFrameTexturePS2*)texture)) return;
	gsKit_prim_quad_goraud_texture(
		m_gsGlobal,
		((CGUIFrameTexturePS2*)texture)->getTexPointer(),
		((float)x*m_xscale)+m_xmove, ((float)y*m_yscale)+m_ymove,
		u1 +CENTER_UV1, v1 +CENTER_UV1,
		((float)x*m_xscale)+m_xmove+((float)w*m_xscale) -1.0f+CENTER_XY2, ((float)y*m_yscale)+m_ymove,
		u2 -CENTER_UV2, v1 +CENTER_UV1,
		((float)x*m_xscale)+m_xmove, ((float)y*m_yscale)+m_ymove+((float)h*m_yscale) -1.0f+CENTER_XY2,
		u1 +CENTER_UV1, v2 -CENTER_UV2,
		((float)x*m_xscale)+m_xmove+((float)w*m_xscale) -1.0f+CENTER_XY2, ((float)y*m_yscale)+m_ymove+((float)h*m_yscale) -1.0f+CENTER_XY2,
		u2 -CENTER_UV2, v2 -CENTER_UV2,
		0,
		GS_SETREG_RGBAQ(r1,g1,b1,(u32)(((float)0x80*alpha1)+0.5f),0x00),
		GS_SETREG_RGBAQ(r2,g2,b2,(u32)(((float)0x80*alpha2)+0.5f),0x00),
		GS_SETREG_RGBAQ(r3,g3,b3,(u32)(((float)0x80*alpha3)+0.5f),0x00),
		GS_SETREG_RGBAQ(r4,g4,b4,(u32)(((float)0x80*alpha4)+0.5f),0x00)
	);
}

#define EE_D1_CHCR       ((volatile u32 *) 0x10009000)
#define EE_D1_MADR       ((volatile u32 *) 0x10009010)
#define EE_D1_QWC        ((volatile u32 *) 0x10009020)
#define EE_D1_TADR       ((volatile u32 *) 0x10009030)
#define EE_D1_ASR0       ((volatile u32 *) 0x10009040)
#define EE_D1_ASR1       ((volatile u32 *) 0x10009050)
#define EE_VIF1_STAT     ((volatile u32 *) 0x10003c00)
#define DPUT_EE_VIF1_FIFO(value)         (*(volatile u128 *)0x10005000 = value)
#define DPUT_GS_CSR(value)               (*(volatile  u64 *)0x12001000 = value)
#define _GS_CSR               (*(volatile  u64 *)0x12001000)

void CGUIFrameRendererPS2::downloadVram(void *virtPage, int physPage, int addr, int bufw, int x, int y, int w, int h, int psm)
{
	u32 * pDmaMem = (u32*)virtPage;//<-- point to sps2 allocated unswapable memory.

	u64 *pDmaMem64;
	void * pPhysDmaMem;
	u32 prev_chcr;
	u64 prev_imr;
	static u32 enable_path3[4] __attribute__((aligned (128))) = {
												0x06000000, //MSKPATH3(0),
												0,
												0,
												0};

	u32 uSrcPtr = addr;
	u32 sizebytes;
	switch (m_gsGlobal->PSM)
	{
		case GS_PSM_CT16:
		case GS_PSM_CT16S:
			sizebytes = 2;
			break;
		case GS_PSM_CT24:
			sizebytes = 3;
			break;
		default:
			sizebytes = 4;
	}

	u32 uQSize = (w*h*sizebytes)/16; // assuming 24/32bit framebuffer

	// Setup transfer texture back to memory
	pDmaMem[0] = 0; //NOP;
	pDmaMem[1] = 0x06008000; //MSKPATH3(0x8000);
	pDmaMem[2] = 0x13000000; //FLUSHA;
	pDmaMem[3] = 0x50000006; //DIRECT(6);*/

	pPhysDmaMem = (void*)physPage;

	// Add GifTag Data
	pDmaMem64 = (u64 *) (pDmaMem + 4);
	pDmaMem64[0] = GIF_TAG(5, 1, 0, 0, 0, 1); // GIFTAG(NLOOP, EOP, PRE, PRIM, FLG, NREG)
	pDmaMem64[1] = GIF_AD;

	pDmaMem64[2] = GS_SETREG_BITBLTBUF( uSrcPtr, bufw, psm, 0, 0, 0 );
	pDmaMem64[3] = GS_BITBLTBUF;

	pDmaMem64[4] = GS_SETREG_TRXPOS(x, y, 0, 0, 0); // SSAX, SSAY, DSAX, DSAY, DIR
	pDmaMem64[5] = GS_TRXPOS;

	pDmaMem64[6] = GS_SETREG_TRXREG(w, h); // RRW, RRh
	pDmaMem64[7] = GS_TRXREG;

	pDmaMem64[8] = 0;
	pDmaMem64[9] = GS_FINISH;

	pDmaMem64[10] = GS_SETREG_TRXDIR(1); // XDIR
	pDmaMem64[11] = GS_TRXDIR;

	prev_imr = GsPutIMR( GsGetIMR() | 0x0200 );
	prev_chcr = *EE_D1_CHCR;

	// vif1 must be available
	if ((*EE_D1_CHCR & 0x0100) != 0)
	{
		printf("There was a problem with VIF1!\n");
	}	

	DPUT_GS_CSR(2);
	// Disable interrupts so we don't lose our finish event
	asm __volatile__ ("di");

	// DMA from memory and start DMA transfer
	FlushCache(0);

	*EE_D1_QWC = 0x7;
	*EE_D1_MADR = ( unsigned int ) pPhysDmaMem & 0x0FFFFFFF ;
	*EE_D1_CHCR = (1<<0) | (0<<2) | (0<<4) | (0<<6) | (0<<7) | (1<<8);

	__asm __volatile__( " sync.l " );
	// check if DMA is complete (STR=0)
	while ( *EE_D1_CHCR & 0x0100 );
	while( ( _GS_CSR & 2 ) == 0 );

	asm __volatile__ ("ei"); 

	// wait for VIF1-FIFO to empty and become idle
	while ( *EE_VIF1_STAT & 0x1f000003 );
	//((tVIF1_STAT*)VIF1_STAT)->FDR = 1;
	*EE_VIF1_STAT = (1<<23);
	GS_SETREG_BUSDIR( ( u64 ) 0x00000001 );
	//FlushCache(0);
	*EE_D1_QWC = uQSize;
	*EE_D1_MADR = ( u32 ) physPage & 0x0FFFFFFF ;
	*EE_D1_CHCR = 0 | (0<<2) | (0<<4) | (0<<6) | (0<<7) | (1<<8);

	__asm __volatile__( " sync.l " );

	// check if DMA is complete (STR=0)
	while ( *EE_D1_CHCR & 0x0100 );
	*EE_D1_CHCR = prev_chcr;
	__asm __volatile__( " sync.l " );

	// wait for VIF1-FIFO to empty and become idle
	while ( *EE_VIF1_STAT & 0x1f000003 );
	//((tVIF1_STAT*)VIF1_STAT)->FDR = 0;
	*EE_VIF1_STAT = 0;
	GS_SETREG_BUSDIR( ( u64 ) 0 );

	// restore the setting of IMR
   GsPutIMR( prev_imr);
   // set the FINISH event to default
   //dput_gs_csr( GS_EE_CSR_FINISH_M );
   DPUT_GS_CSR( 0 ); 

	DPUT_EE_VIF1_FIFO( *( u128 * ) enable_path3 );
}
void CGUIFrameRendererPS2::screenshot(u8 *buff)
{
	dmaKit_wait_fast();
	downloadVram(buff, (u32)buff, m_gsGlobal->ScreenBuffer[m_gsGlobal->ActiveBuffer]/256, m_gsGlobal->Width/64, 0, 0, m_gsGlobal->Width, m_gsGlobal->Height, m_gsGlobal->PSM);
}

CIGUIFrameTexture *CGUIFrameRendererPS2::getFrameTex(int divfac)
{
	CGUIFrameTexturePS2 *fbTex = new CGUIFrameTexturePS2;
	int bytes;

	switch (m_gsGlobal->PSM)
	{
		case GS_PSM_CT32:
			bytes = 4;
			break;
		case GS_PSM_CT24:
			bytes = 3;
			break;
		default:
			bytes = 2;
	}
	int tmpSize = m_gsGlobal->Width*m_gsGlobal->Height*bytes;
	u32 size128 = ((tmpSize+15)>>4);
	u128 *texbuf = new u128[size128];
	u8 *tmpBuff = (u8*)texbuf;
	screenshot(tmpBuff);

	if (divfac > 0 && bytes == 3)
	{
		u32 newSize128 = size128>>divfac;
		u128 *newtexbuf = new u128[newSize128];
		u8 *newtmpBuff = (u8*)newtexbuf;
		int w = m_gsGlobal->Width>>divfac;
		int h = m_gsGlobal->Height>>divfac;
		int bw = m_gsGlobal->Width;
		int bh = m_gsGlobal->Height;
		int newtmpSize = w*h*3;
		int oldlinesize = bw*3;

		if (divfac == 1)
		{
			int oldlpos = 0;
			int newlpos = 0;
			for (int y = 0; y < h; y++)
			{
				for (int x = 0; x < w; x++)
				{
					u32 sumr = 0;
					u32 sumg = 0;
					u32 sumb = 0;

					u8 *tmpBufpos = tmpBuff+oldlpos;

					sumr += tmpBufpos[0];
					sumr += tmpBufpos[3];

					sumg += tmpBufpos[1];
					sumg += tmpBufpos[4];

					sumb += tmpBufpos[2];
					sumb += tmpBufpos[5];

					tmpBufpos += oldlinesize;

					sumr += tmpBufpos[0];
					sumr += tmpBufpos[3];

					sumg += tmpBufpos[1];
					sumg += tmpBufpos[4];

					sumb += tmpBufpos[2];
					sumb += tmpBufpos[5];

					u8 *newtmpBufpos = newtmpBuff+newlpos;
					newtmpBufpos[0] = sumr>>2;
					newtmpBufpos[1] = sumg>>2;
					newtmpBufpos[2] = sumb>>2;

					newlpos += 3;
					oldlpos += 6;
				}
				oldlpos += oldlinesize;
			}
		} else
		{
			for (int y = 0; y < bh; y+=1<<divfac)
			{
				for (int x = 0; x < bw; x+=1<<divfac)
				{
					u32 sumr = 0;
					u32 sumg = 0;
					u32 sumb = 0;
					for (int yi = 0; yi < 1<<divfac; yi++)
					{
						for (int xi = 0; xi < 1<<divfac; xi++)
						{
							int pos = (((y+yi)*bw)+x+xi)*bytes;
							sumr += tmpBuff[pos +0];
							sumg += tmpBuff[pos +1];
							sumb += tmpBuff[pos +2];
						}
					}
					int newpos = ((y>>divfac)*w +(x>>divfac))*bytes;
					int shiftnum = divfac<<1;
					newtmpBuff[newpos +0] = sumr>>shiftnum;
					newtmpBuff[newpos +1] = sumg>>shiftnum;
					newtmpBuff[newpos +2] = sumb>>shiftnum;
				}
			}
		}
		fbTex->loadTextureBufferRaw(m_gsGlobal->PSM, newtmpBuff, w, h, newtmpSize);
		fbTex->setTexfilter(CIGUIFrameTexture::etFiltBilinear);
		delete [] newtexbuf;
	} else
	{
		fbTex->loadTextureBufferRaw(m_gsGlobal->PSM, tmpBuff, m_gsGlobal->Width, m_gsGlobal->Height, tmpSize);
	}

	delete [] texbuf;
	return fbTex;
}

void CGUIFrameRendererPS2::restoreFrameTex(CIGUIFrameTexture *tex)
{
	bool lastAlpha = (m_gsGlobal->PrimAlphaEnable == GS_SETTING_ON ? true : false);
	setAlpha(false);
	if (!m_bInitDone) return;
	if (!checkVram((CGUIFrameTexturePS2*)tex)) return;
	gsKit_prim_sprite_texture(
		m_gsGlobal,
		((CGUIFrameTexturePS2*)tex)->getTexPointer(),
		0, 0,
		0, 0,
		m_gsGlobal->Width-1, m_gsGlobal->Height-1,
		tex->getWidth(), tex->getHeight(),
		0,
		GS_SETREG_RGBAQ(128,128,128,128,0)
	);
	setAlpha(lastAlpha);
}

extern "C" {
#include <gsInline.h>
#define GS_SCISSOR_RESET 0x00 /// Resets SCISSOR values to the entire display bounds
	void gsKit_set_scissor(GSGLOBAL *gsGlobal, u64 ScissorBounds) {
	    u64 *p_data;
		u64 *p_store;

		if (ScissorBounds == GS_SCISSOR_RESET)
			ScissorBounds = GS_SETREG_SCISSOR(0, gsGlobal->Width - 1, 0, gsGlobal->Height - 1);

		p_data = p_store = (u64*)gsKit_heap_alloc(gsGlobal, 1, 16, GIF_AD);

		*p_data++ = GIF_TAG_AD(1);
		*p_data++ = GIF_AD;

		*p_data++ = ScissorBounds;
		*p_data++ = GS_SCISSOR_1+gsGlobal->PrimContext;
	}
}
