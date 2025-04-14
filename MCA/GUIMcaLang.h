#ifndef _GUIMCALANG_H_
#define _GUIMCALANG_H_

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "IGUIFrameRenderer.h"

class CGUIMcaLang
{
public:
	typedef std::map<const std::string, const std::string>::value_type langmapRaw;
private:
	static const std::map<const std::string, const std::string> m_default_lang;
	static const int m_numof_entries;
	static const langmapRaw m_default_lang_raw[];
	std::map<std::string, std::string> m_curr_lang;
public:
	const char *getText(const char *label);
	static void replace(std::string *base, const std::string find, const std::string repl);
	static void replace(std::string *base, const std::string find, const long long repln);
	static void trimRight(std::string& str, const char* chars2remove);
	static void trimLeft(std::string& str, const char* chars2remove);
	void initLang(std::string input);
	CGUIMcaLang(void);
	~CGUIMcaLang(void);
};

#endif //_GUIMCALANG_H_
