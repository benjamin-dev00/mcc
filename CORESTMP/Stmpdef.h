//===================================================================
//
//Zheng zhikai	97-10-8
//===================================================================

#ifndef __STMPDEF_H
#define __STMPDEF_H

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////
//stamps

#define IDS_COMBINE			0X01030100L
#define IDS_BUTTON			0X01040100L
#define IDS_PICTURE			0X01050100L

#define IDS_RECT			0X02010100L
#define IDS_RECTFRAME		0X02011100L

#define IDS_ELLIPSE			0X02020100L
#define IDS_ELLIPSEFRAME	0X02021100L

#define IDS_ROUNDRECT		0X02030100L
#define IDS_ROUNDRECTFRAME	0X02031100L

#define IDS_ARC				0X02040100L

#define IDS_LINE			0X02100100L
#define IDS_MULTILINE		0X02210100L
#define IDS_POLYGON			0X02310100L

#define IDS_STRING			0X02200100L

#define IDS_SLIDE			0x02300200L
#define IDS_FILL			0x02400100L

#define IDS_TREND			0X04010100L
#define IDS_HISTREND		0x04020100L

//_DLLEXP BOOL _ZZKDECL ProvideStamps(void);

#define DEF_DEF( _name_ )																			\
_DLLEXP BOOL  _ZZKDECL _name_##Paint(WRD wFTU_, HDC hdc_);											\
_DLLEXP DWORD _ZZKDECL _name_##Param(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_);				\
_DLLEXP DWORD _ZZKDECL _name_##Build(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_);

DEF_DEF( Combine )
DEF_DEF( Rect )
DEF_DEF( RectFrame )
DEF_DEF( Ellipse )
DEF_DEF( EllipseFrame )
DEF_DEF( RndRct )
DEF_DEF( RndRctFrame )
DEF_DEF( Arc )
DEF_DEF( Line )
DEF_DEF( Trend )
DEF_DEF( String )
DEF_DEF( MultiLine )
DEF_DEF( Polygon )
DEF_DEF( Button )
DEF_DEF( Picture )
DEF_DEF( Slide )
DEF_DEF( Fill )
DEF_DEF( HisTrend )

#ifdef  __cplusplus
}
#endif

#endif	//__STMPDEF_H
