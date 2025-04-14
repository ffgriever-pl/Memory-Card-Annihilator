#ifndef _IGUIFRAMEINCLUDES_H_
#define _IGUIFRAMEINCLUDES_H_

#include <tamtypes.h>


struct tVertex2
{
	tVertex2() {}
	tVertex2(float cx, float cy) {x = cx; y = cy; }
	float x;
	float y;
};
struct tVertex3
{
	tVertex3() {}
	tVertex3(float cx, float cy, float cz) {x = cx; y = cy; z = cz;}
	float x;
	float y;
	float z;
};
struct tVertex4
{
	tVertex4() {}
	tVertex4(float cx, float cy, float cz, float cw) {x = cx; y = cy; z = cz; w = cw;}
	float w;
	float z;
	float y;
	float x;
} __attribute__((aligned(16)));
struct tRGBColor
{
	tRGBColor() {}
	tRGBColor(u8 cr, u8 cg, u8 cb) {r = cr; g = cg; b = cb;}
	u8 r;
	u8 g;
	u8 b;
};

#endif //_IGUIFRAMEINCLUDES_H_
