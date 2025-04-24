#ifndef _GUIMCAPROGRESSBAR_H_
#define _GUIMCAPROGRESSBAR_H_

#include "res/resources.h"
#include "GUITypes.h"
#include "IGUIFrameRenderer.h"

class CGUIMcaProgressBar
{
public:
	enum enStateProgress
	{
		ensPending,
		ensSuccess,
		ensFail,
	};
private:
	float m_x;
	float m_y;

	float m_progress; //0-1

	u8 m_r1; u8 m_g1; u8 m_b1;
	u8 m_r2; u8 m_g2; u8 m_b2;

	u8 m_r3; u8 m_g3; u8 m_b3;
	u8 m_r4; u8 m_g4; u8 m_b4;

	enStateProgress m_state;
	CIGUIFrameRenderer* m_renderer;
public:
	CGUIMcaProgressBar(CIGUIFrameRenderer* renderer, float x, float y);
	virtual ~CGUIMcaProgressBar(void);
	void display(float alpha = 1.0f);
	void setProgress(float progress);
	void setState(enStateProgress state);
	void setColor(u8 r1, u8 g1, u8 b1, u8 r2, u8 g2, u8 b2, u8 r3, u8 g3, u8 b3, u8 r4, u8 g4, u8 b4);
};

#endif //_GUIMCAPROGRESSBAR_H_
