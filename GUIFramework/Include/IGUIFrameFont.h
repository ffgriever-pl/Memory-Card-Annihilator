#ifndef _IGUIFRAMEFONT_H_
#define _IGUIFRAMEFONT_H_

#include <vector>
#include <map>
#include <cstdio>
#include "IGUIFrameIncludes.h"
#include "IGUIFrameTexture.h"
#include "IGUIFrameRenderer.h"

template <class T>
class CIGUIFrameFont
{
private:
	struct tmyGlyph
	{
		u32 unicode;
		u16 x;
		u16 y;
		s16 width;
		s16 height;
		s16 xoff;
		s16 yoff;
		s16 xadv;
		u8  pagenum;
		u8  channel;
	};
	struct tmyKern
	{
		u32 unicode_first;
		u32 unicode_second;
		s16 kerning;
		s16 padding;
	};
	struct tmyHead
	{
		u32 magic; //BFNK 0x4b4e4642
		s16 font_height;
		s16 line_height;
		s16 base_pos;
		u16 tex_width;
		u16 tex_height;
		u16 tex_numof;
		u32 tex_offset;
		u32 glyph_numof;
		u32 glyph_offset;
		u32 kern_numof;
		u32 kern_offset;
	};
	class tGlyph
	{
	public:
		u16 x;
		u16 y;
		s16 width;
		s16 height;
		s16 xoff; //add this to x to get proper position
		s16 yoff; //add this to x to get proper position
		s16 xadv; //add this to total drawn size (next char start)
		u8  page; //texture number;

		tGlyph(u16 cx, u16 cy, s16 cw, s16 ch, s16 cxo, s16 cyo, s16 cxadv, u8 cpage)
			: x(cx), y(cy)
			, width(cw), height(ch)
			, xoff(cxo), yoff(cyo)
			, xadv(cxadv)
			, page(cpage)
		{
		}
	};
	struct tkernPair
	{
		u32 prevChar;
		int kerning;
		tkernPair(int prev, int kern)
			: prevChar(prev)
			, kerning(kern)
		{
		}
	};
	struct tCharToDraw
	{
		float x;
		float y;
		float w;
		float h;
		float u1;
		float u2;
		float v1;
		float v2;
		float xadv;
		bool space;
		int page;
		u8 r;
		u8 g;
		u8 b;
		float alpha;
		u8 r2;
		u8 g2;
		u8 b2;
		float alpha2;
		u8 r3;
		u8 g3;
		u8 b3;
		float alpha3;
		u8 r4;
		u8 g4;
		u8 b4;
		float alpha4;
		bool goraud;
		tCharToDraw(float _x, float _y, float _w, float _h, float _u1, float _u2, float _v1, float _v2, float _xadv, bool _space, u8 _page, u8 _r, u8 _g, u8 _b, float _alpha, u8 _r2 = 0, u8 _g2 = 0, u8 _b2 = 0, float _alpha2 = 0.0f, u8 _r3 = 0, u8 _g3 = 0, u8 _b3 = 0, float _alpha3 = 0.0f, u8 _r4 = 0, u8 _g4 = 0, u8 _b4 = 0, float _alpha4 = 0.0f, bool _goraud = false)
			: x(_x), y(_y)
			, w(_w), h(_h)
			, u1(_u1), u2(_u2)
			, v1(_v1), v2(_v2)
			, xadv(_xadv), space(_space)
			, page(_page)
			, r(_r), g(_g), b(_b), alpha(_alpha)
			, r2(_r2), g2(_g2), b2(_b2), alpha2(_alpha2)
			, r3(_r3), g3(_g3), b3(_b3), alpha3(_alpha3)
			, r4(_r4), g4(_g4), b4(_b4), alpha4(_alpha4)
			, goraud(_goraud)
		{
		};
	};
	std::multimap<int, tkernPair> m_kerning; //curr char - tkernPair(prev char, kerr) - usage on www.yolinux.com/TUTORIALS/CppStlMultimap.html#MULTIMAP
	std::vector<CIGUIFrameTexture *> m_tex_list;
	std::map<int, tGlyph *> m_glyphs;
	CIGUIFrameRenderer *m_renderer;
	bool m_init_done;

	s16 m_font_height;
	s16 m_line_height;
	s16 m_base_pos;
	u16 m_tex_width;
	u16 m_tex_height;

	static const u8 utf8d[];
	void clearEverything();
public:
	enum eAlignment
	{
		etxAlignLeft,
		etxAlignRight,
		etxAlignCenter,
		etxAlignJustify,
		etxAlignLeftRight
	};
	CIGUIFrameFont();
	~CIGUIFrameFont(void);
	bool loadFontBuffer(u8 *buffer, u32 size, CIGUIFrameRenderer *renderer);
	void deinit();
	s16 getBasePos();
	float getScale(float size);
	static void utf8decode(const char* s, u32 *outstr, s32 max);
	static void asciidecode(const char* s, u32 *outstr, s32 max);
	static u32 wstrlen(const u32 *string);
	tVertex2 printUTF8(const char *string, float xpos, float ypos, float size, float spacing, u8 r, u8 g, u8 b, float alpha, u8 r2 = 0, u8 g2 = 0, u8 b2 = 0, float alpha2 = 0.0f, u8 r3 = 0, u8 g3 = 0, u8 b3 = 0, float alpha3 = 0.0f, u8 r4 = 0, u8 g4 = 0, u8 b4 = 0, float alpha4 = 0.0f, bool goraud = false);//size = 0: default, rgb 0-128, alpha 0-1
	tVertex2 checkLenUTF8(const char *string, float size, float spacing);
	tVertex2 printASCII(const char *string, float xpos, float ypos, float size, float spacing, u8 r, u8 g, u8 b, float alpha, u8 r2 = 0, u8 g2 = 0, u8 b2 = 0, float alpha2 = 0.0f, u8 r3 = 0, u8 g3 = 0, u8 b3 = 0, float alpha3 = 0.0f, u8 r4 = 0, u8 g4 = 0, u8 b4 = 0, float alpha4 = 0.0f, bool goraud = false);//size = 0: default, rgb 0-128, alpha 0-1
	tVertex2 printUTF8Box(const char *string, float xpos, float ypos, int bw, int bh, eAlignment align, float size, float spacing, u8 r, u8 g, u8 b, float alpha, u8 r2 = 0, u8 g2 = 0, u8 b2 = 0, float alpha2 = 0.0f, u8 r3 = 0, u8 g3 = 0, u8 b3 = 0, float alpha3 = 0.0f, u8 r4 = 0, u8 g4 = 0, u8 b4 = 0, float alpha4 = 0.0f, bool goraud = false);//size = 0: default, rgb 0-128, alpha 0-1
	tVertex2 printASCIIBox(const char *string, float xpos, float ypos, int bw, int bh, eAlignment align, float size, float spacing, u8 r, u8 g, u8 b, float alpha, u8 r2 = 0, u8 g2 = 0, u8 b2 = 0, float alpha2 = 0.0f, u8 r3 = 0, u8 g3 = 0, u8 b3 = 0, float alpha3 = 0.0f, u8 r4 = 0, u8 g4 = 0, u8 b4 = 0, float alpha4 = 0.0f, bool goraud = false);//size = 0: default, rgb 0-128, alpha 0-1
};

template <class T> CIGUIFrameFont<T>::CIGUIFrameFont()
{
	m_init_done = false;
}

template <class T> CIGUIFrameFont<T>::~CIGUIFrameFont(void)
{
	clearEverything();
}

template <class T> void CIGUIFrameFont<T>::clearEverything()
{
	if (!m_init_done) return;
	typename std::vector<CIGUIFrameTexture *>::iterator itex;

	for (itex = m_tex_list.begin(); itex != m_tex_list.end(); itex++)
	{
		if (*itex)
			delete (*itex);
	}

	m_tex_list.clear();

	typename std::map<int, tGlyph *>::iterator iglyph;
	for (iglyph = m_glyphs.begin(); iglyph != m_glyphs.end(); iglyph++)
	{
		if ( (*iglyph).second )
			delete (*iglyph).second;
	}

	m_glyphs.clear();

	m_kerning.clear();

	m_init_done = false;
}

template <class T> void CIGUIFrameFont<T>::deinit()
{
	clearEverything();
}

template <class T> bool CIGUIFrameFont<T>::loadFontBuffer(u8 *buffer, u32 size, CIGUIFrameRenderer *renderer)
{
	if (m_init_done) return false;
	m_renderer = renderer;

	tmyHead *myHead = (tmyHead*)buffer;

	if (myHead->magic != 0x4b4e4642) //BFNK
	{
		return false;
	}
	m_font_height = myHead->font_height;
	m_line_height = myHead->line_height;
	m_base_pos = myHead->base_pos;
	m_tex_width = myHead->tex_width;
	m_tex_height = myHead->tex_height;

	tmyGlyph *myGlyphs = (tmyGlyph *)&buffer[myHead->glyph_offset];
	tmyKern *myKerns = (tmyKern *)&buffer[myHead->kern_offset];
	u32 *myTexOffsets = (u32 *)&buffer[myHead->tex_offset];

	for (u32 i = 0; i < myHead->glyph_numof; i++)
	{
		m_glyphs[myGlyphs[i].unicode] = new tGlyph(myGlyphs[i].x, myGlyphs[i].y, myGlyphs[i].width, myGlyphs[i].height, myGlyphs[i].xoff, myGlyphs[i].yoff, myGlyphs[i].xadv, myGlyphs[i].pagenum);
	}
	for (u32 i = 0; i < myHead->kern_numof; i++)
	{
		m_kerning.insert( std::pair<int, tkernPair>(myKerns[i].unicode_second, tkernPair(myKerns[i].unicode_first, myKerns[i].kerning) ) );
	}
	for (u32 i = 0; i < myHead->tex_numof; i++)
	{
		CIGUIFrameTexture *newtex = new T;

		if (newtex->loadTextureBuffer(&buffer[myTexOffsets[i*2 +0]], myTexOffsets[i*2 +1], false) != CIGUIFrameTexture::etResOk)
		{
			m_init_done = true;
			delete newtex;
			clearEverything();
			return false;
		}
		newtex->setTexfilter(CIGUIFrameTexture::etFiltBilinear);

		m_tex_list.push_back(newtex);
	}

	m_init_done = true;
	return true;
}

/*
  The first 128 entries are tuples of 4 bit values. The lower bits
  are a mask that when xor'd with a byte removes the leading utf-8
  bits. The upper bits are a character class number. The remaining
  160 entries are a minimal deterministic finite automaton. It has
  10 states and each state has 13 character class transitions, and 
  3 unused transitions for padding reasons. When the automaton en-
  ters state zero, it has found a complete valid utf-8 code point;
  if it enters state one then the input sequence is not utf-8. The
  start state is state nine. Note the mixture of octal and decimal
  for stylistic reasons. The ASCII_IN_TABLE macro makes the array
  bigger and the code simpler--but not necessarily faster--if set.
*/

template <class T> const u8 CIGUIFrameFont<T>::utf8d[] = {

	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

	070,070,070,070,070,070,070,070,070,070,070,070,070,070,070,070,
	050,050,050,050,050,050,050,050,050,050,050,050,050,050,050,050,
	030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,
	030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,030,
	204,204,188,188,188,188,188,188,188,188,188,188,188,188,188,188,
	188,188,188,188,188,188,188,188,188,188,188,188,188,188,188,188,
	174,158,158,158,158,158,158,158,158,158,158,158,158,142,126,126,
	111, 95, 95, 95, 79,207,207,207,207,207,207,207,207,207,207,207,

	0,1,1,1,8,7,6,4,5,4,3,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,1,1,1,1,1,1,1,1,1,1,1,1,
	1,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,4,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,8,7,6,4,5,4,3,2,1,1,1,1,

};

template <class T> void CIGUIFrameFont<T>::utf8decode(const char* s, u32 *outstr, s32 max)
{
	u8 data, byte, stat = 9;
	u32 unic = 0;

	while ((byte = *s++) && max--)
	{

		// Each byte is associated with a character class and a mask;
		// The character class is used to advance a finite automaton;
		// The mask is used to strip off leading bits from the byte;
		// The remaining bits are combined into a Unicode code point;
		// A code point is complete if the DFA enters the final state.

		data = utf8d[ byte ];
		stat = utf8d[ 256 + (stat << 4) + (data >> 4) ];
		byte = (byte ^ (u8)(data << 4));

		unic = (unic << 6) | byte;

		if (!stat)
		{
			// unic is now a proper code point, we just print it out.
			*outstr++ = unic;
			unic = 0;
		}

		if (stat == 1)
		{
			// the byte is not allowed here; the state would have to
			// be reset to continue meaningful reading of the string
		}

	}
	*outstr = 0;
}

template <class T> void CIGUIFrameFont<T>::asciidecode(const char* s, u32 *outstr, s32 max)
{
	u8 byte;

	while ((byte = *s++) && max--)
	{
		if (byte > 31 && byte < 127) *outstr++ = byte;
		else *outstr++ = '_';
	}
	*outstr = 0;
}

template <class T> u32 CIGUIFrameFont<T>::wstrlen(const u32 *string)
{
	u32 len = 0;
	while (*string++) len++;

	return len;
}

template <class T> s16 CIGUIFrameFont<T>::getBasePos()
{
	return m_base_pos;
}

template <class T> float CIGUIFrameFont<T>::getScale(float size)
{
	return (size == 0.0f ? 1.0f : size/(float)m_font_height);
}

template <class T> tVertex2 CIGUIFrameFont<T>::checkLenUTF8(const char *string, float size, float spacing)
{
	u32 max_uni_size = strlen(string)+ (strlen(string)>>1) +1;
	u32 *unicode = new u32[max_uni_size];
	u32 *curchar = unicode;
	u32 prevchar = 0;//0 means first

	float caretx = 0;
	float carety = 0;
	spacing = 0.0f;

	float scale = (size == 0.0f ? 1.0f : size/(float)m_font_height);

	utf8decode(string, unicode, max_uni_size-1);
	unicode[max_uni_size-1] = 0;

	typename std::map<int, tGlyph *>::iterator iglyph;

	while ((*curchar))
	{
		if ( (iglyph = m_glyphs.find(*curchar)) != m_glyphs.end() )
		{
			if (prevchar != 0)
			{
				typename std::pair<typename std::multimap<int, tkernPair>::iterator, typename std::multimap<int, tkernPair>::iterator> kernings;
				kernings = m_kerning.equal_range(*curchar);

				for (typename std::multimap<int, tkernPair>::iterator ikern = kernings.first; ikern != kernings.second; ikern++)
				{
					if ( (*ikern).second.prevChar == prevchar )
					{
						caretx += (float)((*ikern).second.kerning)*scale;
					}
				}
			}

			caretx += (float)(*iglyph).second->xadv * scale;
		}
		
		prevchar = *curchar;
		curchar++;
	}

	delete [] unicode;

	return tVertex2(caretx, carety);
}

template <class T> tVertex2 CIGUIFrameFont<T>::printUTF8(const char *string, float xpos, float ypos, float size, float spacing, u8 r, u8 g, u8 b, float alpha, u8 r2, u8 g2, u8 b2, float alpha2, u8 r3, u8 g3, u8 b3, float alpha3, u8 r4, u8 g4, u8 b4, float alpha4, bool goraud)
{
	u32 max_uni_size = strlen(string)+ (strlen(string)>>1) +1;
	u32 *unicode = new u32[max_uni_size];
	u32 *curchar = unicode;
	u32 prevchar = 0;//0 means first

	float caretx = 0;
	float carety = 0;
	spacing = 0.0f;

	float scale = (size == 0.0f ? 1.0f : size/(float)m_font_height);

	utf8decode(string, unicode, max_uni_size-1);
	unicode[max_uni_size-1] = 0;

	typename std::map<int, tGlyph *>::iterator iglyph;

	while ((*curchar))
	{
		if ( (iglyph = m_glyphs.find(*curchar)) != m_glyphs.end() )
		{
			float x, y, u1, v1, w, h, u2, v2;
			u1 = (*iglyph).second->x;
			v1 = (*iglyph).second->y;
			w  = (float)(*iglyph).second->width*scale +scale;
			h  = (float)(*iglyph).second->height*scale +scale;
			u2 = u1 + (*iglyph).second->width +1.0f;
			v2 = v1 + (*iglyph).second->height +1.0f;

			if (prevchar != 0)
			{
				typename std::pair<typename std::multimap<int, tkernPair>::iterator, typename std::multimap<int, tkernPair>::iterator> kernings;
				kernings = m_kerning.equal_range(*curchar);

				for (typename std::multimap<int, tkernPair>::iterator ikern = kernings.first; ikern != kernings.second; ikern++)
				{
					if ( (*ikern).second.prevChar == prevchar )
					{
						caretx += (float)((*ikern).second.kerning)*scale;
					}
				}
			}

			x = xpos + caretx +(float)(*iglyph).second->xoff*scale + spacing*scale;
			y = ypos + carety +(float)(*iglyph).second->yoff*scale -(float)m_base_pos*scale;

			if (goraud)
			{
				m_renderer->drawSpriteGT(
					m_tex_list[(*iglyph).second->page],
					x, y,
					w, h,
					u1, v1,
					u2, v2,
					r, g, b,
					r2, g2, b2,
					r3, g3, b3,
					r4, g4, b4,
					alpha, alpha2, alpha3, alpha4
				);
			} else
			{
				m_renderer->drawSpriteT(
					m_tex_list[(*iglyph).second->page],
					x, y,
					w, h,
					u1, v1,
					u2, v2,
					r, g, b, alpha
				);
			}

			caretx += (float)(*iglyph).second->xadv * scale;
		}
		
		prevchar = *curchar;
		curchar++;
	}

	delete [] unicode;

	return tVertex2(caretx+xpos, carety+ypos);
}

template <class T> tVertex2 CIGUIFrameFont<T>::printASCII(const char *string, float xpos, float ypos, float size, float spacing, u8 r, u8 g, u8 b, float alpha, u8 r2, u8 g2, u8 b2, float alpha2, u8 r3, u8 g3, u8 b3, float alpha3, u8 r4, u8 g4, u8 b4, float alpha4, bool goraud)
{
	u32 max_uni_size = strlen(string)+ (strlen(string)>>1) +1;
	u32 *unicode = new u32[max_uni_size];
	u32 *curchar = unicode;
	u32 prevchar = 0;//0 means first

	float caretx = 0;
	float carety = 0;
	spacing = 0.0f;

	float scale = (size == 0.0f ? 1.0f : size/(float)m_font_height);

	asciidecode(string, unicode, max_uni_size-1);
	unicode[max_uni_size-1] = 0;

	typename std::map<int, tGlyph *>::iterator iglyph;

	while ((*curchar))
	{
		if ( (iglyph = m_glyphs.find(*curchar)) != m_glyphs.end() )
		{
			float x, y, u1, v1, w, h, u2, v2;
			u1 = (*iglyph).second->x;
			v1 = (*iglyph).second->y;
			w  = (float)(*iglyph).second->width*scale +scale;
			h  = (float)(*iglyph).second->height*scale +scale;
			u2 = u1 + (*iglyph).second->width +1.0f;
			v2 = v1 + (*iglyph).second->height +1.0f;

			if (prevchar != 0)
			{
				typename std::pair<typename std::multimap<int, tkernPair>::iterator, typename std::multimap<int, tkernPair>::iterator> kernings;
				kernings = m_kerning.equal_range(*curchar);

				for (typename std::multimap<int, tkernPair>::iterator ikern = kernings.first; ikern != kernings.second; ikern++)
				{
					if ( (*ikern).second.prevChar == prevchar )
					{
						caretx += (float)((*ikern).second.kerning)*scale;
					}
				}
			}

			x = xpos + caretx +(float)(*iglyph).second->xoff*scale + spacing*scale;
			y = ypos + carety +(float)(*iglyph).second->yoff*scale -(float)m_base_pos*scale;

			if (goraud)
			{
				m_renderer->drawSpriteGT(
					m_tex_list[(*iglyph).second->page],
					x, y,
					w, h,
					u1, v1,
					u2, v2,
					r, g, b,
					r2, g2, b2,
					r3, g3, b3,
					r4, g4, b4,
					alpha, alpha2, alpha3, alpha4
				);
			} else
			{
				m_renderer->drawSpriteT(
					m_tex_list[(*iglyph).second->page],
					x, y,
					w, h,
					u1, v1,
					u2, v2,
					r, g, b, alpha
				);
			}

			caretx += (float)(*iglyph).second->xadv * scale;
		}
		
		prevchar = *curchar;
		curchar++;
	}

	delete [] unicode;

	return tVertex2(caretx+xpos, carety+ypos);
}

template <class T> tVertex2 CIGUIFrameFont<T>::printUTF8Box(const char *string, float xpos, float ypos, int bw, int bh, eAlignment align, float size, float spacing, u8 r, u8 g, u8 b, float alpha, u8 r2, u8 g2, u8 b2, float alpha2, u8 r3, u8 g3, u8 b3, float alpha3, u8 r4, u8 g4, u8 b4, float alpha4, bool goraud)
{
	u32 max_uni_size = strlen(string)+ (strlen(string)>>1) +1;
	u32 *unicode = new u32[max_uni_size];
	u32 *curchar = unicode;
	u32 prevchar = 0;//0 means first

	float scale = (size == 0.0f ? 1.0f : size/(float)m_font_height);
	float caretx = 0;
	float carety = (float)m_base_pos*scale;
	spacing = 0.0f;

	int spacesinline = 0;
	int wordstart = 0;
	int charnum = 0;
	typename std::map<int, tGlyph *>::iterator iglyph;

	utf8decode(string, unicode, max_uni_size-1);
	unicode[max_uni_size-1] = 0;

	typename std::vector<tCharToDraw> line;
	typename std::vector<tCharToDraw> word;

	m_renderer->setScissor(true, (int)(xpos+0.5f), (int)(ypos+0.5f), (int)(xpos+bw), (int)(ypos+bh));
	bool last = false;
	while ((*curchar))
	{
		if (carety -(float)m_base_pos*scale > (float)bh) break;

		if (curchar[1] == 0) last = true;
		switch (*curchar)
		{
			case 0x20: //space
				spacesinline++;
				wordstart = charnum+1;
				break;
			case 0x3000: //japanese space
				spacesinline++;
				wordstart = charnum+1;
				break;
			case 0x0d: //cr
				curchar++;
				charnum++;
				continue;
				break;
			case 0x0a: //lf
				{
					float movestart = 0.0f;
					float movespace = 0.0f;
					for (u32 i = 0; i < word.size(); i++)
					{
						line.push_back(word[i]);
					}
					word.clear();
					if (!line.empty())
					{
						while(!line.empty() && line.back().space)
						{
							spacesinline--;
							line.pop_back();
						}
						float endpos = line.back().x+line.back().w-xpos;
						switch (align)
						{
							case etxAlignJustify:
								break;
							case etxAlignRight:
								movestart = (float)bw - endpos;
								break;
							case etxAlignCenter:
								movestart = ((float)bw - endpos)/2.0f;
								break;
							case etxAlignLeft:
							case etxAlignLeftRight:
							default:
								//nothing here, just draw it
								break;
						}
						float spaceadd = 0.0f;
						for (u32 i = 0; i < line.size(); i++)
						{
							if (line[i].space) spaceadd += movespace;
							if (line[i].goraud)
							{
								m_renderer->drawSpriteGT(
									m_tex_list[line[i].page],
									line[i].x +movestart+spaceadd, line[i].y,
									line[i].w, line[i].h,
									line[i].u1, line[i].v1,
									line[i].u2, line[i].v2,
									line[i].r, line[i].g, line[i].b,
									line[i].r2, line[i].g2, line[i].b2,
									line[i].r3, line[i].g3, line[i].b3,
									line[i].r4, line[i].g4, line[i].b4,
									line[i].alpha, line[i].alpha2, line[i].alpha3, line[i].alpha4
								);
							} else
							{
								m_renderer->drawSpriteT(
									m_tex_list[line[i].page],
									line[i].x +movestart+spaceadd, line[i].y,
									line[i].w, line[i].h,
									line[i].u1, line[i].v1,
									line[i].u2, line[i].v2,
									line[i].r, line[i].g, line[i].b, line[i].alpha
								);
							}
						}
					}
					prevchar = 0;
					line.clear();

					spacesinline = 0;
					caretx = 0;
					carety += m_line_height*scale; //if variable height is implemented, always keep largest scale for line
					curchar++;
					charnum++;
					wordstart = charnum;
					continue;
				}
				break;
		}

		if ( (iglyph = m_glyphs.find(*curchar)) != m_glyphs.end() )
		{
			float x, y, u1, v1, w, h, u2, v2;
			u1 = (*iglyph).second->x;
			v1 = (*iglyph).second->y;
			w  = (float)(*iglyph).second->width*scale +scale;
			h  = (float)(*iglyph).second->height*scale +scale;
			u2 = u1 + (*iglyph).second->width +1.0f;
			v2 = v1 + (*iglyph).second->height +1.0f;

			if (prevchar != 0)
			{
				typename std::pair<typename std::multimap<int, tkernPair>::iterator, typename std::multimap<int, tkernPair>::iterator> kernings;
				kernings = m_kerning.equal_range(*curchar);

				for (typename std::multimap<int, tkernPair>::iterator ikern = kernings.first; ikern != kernings.second; ikern++)
				{
					if ( (*ikern).second.prevChar == prevchar )
					{
						caretx += (float)((*ikern).second.kerning)*scale;
					}
				}
			}
			if (prevchar == 0)
				caretx -= (float)(*iglyph).second->xoff*scale;

			x = xpos + caretx +(float)(*iglyph).second->xoff*scale + spacing*scale;
			y = ypos + carety +(float)(*iglyph).second->yoff*scale -(float)m_base_pos*scale;

			caretx += (float)(*iglyph).second->xadv * scale;

			bool drawWholeLine = false;
			if ((caretx > (float)bw && align != etxAlignLeftRight) || last)
			{
				float movestart = 0.0f;
				float movespace = 0.0f;

				if (spacesinline > 0 && *curchar != 0x20 && *curchar != 0x3000) spacesinline--;
				if (spacesinline == 0)
				{//simply break... looooooooong text in line
					if (last)
					{
						if (goraud)
							word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha, r2, g2, b2, alpha2, r3, g3, b3, alpha3, r4, g4, b4, alpha4, true));
						else
							word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha));
					}
					for (u32 i = 0; i < word.size(); i++)
					{
						line.push_back(word[i]);
					}
					word.clear();

					float endpos = line.back().x+line.back().w-xpos;
					switch (align)
					{
						case etxAlignJustify:
							break;
						case etxAlignRight:
							movestart = (float)bw - endpos;
							break;
						case etxAlignCenter:
							movestart = ((float)bw - endpos)/2.0f;
							break;
						case etxAlignLeftRight:
							if (caretx > (float)bw) movestart = (float)bw - endpos;
							break;
						case etxAlignLeft:
						default:
							//nothing here, just draw it
							break;
					}
					if (!last)
					{
						caretx = 0;
						if (align != etxAlignLeftRight) carety += m_line_height*scale; //if variable height is implemented, always keep largest scale for line
					}
					y +=  m_line_height*scale; //if variable height is implemented, always keep largest scale for line
					drawWholeLine = true;
					wordstart = charnum;
				} else
				{
					if (last || (*curchar == 0x20 || *curchar == 0x3000))
					{
						if (last)
						{
							if (goraud)
								word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha, r2, g2, b2, alpha2, r3, g3, b3, alpha3, r4, g4, b4, alpha4, true));
							else
								word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha));
						}
						for (u32 i = 0; i < word.size(); i++)
						{
							line.push_back(word[i]);
						}
						word.clear();
					}
					int addword = 0;
					while (unicode[wordstart+addword] == 0x20 || unicode[wordstart+addword] == 0x3000) addword++;
					wordstart += addword;
					charnum = wordstart;
					curchar = &unicode[charnum];

					while(!line.empty() && line.back().space)
					{
						spacesinline--;
						line.pop_back();
					}
					float endpos = line.back().x+line.back().w-xpos;
					switch (align)
					{
						case etxAlignJustify:
							if (!last && spacesinline > 0) movespace = ((float)bw - endpos)/(float)spacesinline; //if space then xadv += xadv*movespacescale
							break;
						case etxAlignRight:
							movestart = (float)bw - endpos;
							break;
						case etxAlignCenter:
							movestart = ((float)bw - endpos)/2.0f;
							break;
						case etxAlignLeftRight:
							if (caretx > (float)bw) movestart = (float)bw - endpos;
							break;
						case etxAlignLeft:
						default:
							//nothing here, just draw it
							break;
					}
					spacesinline = 0;
					if (!last)
					{
						caretx = 0;
						carety += m_line_height*scale; //if variable height is implemented, always keep largest scale for line
					}
					word.clear();
					drawWholeLine = true;
				}
				if (drawWholeLine)
				{
					float spaceadd = 0.0f;
					for (u32 i = 0; i < line.size(); i++)
					{
						if (line[i].goraud)
						{
							m_renderer->drawSpriteGT(
								m_tex_list[line[i].page],
								line[i].x +movestart+spaceadd, line[i].y,
								line[i].w, line[i].h,
								line[i].u1, line[i].v1,
								line[i].u2, line[i].v2,
								line[i].r, line[i].g, line[i].b,
								line[i].r2, line[i].g2, line[i].b2,
								line[i].r3, line[i].g3, line[i].b3,
								line[i].r4, line[i].g4, line[i].b4,
								line[i].alpha, line[i].alpha2, line[i].alpha3, line[i].alpha4
							);
						} else
						{
							m_renderer->drawSpriteT(
								m_tex_list[line[i].page],
								line[i].x +movestart+spaceadd, line[i].y,
								line[i].w, line[i].h,
								line[i].u1, line[i].v1,
								line[i].u2, line[i].v2,
								line[i].r, line[i].g, line[i].b, line[i].alpha
							);
						}
						if (line[i].space) spaceadd += movespace;
					}
					prevchar = 0;
					line.clear();

					if (last) break;
					continue;
				}
			} else
			{
				if (*curchar == 0x20 || *curchar == 0x3000)
				{
					for (u32 i = 0; i < word.size(); i++)
					{
						line.push_back(word[i]);
					}
					word.clear();
				}

				if (goraud)
					word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha, r2, g2, b2, alpha2, r3, g3, b3, alpha3, r4, g4, b4, alpha4, true));
				else
					word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha));
			}			
		}
		
		prevchar = *curchar;
		charnum++;
		curchar++;
		if (last) break;
	}
	m_renderer->setScissor(false);
	delete [] unicode;

	if (align == etxAlignRight)
	{
		return tVertex2(bw+xpos, carety+ypos -(float)m_base_pos*scale);
	} else if (align == etxAlignLeftRight)
	{
		if (caretx > (float)bw)
			return tVertex2(bw+xpos, carety+ypos -(float)m_base_pos*scale);
		else
			return tVertex2(caretx+xpos, carety+ypos -(float)m_base_pos*scale);
	} else
	{
		return tVertex2(caretx+xpos, carety+ypos -(float)m_base_pos*scale);
	}
}

template <class T> tVertex2 CIGUIFrameFont<T>::printASCIIBox(const char *string, float xpos, float ypos, int bw, int bh, eAlignment align, float size, float spacing, u8 r, u8 g, u8 b, float alpha, u8 r2, u8 g2, u8 b2, float alpha2, u8 r3, u8 g3, u8 b3, float alpha3, u8 r4, u8 g4, u8 b4, float alpha4, bool goraud)
{
	u32 max_uni_size = strlen(string)+ (strlen(string)>>1) +1;
	u32 *unicode = new u32[max_uni_size];
	u32 *curchar = unicode;
	u32 prevchar = 0;//0 means first

	float scale = (size == 0.0f ? 1.0f : size/(float)m_font_height);
	float caretx = 0;
	float carety = (float)m_base_pos*scale;
	spacing = 0.0f;

	int spacesinline = 0;
	int wordstart = 0;
	int charnum = 0;
	typename std::map<int, tGlyph *>::iterator iglyph;

	asciidecode(string, unicode, max_uni_size-1);
	unicode[max_uni_size-1] = 0;

	typename std::vector<tCharToDraw> line;
	typename std::vector<tCharToDraw> word;

	m_renderer->setScissor(true, (int)(xpos+0.5f), (int)(ypos+0.5f), (int)(xpos+bw), (int)(ypos+bh));
	bool last = false;
	while ((*curchar))
	{
		if (carety -(float)m_base_pos*scale > (float)bh) break;

		if (curchar[1] == 0) last = true;
		switch (*curchar)
		{
			case 0x20: //space
				spacesinline++;
				wordstart = charnum+1;
				break;
			case 0x3000: //japanese space
				spacesinline++;
				wordstart = charnum+1;
				break;
			case 0x0d: //cr
				curchar++;
				charnum++;
				continue;
				break;
			case 0x0a: //lf
				{
					float movestart = 0.0f;
					float movespace = 0.0f;
					for (u32 i = 0; i < word.size(); i++)
					{
						line.push_back(word[i]);
					}
					word.clear();
					if (!line.empty())
					{
						while(!line.empty() && line.back().space)
						{
							spacesinline--;
							line.pop_back();
						}
						float endpos = line.back().x+line.back().w-xpos;
						switch (align)
						{
							case etxAlignJustify:
								break;
							case etxAlignRight:
								movestart = (float)bw - endpos;
								break;
							case etxAlignCenter:
								movestart = ((float)bw - endpos)/2.0f;
								break;
							case etxAlignLeft:
							case etxAlignLeftRight:
							default:
								//nothing here, just draw it
								break;
						}
						float spaceadd = 0.0f;
						for (u32 i = 0; i < line.size(); i++)
						{
							if (line[i].space) spaceadd += movespace;
							if (line[i].goraud)
							{
								m_renderer->drawSpriteGT(
									m_tex_list[line[i].page],
									line[i].x +movestart+spaceadd, line[i].y,
									line[i].w, line[i].h,
									line[i].u1, line[i].v1,
									line[i].u2, line[i].v2,
									line[i].r, line[i].g, line[i].b,
									line[i].r2, line[i].g2, line[i].b2,
									line[i].r3, line[i].g3, line[i].b3,
									line[i].r4, line[i].g4, line[i].b4,
									line[i].alpha, line[i].alpha2, line[i].alpha3, line[i].alpha4
								);
							} else
							{
								m_renderer->drawSpriteT(
									m_tex_list[line[i].page],
									line[i].x +movestart+spaceadd, line[i].y,
									line[i].w, line[i].h,
									line[i].u1, line[i].v1,
									line[i].u2, line[i].v2,
									line[i].r, line[i].g, line[i].b, line[i].alpha
								);
							}
						}
					}
					prevchar = 0;
					line.clear();

					spacesinline = 0;
					caretx = 0;
					carety += m_line_height*scale; //if variable height is implemented, always keep largest scale for line
					curchar++;
					charnum++;
					wordstart = charnum;
					continue;
				}
				break;
		}

		if ( (iglyph = m_glyphs.find(*curchar)) != m_glyphs.end() )
		{
			float x, y, u1, v1, w, h, u2, v2;
			u1 = (*iglyph).second->x;
			v1 = (*iglyph).second->y;
			w  = (float)(*iglyph).second->width*scale +scale;
			h  = (float)(*iglyph).second->height*scale +scale;
			u2 = u1 + (*iglyph).second->width +1.0f;
			v2 = v1 + (*iglyph).second->height +1.0f;

			if (prevchar != 0)
			{
				typename std::pair<typename std::multimap<int, tkernPair>::iterator, typename std::multimap<int, tkernPair>::iterator> kernings;
				kernings = m_kerning.equal_range(*curchar);

				for (typename std::multimap<int, tkernPair>::iterator ikern = kernings.first; ikern != kernings.second; ikern++)
				{
					if ( (*ikern).second.prevChar == prevchar )
					{
						caretx += (float)((*ikern).second.kerning)*scale;
					}
				}
			}
			if (prevchar == 0)
				caretx -= (float)(*iglyph).second->xoff*scale;

			x = xpos + caretx +(float)(*iglyph).second->xoff*scale + spacing*scale;
			y = ypos + carety +(float)(*iglyph).second->yoff*scale -(float)m_base_pos*scale;

			caretx += (float)(*iglyph).second->xadv * scale;

			bool drawWholeLine = false;
			if ((caretx > (float)bw && align != etxAlignLeftRight) || last)
			{
				float movestart = 0.0f;
				float movespace = 0.0f;

				if (spacesinline > 0 && *curchar != 0x20 && *curchar != 0x3000) spacesinline--;
				if (spacesinline == 0)
				{//simply break... looooooooong text in line
					if (last)
					{
						if (goraud)
							word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha, r2, g2, b2, alpha2, r3, g3, b3, alpha3, r4, g4, b4, alpha4, true));
						else
							word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha));
					}
					for (u32 i = 0; i < word.size(); i++)
					{
						line.push_back(word[i]);
					}
					word.clear();

					float endpos = line.back().x+line.back().w-xpos;
					switch (align)
					{
						case etxAlignJustify:
							break;
						case etxAlignRight:
							movestart = (float)bw - endpos;
							break;
						case etxAlignCenter:
							movestart = ((float)bw - endpos)/2.0f;
							break;
						case etxAlignLeftRight:
							if (caretx > (float)bw) movestart = (float)bw - endpos;
							break;
						case etxAlignLeft:
						default:
							//nothing here, just draw it
							break;
					}
					if (!last)
					{
						caretx = 0;
						if (align != etxAlignLeftRight) carety += m_line_height*scale; //if variable height is implemented, always keep largest scale for line
					}
					y +=  m_line_height*scale; //if variable height is implemented, always keep largest scale for line
					drawWholeLine = true;
					wordstart = charnum;
				} else
				{
					if (last || (*curchar == 0x20 || *curchar == 0x3000))
					{
						if (last)
						{
							if (goraud)
								word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha, r2, g2, b2, alpha2, r3, g3, b3, alpha3, r4, g4, b4, alpha4, true));
							else
								word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha));
						}
						for (u32 i = 0; i < word.size(); i++)
						{
							line.push_back(word[i]);
						}
						word.clear();
					}
					int addword = 0;
					while (unicode[wordstart+addword] == 0x20 || unicode[wordstart+addword] == 0x3000) addword++;
					wordstart += addword;
					charnum = wordstart;
					curchar = &unicode[charnum];

					while(!line.empty() && line.back().space)
					{
						spacesinline--;
						line.pop_back();
					}
					float endpos = line.back().x+line.back().w-xpos;
					switch (align)
					{
						case etxAlignJustify:
							if (!last && spacesinline > 0) movespace = ((float)bw - endpos)/(float)spacesinline; //if space then xadv += xadv*movespacescale
							break;
						case etxAlignRight:
							movestart = (float)bw - endpos;
							break;
						case etxAlignCenter:
							movestart = ((float)bw - endpos)/2.0f;
							break;
						case etxAlignLeftRight:
							if (caretx > (float)bw) movestart = (float)bw - endpos;
							break;
						case etxAlignLeft:
						default:
							//nothing here, just draw it
							break;
					}
					spacesinline = 0;
					if (!last)
					{
						caretx = 0;
						carety += m_line_height*scale; //if variable height is implemented, always keep largest scale for line
					}
					word.clear();
					drawWholeLine = true;
				}
				if (drawWholeLine)
				{
					float spaceadd = 0.0f;
					for (u32 i = 0; i < line.size(); i++)
					{
						if (line[i].goraud)
						{
							m_renderer->drawSpriteGT(
								m_tex_list[line[i].page],
								line[i].x +movestart+spaceadd, line[i].y,
								line[i].w, line[i].h,
								line[i].u1, line[i].v1,
								line[i].u2, line[i].v2,
								line[i].r, line[i].g, line[i].b,
								line[i].r2, line[i].g2, line[i].b2,
								line[i].r3, line[i].g3, line[i].b3,
								line[i].r4, line[i].g4, line[i].b4,
								line[i].alpha, line[i].alpha2, line[i].alpha3, line[i].alpha4
							);
						} else
						{
							m_renderer->drawSpriteT(
								m_tex_list[line[i].page],
								line[i].x +movestart+spaceadd, line[i].y,
								line[i].w, line[i].h,
								line[i].u1, line[i].v1,
								line[i].u2, line[i].v2,
								line[i].r, line[i].g, line[i].b, line[i].alpha
							);
						}
						if (line[i].space) spaceadd += movespace;
					}
					prevchar = 0;
					line.clear();

					if (last) break;
					continue;
				}
			} else
			{
				if (*curchar == 0x20 || *curchar == 0x3000)
				{
					for (u32 i = 0; i < word.size(); i++)
					{
						line.push_back(word[i]);
					}
					word.clear();
				}

				if (goraud)
					word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha, r2, g2, b2, alpha2, r3, g3, b3, alpha3, r4, g4, b4, alpha4, true));
				else
					word.push_back(tCharToDraw(x, y, w, h, u1, u2, v1, v2, (float)(*iglyph).second->xadv * scale, ((*curchar == 0x20 || *curchar == 0x3000) ? true : false), (*iglyph).second->page, r, g, b, alpha));
			}			
		}
		
		prevchar = *curchar;
		charnum++;
		curchar++;
		if (last) break;
	}
	m_renderer->setScissor(false);
	delete [] unicode;

	if (align == etxAlignRight)
	{
		return tVertex2(bw+xpos, carety+ypos -(float)m_base_pos*scale);
	} else if (align == etxAlignLeftRight)
	{
		if (caretx > (float)bw)
			return tVertex2(bw+xpos, carety+ypos -(float)m_base_pos*scale);
		else
			return tVertex2(caretx+xpos, carety+ypos -(float)m_base_pos*scale);
	} else
	{
		return tVertex2(caretx+xpos, carety+ypos -(float)m_base_pos*scale);
	}
}

#endif //_IGUIFRAMEFONT_H_
