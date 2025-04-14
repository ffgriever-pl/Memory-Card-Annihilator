#ifndef _CPS2APPLICATION_H_
#define _CPS2APPLICATION_H_

#include <string>
#include "GUIFramePS2Includes.h"

class CPS2Application
{
private:
	static CPS2Application *m_pInstance;
	CPS2Application(void);
	static bool loadLanguage(const std::string& langfile);
	static void initLanguage();
	static void setBootPath(const char* path);
public:
	~CPS2Application(void);
	int main(int argc, char *argv[]);
	static CPS2Application* getInstance();
	static void delInstance();
};

#endif //_CPS2APPLICATION_H_
