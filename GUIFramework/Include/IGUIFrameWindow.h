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
	CIGUIFrameWindow(void);
public:
	virtual ~CIGUIFrameWindow(void);
};

#endif //_IGUIFRAMEWINDOW_H_
