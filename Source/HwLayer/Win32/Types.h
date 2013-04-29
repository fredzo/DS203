#ifndef __TYPES_H__
#define __TYPES_H__

#ifndef _WIN32
#error Include the folder "HwLayer/Win32" only for desktop application compilation
#endif

#include <stdint.h>
#include <windows.h>
#include <crtdbg.h>
#include <stdio.h>
#include <math.h>

typedef unsigned char ui8;
typedef unsigned char ui8;
typedef unsigned short u16;
typedef signed short si16;
typedef signed char si8;
typedef unsigned short ui16;
typedef unsigned long ui32;
typedef signed long si32;
typedef signed short si16;

#define RGB565RGB(r, g, b) (((r)>>3)|(((g)>>2)<<5)|(((b)>>3)<<11))
#define Get565R(rgb) (((rgb)&0x1f)<<3)
#define Get565G(rgb) ((((rgb)>>5)&0x3f)<<2)
#define Get565B(rgb) ((((rgb)>>11)&0x1f)<<3)
#define GetHtmlR(rgb) ((rgb) >> 16)
#define GetHtmlG(rgb) (((rgb) >> 8)&0xff)
#define GetHtmlB(rgb) ((rgb)&0xff)
#define GetColorR(rgb) ((rgb) & 0xff)
#define GetColorG(rgb) (((rgb) >> 8)&0xff)
#define GetColorB(rgb) (((rgb) >> 16)&0xff)

#define RGB565(rgb) RGB565RGB( GetHtmlR(0x##rgb), GetHtmlG(0x##rgb), GetHtmlB(0x##rgb))
#define COUNT(arr) (int)(sizeof(arr)/sizeof(arr[0]))
#define RGBTRANS (RGB565(ff00ff)-1)

#define RGB32(r,g,b) ((r) | ((g)<<8) | ((b)<<16))

/*
#ifndef min 
#	define min(a, b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#	define max(a, b) ((a)>(b)?(a):(b))
#endif
#ifndef abs
#	define abs(a) ((a)>0?(a):(-(a)))
#endif
*/

#define DecEnum(e) *((ui8*)&(e))-=1
#define IncEnum(e) *((ui8*)&(e))+=1

#define NATIVEPTR ULONGLONG
#define NATIVEENUM LONG

#define MAXUINT     ((UINT)~((UINT)0))
#define MAXINT      ((INT)(MAXUINT >> 1))
#define MININT      ((INT)~MAXINT)

#define ToWord(a, b) (ui16)(((a)<<8)|(b))
#define ToDword(a, b, c, d) (ui32)((ToWord(d, c)<<16)|ToWord(b,a))

#define _ASSERTW(a) _ASSERT(a)
#define _ASSERT_VALID(a) if(!(a)) { _ASSERT(#a); }
#define LINKERSECTION(sec) 
#define __asm__(x) 
//__attribute__((section(".extra")))
/*
#ifndef _WIN32
__attribute__((section(".extra")))
#endif
*/

struct FILEINFO {
	enum {
		SectorSize = 512
	};
	/*
	ui16 pCluster[3];
	ui32 pDirAddr[1];
	*/
	ui8 nMode;
	ui8 nSectors;
	FILE *f;
};

#ifdef _DLLEXPORT
#	define DLLAPI __declspec(dllexport)
#else
#	define DLLAPI
#endif

#endif