#include "GUIMcaProgressBar.h"


CGUIMcaProgressBar::CGUIMcaProgressBar(CIGUIFrameRenderer* renderer, float x, float y)
	: m_x(x), m_y(y)
	, m_progress(0.0f)
	, m_r1(255), m_g1(133), m_b1(112)
	, m_r2(244), m_g2(116), m_b2(94)
	, m_r3(207), m_g3(31), m_b3(0)
	, m_r4(149), m_g4(22), m_b4(0)
	, m_state(ensPending)
	, m_renderer(renderer)
{
	CResources::m_bar_red.loadTextureBuffer(CResources::progress_red_tm2, CResources::size_progress_red_tm2, true);
	CResources::m_bar_green.loadTextureBuffer(CResources::progress_green_tm2, CResources::size_progress_green_tm2, true);
	CResources::m_bar_black.loadTextureBuffer(CResources::progress_black_tm2, CResources::size_progress_black_tm2, true);

	CResources::m_bar_lead_green.loadTextureBuffer(CResources::progress_lead_green_tm2, CResources::size_progress_lead_green_tm2, true);
	CResources::m_bar_lead_red.loadTextureBuffer(CResources::progress_lead_red_tm2, CResources::size_progress_lead_red_tm2, true);
}


CGUIMcaProgressBar::~CGUIMcaProgressBar(void)
{
}

void CGUIMcaProgressBar::setState(enStateProgress state)
{
	m_state = state;
}

void CGUIMcaProgressBar::display(float alpha)
{
	float shadowpos = 20.0f;
	m_renderer->drawSpriteT(
		&CResources::m_bar_black
		, m_x, m_y
		, CResources::m_bar_black.getWidth(), CResources::m_bar_black.getHeight()
		, 0, 0
		, CResources::m_bar_black.getWidth(), CResources::m_bar_black.getHeight()
		, 128, 128, 128, alpha*0.18f
	);

	m_renderer->drawQuadGT(
		&CResources::m_bar_black
		, m_x, m_y+shadowpos
		, 0.5f, CResources::m_bar_black.getHeight()-0.5f
		, m_x+CResources::m_bar_black.getWidth() -1.0f+0.0625f, m_y+shadowpos
		, CResources::m_bar_black.getWidth(), CResources::m_bar_black.getHeight()-0.5f
		, m_x, m_y+CResources::m_bar_black.getHeight() -1.0f+0.0625f+shadowpos
		, 0.5f, 0.0f
		, m_x+CResources::m_bar_black.getWidth() -1.0f+0.0625f, m_y+CResources::m_bar_black.getHeight() -1.0f+0.0625f+shadowpos
		, CResources::m_bar_black.getWidth(), 0.0f
		, 128, 128, 128
		, 128, 128, 128
		, 128, 128, 128
		, 128, 128, 128
		, alpha*0.10f
		, alpha*0.10f
		, alpha*0.00f
		, alpha*0.00f
	);
	if (m_progress > 0.0f)
	{
		CGUITexture *state_bar = NULL;
		CGUITexture *state_lead = NULL;
		switch (m_state)
		{
			case ensFail:
				state_bar = &CResources::m_bar_black;
				break;
			case ensSuccess:
				state_bar = &CResources::m_bar_green;
				state_lead = &CResources::m_bar_lead_green;
				break;
			default:
				state_bar = &CResources::m_bar_red;
				state_lead = &CResources::m_bar_lead_red;
				break;
		}
		if (state_lead)
		{
			if (362.0f*m_progress >= 3.0f && 362.0f*m_progress <= 361.0f)
			{
				m_renderer->drawSpriteT(
					state_lead
					, m_x+3.0f -4.0f +362.0f*m_progress, m_y
					, state_lead->getWidth(), state_lead->getHeight()
					, 0, 0
					, state_lead->getWidth(), state_lead->getHeight()
					, 128, 128, 128, alpha
				);

				m_renderer->drawQuadGT(
					state_lead
					, m_x+3.0f -4.0f +362.0f*m_progress, m_y+shadowpos
					, 0.5f, state_lead->getHeight()-0.5f
					, m_x+3.0f -4.0f +362.0f*m_progress+state_lead->getWidth() -1.0f+0.0625f, m_y+shadowpos
					, state_lead->getWidth(), state_lead->getHeight()-0.5f
					, m_x+3.0f -4.0f +362.0f*m_progress, m_y+state_lead->getHeight() -1.0f+0.0625f+shadowpos
					, 0.5f, 0.0f
					, m_x+3.0f -4.0f +362.0f*m_progress+state_lead->getWidth() -1.0f+0.0625f, m_y+state_lead->getHeight() -1.0f+0.0625f+shadowpos
					, state_lead->getWidth(), 0.0f
					, 128, 128, 128
					, 128, 128, 128
					, 128, 128, 128
					, 128, 128, 128
					, alpha*0.15f
					, alpha*0.15f
					, alpha*0.00f
					, alpha*0.00f
				);
			}
		}
		m_renderer->drawSpriteT(
			state_bar
			, m_x, m_y
			, 3.0f + 362.0f*m_progress, state_bar->getHeight()
			, 0, 0
			, 3.0f + 362.0f*m_progress, state_bar->getHeight()
			, 128, 128, 128, alpha
		);
		m_renderer->drawQuadGT(
			state_bar
			, m_x, m_y+shadowpos
			, 0.5f, state_bar->getHeight()-0.5f
			, m_x+3.0f + 362.0f*m_progress -1.0f+0.0625f, m_y+shadowpos
			, 3.0f + 362.0f*m_progress, state_bar->getHeight()-0.5f
			, m_x, m_y+state_bar->getHeight() -1.0f+0.0625f+shadowpos
			, 0.5f, 0.0f
			, m_x+3.0f + 362.0f*m_progress -1.0f+0.0625f, m_y+state_bar->getHeight() -1.0f+0.0625f+shadowpos
			, 3.0f + 362.0f*m_progress, 0.0f
			, 128, 128, 128
			, 128, 128, 128
			, 128, 128, 128
			, 128, 128, 128
			, alpha*0.15f
			, alpha*0.15f
			, alpha*0.00f
			, alpha*0.00f
		);
	}
}

void CGUIMcaProgressBar::setProgress(float progress)
{
	m_progress = progress;
	if (m_progress > 1.0f) m_progress = 1.0f;
}

void CGUIMcaProgressBar::setColor(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2, u8 r3, u8 g3, u8 b3, u8 r4, u8 g4, u8 b4)
{
	m_r1 = r1;
	m_g1 = g1;
	m_b1 = b1;
	m_r2 = r2;
	m_g2 = g2;
	m_b2 = b2;

	m_r3 = r3;
	m_g3 = g3;
	m_b3 = b3;
	m_r4 = r4;
	m_g4 = g4;
	m_b4 = b4;
}

