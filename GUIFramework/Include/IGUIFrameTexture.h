#ifndef _IGUIFRAMETEXTURE_H_
#define _IGUIFRAMETEXTURE_H_

#include <string>
#include "IGUIFrameIncludes.h"

class CIGUIFrameTexture
{
public:
	enum eTextureResult
	{
		etResOk,
		etResNotSupported,
		etResMethodUnsupported,
		etResCantRead,
		etResNoMemory,
		etResFail,
		etResFuncUnsupported
	};
	enum eTextureFilter
	{
		etFiltNearest,
		etFiltBilinear,
		etFiltTrilinear
	};
protected:
	int m_width;
	int m_height;
	int m_bpp;
	bool m_bTexLoaded;
	bool m_bStaticBuf;
	u8 *m_pBuffer;
	static const std::string m_pszTextureResult[];
public:
	CIGUIFrameTexture(void);
	virtual ~CIGUIFrameTexture(void);

	const std::string *getErrorDesc(eTextureResult result);
	bool getLoadedStatus();
	int getWidth();
	int getHeight();
	int getBpp();

	virtual eTextureResult loadTextureBuffer(u8 *buffer, u32 size, bool staticBuffer) = 0;
	virtual eTextureResult loadTextureFile(char filename[]) = 0;
	virtual void setTexfilter(eTextureFilter filter) = 0;
	virtual void blur(int size) = 0;
};

#endif //_IGUIFRAMETEXTURE_H_
