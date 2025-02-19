#ifndef _IGUIFRAMEWINDOW_H_
#define _IGUIFRAMEWINDOW_H_

#include "IGUIFrameIncludes.h"
#include "IGUIFrameControl.h"
#include "IGUIFrameRenderer.h"
#include "IGUIFrameInput.h"
#include "IGUIFrameTimer.h"
#include <vector>

class CIGUIFrameWindow
{
protected:
	std::vector<CIGUIFrameControl*> m_control_list;
	//CIGUIFrameRenderer *m_renderer;
public:
	CIGUIFrameWindow(void);
	virtual ~CIGUIFrameWindow(void);
	virtual int display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur = false) = 0;
};

#endif //_IGUIFRAMEWINDOW_H_
