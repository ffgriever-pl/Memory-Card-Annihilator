#include "GUIMcaPopup.h"
#include "res/resources.h"


CGUIMcaPopup::CGUIMcaPopup(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y)
	: CGUIMcaBaseWindow(renderer, input, timer, x, y)
{
	CResources::m_popup_pal.loadTextureBuffer(CResources::popup_pal_tm2, CResources::size_popup_pal_tm2, true);
	CResources::m_popup_pal_info.loadTextureBuffer(CResources::popup_pal_info_tm2, CResources::size_popup_pal_info_tm2, true);
	CResources::m_icon_exclam.loadTextureBuffer(CResources::icon_exclam_tm2, CResources::size_icon_exclam_tm2, true);
	CResources::m_icon_fail.loadTextureBuffer(CResources::icon_fail_tm2, CResources::size_icon_fail_tm2, true);
	CResources::m_icon_success.loadTextureBuffer(CResources::icon_success_tm2, CResources::size_icon_success_tm2, true);
}

CGUIMcaPopup::~CGUIMcaPopup(void)
{
}

void CGUIMcaPopup::drawExclam(float alpha)
{
	m_renderer->drawSpriteT(
		&CResources::m_icon_exclam
		, m_x +116, m_y +20
		, CResources::m_icon_exclam.getWidth(), CResources::m_icon_exclam.getHeight()
		, 0, 0
		, CResources::m_icon_exclam.getWidth(), CResources::m_icon_exclam.getHeight()
		, 128, 128, 128, alpha
	);
}

void CGUIMcaPopup::drawFail(float alpha)
{
	m_renderer->drawSpriteT(
		&CResources::m_icon_fail
		, m_x +116, m_y +20
		, CResources::m_icon_fail.getWidth(), CResources::m_icon_fail.getHeight()
		, 0, 0
		, CResources::m_icon_fail.getWidth(), CResources::m_icon_fail.getHeight()
		, 128, 128, 128, alpha
	);
}

void CGUIMcaPopup::drawSuccess(float alpha)
{
	m_renderer->drawSpriteT(
		&CResources::m_icon_success
		, m_x +116, m_y +20
		, CResources::m_icon_success.getWidth(), CResources::m_icon_success.getHeight()
		, 0, 0
		, CResources::m_icon_success.getWidth(), CResources::m_icon_success.getHeight()
		, 128, 128, 128, alpha
	);
}

void CGUIMcaPopup::drawWindow(float alpha)
{
	m_renderer->drawSpriteT(
		&CResources::m_popup_pal
		, m_x, m_y
		, CResources::m_popup_pal.getWidth(), CResources::m_popup_pal.getHeight()
		, 0, 0
		, CResources::m_popup_pal.getWidth(), CResources::m_popup_pal.getHeight()
		, 128, 128, 128, alpha*0.90f
	);
}

void CGUIMcaPopup::drawWindowInfo(float alpha)
{
	m_renderer->drawSpriteT(
		&CResources::m_popup_pal_info
		, m_x, m_y
		, CResources::m_popup_pal_info.getWidth(), CResources::m_popup_pal_info.getHeight()
		, 0, 0
		, CResources::m_popup_pal_info.getWidth(), CResources::m_popup_pal_info.getHeight()
		, 128, 128, 128, alpha*0.90f
	);
}
