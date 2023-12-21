//===================================================================
//
//Zheng zhikai	96-12-26
//===================================================================

#ifndef __CGDI_H
#define __CGDI_H

//define colors and fill mode
//0x00-00-00-00

/////////////////////////////////////////////////////////
//Define brush, extend from RGB color                  //
/////////////////////////////////////////////////////////

//following definitions define the highest 8-bit(H8B),
//the lower 24-bit(L24B) depend on the each definitions

#define CLR_MASK	0xFF000000L

//CLR_RGB causes that solid brush with the rgb color is created
#define	CLR_RGB		0x00000000L		//H8B == 0x00; L24B: rgb value

//CLR_SYS: as same as GetSysColor
#define	CLR_SYS		0x01000000L		//H8B == 0x01; L24B: index defined in GetSysColor  windows.h

//CLR_CORE: pre-defined color used in this software
//there is only NULL color, i.e., transparent color, now
#define CLR_CORE	0x02000000L		//H8B == 0x02;
#define CCLR_NULL	(CLR_CORE|0x0L)

#define CCLR_SCROLLBAR         (COLOR_SCROLLBAR|CLR_SYS)
#define CCLR_BACKGROUND        (COLOR_BACKGROUND|CLR_SYS)
#define CCLR_ACTIVECAPTION     (COLOR_ACTIVECAPTION|CLR_SYS)
#define CCLR_INACTIVECAPTION   (COLOR_INACTIVECAPTION|CLR_SYS)
#define CCLR_MENU              (COLOR_MENU|CLR_SYS)
#define CCLR_WINDOW            (COLOR_WINDOW|CLR_SYS)
#define CCLR_WINDOWFRAME       (COLOR_WINDOWFRAME|CLR_SYS)
#define CCLR_MENUTEXT          (COLOR_MENUTEXT|CLR_SYS)
#define CCLR_WINDOWTEXT        (COLOR_WINDOWTEXT|CLR_SYS)
#define CCLR_CAPTIONTEXT       (COLOR_CAPTIONTEXT|CLR_SYS)
#define CCLR_ACTIVEBORDER      (COLOR_ACTIVEBORDER|CLR_SYS)
#define CCLR_INACTIVEBORDER    (COLOR_INACTIVEBORDER|CLR_SYS)
#define CCLR_APPWORKSPACE      (COLOR_APPWORKSPACE|CLR_SYS)
#define CCLR_HIGHLIGHT         (COLOR_HIGHLIGHT|CLR_SYS)
#define CCLR_HIGHLIGHTTEXT     (COLOR_HIGHLIGHTTEXT|CLR_SYS)
#define CCLR_BTNFACE           (COLOR_BTNFACE|CLR_SYS)
#define CCLR_BTNSHADOW         (COLOR_BTNSHADOW|CLR_SYS)
#define CCLR_GRAYTEXT          (COLOR_GRAYTEXT|CLR_SYS)
#define CCLR_BTNTEXT           (COLOR_BTNTEXT|CLR_SYS)
#define CCLR_INACTIVECAPTIONTEXT (COLOR_INACTIVECAPTIONTEXT|CLR_SYS)
#define CCLR_BTNHIGHLIGHT      (COLOR_BTNHIGHLIGHT|CLR_SYS)
#define CCLR_3DDKSHADOW        (COLOR_3DDKSHADOW|CLR_SYS)
#define CCLR_3DLIGHT           (COLOR_3DLIGHT|CLR_SYS)
#define CCLR_INFOTEXT          (COLOR_INFOTEXT|CLR_SYS)
#define CCLR_INFOBK            (COLOR_INFOBK|CLR_SYS)

//#define CCLR_3DLIGHT
#define CCLR_3D					CCLR_BTNFACE
#define CCLR_3DSHADOW			CCLR_BTNSHADOW			
#define CCLR_3DTEXT				CCLR_BTNTEXT

#define H8B(_clr_)	((WORD)(((DWORD)(_clr_) >> 24) & 0x00FF))
#define L24B(_clr_)	(_clr_ & 0x00FFFFFFL)

/////////////////////////////////////////////////////////
//Define the line style ( border style )               //
/////////////////////////////////////////////////////////

//extend style from microsoft line style
//my line style begin at 1000
#define ZLS_BEGIN		1000

//Dip line and Dump line:
//Description:
//	this style is only for vertical line or horizontal line;
//	If this line style is used for drawing a rectange, the
//	join between the line is embellished, the result is as
//	same drawing a box with ZLS_SUNKEN/ZLS_RAISED in a box
//	with ZLS_RAISED/ZLS_SUNKEN;
//Width:
//	this style line uses the 32-bit width: the low 16-bit
//	specifies the depth/height, this depth/height 0 is equal
//	with depth/height 1, the high 16-bit value specifies the
//	gap width between the two sides;
//Color:
//	The color specifies the color for the gap between the
//	two sides;
//Note:
//	If this style is not used for vertical line nor horizontal
//	line, the style treat as normal line style: PS_SOLID for
//	the non-closed shapes, PS_INSIDEFRAME for the closed shapes;
//	the 32bit width casts to the POINT structure in LOGPEN; ref
//	to CreatePenIndirect; the color is normally semantic;
#define ZLS_DIP		(ZLS_BEGIN + 1)
#define ZLS_BUMP	(ZLS_BEGIN + 2)

//Raised/Sunken line:
//Description:
//	Draw a raised/retuse region, only for rectangle;
//Width:
//	it uses integer type parameter, which specifies the depth
//	or height of the region;
//Color:
//	The color is omitted;
//Note:
//	If this style is not used for vertical line nor horizontal
//	line, the style treat as normal line style: PS_SOLIDE for
//	the non-closed shapes, PS_INSIDEFRAME for the closed shapes;
//	the color is normally semantic;
#define ZLS_RAISED	(ZLS_BEGIN + 3)
#define ZLS_SUNKEN	(ZLS_BEGIN + 4)

_DLLEXP HPEN _ZZKDECL CoreCreatePen( int fnPenStyle, int nWidth, CCLR clr_ );
_DLLEXP BOOL _ZZKDECL CoreDeletePen( HPEN hbr_ );
_DLLEXP HBRUSH _ZZKDECL CoreCreateBrush( CCLR clr_ );
_DLLEXP BOOL _ZZKDECL CoreDeleteBrush( HBRUSH hbr_ );
_DLLEXP DWORD _ZZKDECL CoreGetPenWidth( int nPenStyle_, CCLR clrPen_, DWORD width_org_ );
_DLLEXP BOOL _ZZKDECL CoreFillRect( HDC hdc_, LPRECT lprc_, CCLR clrFill_ );
_DLLEXP BOOL _ZZKDECL CoreDrawBox( HDC hdc_, LPRECT lprc_, int nPenStyle_, DWORD dwPenWidth_, CCLR clr_ );
_DLLEXP BOOL _ZZKDECL IsTransparentColor( CCLR clr_ );
_DLLEXP BOOL _ZZKDECL CclrGetRGB( CCLR clr_, COLORREF FAR* lpRgb_ );

#endif		//__CGDI_H
