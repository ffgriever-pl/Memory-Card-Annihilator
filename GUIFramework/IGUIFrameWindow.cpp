#include "Include\IGUIFrameWindow.h"


CIGUIFrameWindow::CIGUIFrameWindow(void)
{
}


CIGUIFrameWindow::~CIGUIFrameWindow(void)
{
	for (u32 i = 0; i < m_control_list.size(); i++)
	{
		if (m_control_list[i] != NULL)
		{
			delete m_control_list[i];
			m_control_list[i] = NULL;
		}
	}
	m_control_list.clear();
}
