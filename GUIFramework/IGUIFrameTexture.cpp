#include "Include\IGUIFrameTexture.h"


CIGUIFrameTexture::CIGUIFrameTexture(void)
	: m_width(0)
	, m_height(0)
	, m_bpp(0)
	, m_bTexLoaded(false)
	, m_bStaticBuf(true)
{
}


CIGUIFrameTexture::~CIGUIFrameTexture(void)
{
}

const std::string *CIGUIFrameTexture::getErrorDesc(eTextureResult result)
{
	return &m_pszTextureResult[result];
}

bool CIGUIFrameTexture::getLoadedStatus()
{
	return m_bTexLoaded;
}

int CIGUIFrameTexture::getWidth()
{
	return m_width;
}

int CIGUIFrameTexture::getHeight()
{
	return m_height;
}

int CIGUIFrameTexture::getBpp()
{
	return m_bpp;
}

const std::string CIGUIFrameTexture::m_pszTextureResult[] = {
	"Operation completed successfully",
	"File type not supported",
	"Texture load method not supported",
	"Can't read file",
	"Out of memory",
	"Unhandled failure",
	"Function is not supported"
};
