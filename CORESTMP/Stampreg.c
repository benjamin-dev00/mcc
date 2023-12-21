//===================================================================
//
//Zheng zhikai	95-4-22
//===================================================================

#include "prehead.h"

#include "core.h"
#include "cface.h"
#include "cstamp.h"
#include "corerw.h"

#include "stmpdef.h"

static STU		sSTUs[] = {
	{"Combine",	IDS_COMBINE, CombinePaint, CombineParam, CombineBuild},
	{"Line", IDS_LINE, LinePaint, LineParam, LineBuild},
	{"Rect", IDS_RECT, RectPaint, RectParam, RectBuild},
	{"RectFrame", IDS_RECTFRAME, RectFramePaint, RectFrameParam, RectFrameBuild},
	{"RoundRect", IDS_ROUNDRECT, RndRctPaint, RndRctParam, RndRctBuild},
	{"RoundRectFrame", IDS_ROUNDRECTFRAME, RndRctFramePaint, RndRctFrameParam, RndRctFrameBuild},
	{"Ellipse", IDS_ELLIPSE, EllipsePaint, EllipseParam, EllipseBuild},
	{"EllipseFrame", IDS_ELLIPSEFRAME, EllipseFramePaint, EllipseFrameParam, EllipseFrameBuild},
	{"Arc", IDS_ARC, ArcPaint, ArcParam, ArcBuild},
	{"Trend", IDS_TREND, TrendPaint, TrendParam, TrendBuild},
	{"Text", IDS_STRING, StringPaint, StringParam, StringBuild},
	{"MultiLine", IDS_MULTILINE, MultiLinePaint, MultiLineParam, MultiLineBuild },
	{"Polygon", IDS_POLYGON, PolygonPaint, PolygonParam, PolygonBuild },
	{"Button", IDS_BUTTON, ButtonPaint, ButtonParam, ButtonBuild },
	{"Picture", IDS_PICTURE, PicturePaint, PictureParam, PictureBuild },
	{"Slide", IDS_SLIDE, SlidePaint, SlideParam, SlideBuild },
	{"Fill", IDS_FILL, FillPaint, FillParam, FillBuild },
	{"HisTrend", IDS_HISTREND, HisTrendPaint, HisTrendParam, HisTrendBuild },
};

//...................................................................
//Description:
//...................................................................
BOOL ProvideStamps()
{
	int		_i;

	for(_i=0; _i < sizeof(sSTUs)/sizeof(STU); _i++)
		if(!RegisterStamp( &sSTUs[_i]) )
		{
			MessageBeep( MB_ICONSTOP );
			MessageBeep( MB_ICONSTOP );
			return FALSE;
		}

	return TRUE;
}

//...................................................................
//Description:
//...................................................................
BOOL ReleaseStamps()
{
	int		_i;

	for(_i=0; _i < sizeof(sSTUs)/sizeof(STU); _i++)
		UnregisterStamp( sSTUs[_i].wID );

	return TRUE;
}

BOOL AccessStampDefaultParam( LPCSTR lpszDefName_, BOOL bSave_ )
{
	BOOL	_ret = TRUE;
	int		_wrd;
	char	_szBuffer[_MAX_PATH + LENGTH_NAME_STAMP + 2];

	for(_wrd=0; _wrd < sizeof(sSTUs)/sizeof(STU); _wrd++)
	{
		strcpy( _szBuffer, lpszDefName_ );
		strcpy( _szBuffer + strlen(_szBuffer) + 1, sSTUs[_wrd].szName );
		if( !(*sSTUs[_wrd].lpfnParam)( bSave_? PIC_SAVEDEFAULT: PIC_LOADDEFAULT, 
										0, 0, (LPARAM)_szBuffer ) )
		{
			LOGWARN2( "CORESTMP.DLL %s defParam failed: %s", bSave_?"Save": "Load", sSTUs[_wrd].szName );
			_ret = FALSE;
		}
	}

	return _ret;
}

//...................................................................
//Description:
//...................................................................
BOOL StampsOnLoad()
{
	int		_i;

	for(_i=0; _i < sizeof(sSTUs)/sizeof(STU); _i++)
		if( !( *sSTUs[_i].lpfnParam)( PIC_ONLOAD, 0, 0, 0) )
			return FALSE;

	return TRUE;
}

//...................................................................
//Description:
//...................................................................
BOOL StampsOnExit()
{
	int		_i;

	for(_i=0; _i < sizeof(sSTUs)/sizeof(STU); _i++)
		if( !( *sSTUs[_i].lpfnParam)( PIC_ONEXIT, 0, 0, 0) )
			return FALSE;

	return TRUE;
}


