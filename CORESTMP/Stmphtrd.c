//===================================================================
//
//Zheng zhikai	97-9-6
//===================================================================
#include "prehead.h"

#include "coreall.h"

#include "stmpdef.h"
#include "form.h"
#include <dos.h>
#include <math.h>
#include <time.h>
#include "StmpRes.h"

#pragma warning( disable : 4244 )	//'conversion' conversion from 'type1' to 'type2', possible loss of data

//the ID of form window in history trend configure dialog
#define IDC_PENFORM 	0x223

//the trend style:
#define STYLE_LEFTLABEL 	   0x00000008L
#define STYLE_TOPLABEL		   0x00000004L
#define STYLE_RIGHTLABEL	   0x00000002L
#define STYLE_BOTTOMLABEL	   0x00000001L

//define the gip between the trend chart and lebal
#define _GIP_X					(4)
#define _GIP_Y					(4)

extern HINSTANCE	ghInst;

BOOL _PenFormData( LPCELLINFO lpCellInfo_ );

_DLLEXP BOOL _DLLFUNC _PenFormGetData( LONG lData_, LPSTR lpvalstr_, DWORD size_, UINT row_, UINT col_ );
_DLLEXP BOOL _DLLFUNC _PenFormSetData( LONG lData_, LPSTR lpvalstr_, UINT row_, UINT col_ );
_DLLEXP BOOL _DLLFUNC _PenFormGetRowTitle( LONG lData_, LPSTR szBuffer_, DWORD dwSize_, UINT wRow_);
_DLLEXP BOOL _DLLFUNC _PenFormGetColTitle( LONG lData_, LPSTR szBuffer_, DWORD dwSize_, UINT wCol_);

//static const unsigned long msInDay = (24L*60L*60L*1000L);
//static const unsigned long msInHour = (60L*60L*1000L);

//BOOL CoreTime2tm( CORETIME ct_, struct tm _FAR* lptm_ );

//static LPPU	near _get_termin_pu(LPPU lpPU_);
static WORD _near _TrendRunDirty( WRD wFTU_, WORD dirty_ );
static BOOL _near UpdateHisData(WRD wFTU_, BOOL bUpdate_);
static BOOL _near _RedrawTrend(WRD wFTU_, HDC hdc_);
static BOOL _near _CalculateChartRect( HDC hdc_, LPPU, LPRECT lprc_, LPRECT lprcChart_ );
static void _near CoreTimeToAscii( time_t tt_, LPSTR lpszDate_, LPSTR lpszTime_ );

static void _near _TrendDrawHrzLines( HDC hdc_, LPPU lpPrm_, LPRECT lprcChart_ );
static void _near _TrendDrawVerLines(HDC	hdc_,
									 LPPU	lpPrm_, 	//beginning of parameters of the trend
									 LPRECT lprc_); 	//rcChart_
static void _near _TrendDrawHrzLabel(HDC		hdc_,
									 LPPU		lpPrm_, 		//beginning of parameters of the trend
									 DWORD		dwStyle_,		//the style of the trend
									 CORETIME	ctBeginTime_,
									 CORETIME	ctEndTime_,
									 LPRECT 	lprcChart_, 		//the rect of the chart of the trend: rcChart_
									 LPRECT 	lprcTrend_);		//the rect of the trend
//static void _near msToAsciiTime(long ms, LPSTR lpszBuffer_ );

#ifdef _WIN32
BOOL CALLBACK
#else
BOOL CALLBACK _export 
#endif
ConfigDlgProc( HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_ );

enum _trend_param
{
	HISTRD_BORDERSTYLE = FACE_FIRSTPARAM,	  //border line style
	HISTRD_BORDERWIDTH, 	//border line width
	HISTRD_BORDERCLR,		//border line color
	HISTRD_BACKCLR, 		//back color of chart

	HISTRD_LEFTLABEL,
	HISTRD_TOPLABEL,
	HISTRD_RIGHTLABEL,
	HISTRD_BOTTOMLABEL,
	HISTRD_MIN, 			//min value in vertical axes
	HISTRD_MAX, 			//max value in vertical axes

	HISTRD_HRZLABELCLR, 	//horizontal label color
	HISTRD_HRZLABELDIV, 	//horizontal label divisor

	HISTRD_VERLABELCLR, 	//vertical label color
	HISTRD_VERLABELDIV, 	//vertical label divisor

	HISTRD_HRZMAJLNSTYLE,	//horizontal major line style
	HISTRD_HRZMAJLNWIDTH,	//horizontal major line width
	HISTRD_HRZMAJLNCLR, 	//horizontal major line color
	HISTRD_HRZMAJLNDIV, 	//horizontal major line divisor

	HISTRD_HRZMINLNSTYLE,	//horizontal minor line style
	HISTRD_HRZMINLNWIDTH,	//horizontal minor line width
	HISTRD_HRZMINLNCLR, 	//horizontal minor line color
	HISTRD_HRZMINLNDIV, 	//horizontal minor line divisor

	HISTRD_VERMAJLNSTYLE,	//vertical major line style
	HISTRD_VERMAJLNWIDTH,	//vertical major line width
	HISTRD_VERMAJLNCLR, 	//vertical major line color
	HISTRD_VERMAJLNDIV, 	//vertical major line divisor

	HISTRD_VERMINLNSTYLE,	//vertical minor line style
	HISTRD_VERMINLNWIDTH,	//vertical minor line width
	HISTRD_VERMINLNCLR, 	//vertical minor line color
	HISTRD_VERMINLNDIV, 	//vertical minor line divisor

	HISTRD_BEGINDATETIME,	//begin date/time
	HISTRD_ENDDATETIME, 	//end date/time
};

///////
#define 	HISTRD_LEFTLABEL_PDT		PDT_BOOL
#define 	HISTRD_TOPLABEL_PDT 		PDT_BOOL
#define 	HISTRD_RIGHTLABEL_PDT		PDT_BOOL
#define 	HISTRD_BOTTOMLABEL_PDT		PDT_BOOL
#define 	HISTRD_MIN_PDT				PDT_REAL
#define 	HISTRD_MAX_PDT				PDT_REAL
#define 	HISTRD_BACKCLR_PDT			PDT_COLORREF
#define 	HISTRD_BORDERSTYLE_PDT		PDT_int
#define 	HISTRD_BORDERWIDTH_PDT		PDT_DWORD
#define 	HISTRD_BORDERCLR_PDT		PDT_COLORREF
#define 	HISTRD_HRZLABELCLR_PDT		PDT_COLORREF
#define 	HISTRD_VERLABELCLR_PDT		PDT_COLORREF
#define 	HISTRD_HRZMAJLNSTYLE_PDT	PDT_int
#define 	HISTRD_HRZMAJLNWIDTH_PDT	PDT_int
#define 	HISTRD_HRZMAJLNCLR_PDT		PDT_COLORREF
#define 	HISTRD_HRZMINLNSTYLE_PDT	PDT_int
#define 	HISTRD_HRZMINLNWIDTH_PDT	PDT_int
#define 	HISTRD_HRZMINLNCLR_PDT		PDT_COLORREF
#define 	HISTRD_VERMAJLNSTYLE_PDT	PDT_int
#define 	HISTRD_VERMAJLNWIDTH_PDT	PDT_int
#define 	HISTRD_VERMAJLNCLR_PDT		PDT_COLORREF
#define 	HISTRD_VERMINLNSTYLE_PDT	PDT_int
#define 	HISTRD_VERMINLNWIDTH_PDT	PDT_int
#define 	HISTRD_VERMINLNCLR_PDT		PDT_COLORREF
#define 	HISTRD_HRZLABELDIV_PDT		PDT_WORD
#define 	HISTRD_VERLABELDIV_PDT		PDT_WORD
#define 	HISTRD_HRZMAJLNDIV_PDT		PDT_WORD
#define 	HISTRD_HRZMINLNDIV_PDT		PDT_WORD
#define 	HISTRD_VERMAJLNDIV_PDT		PDT_WORD
#define 	HISTRD_VERMINLNDIV_PDT		PDT_WORD

#define 	HISTRD_BEGINDATETIME_PDT	PDT_CORETIME
#define 	HISTRD_ENDDATETIME_PDT		PDT_CORETIME

#define 	HISTRD_SAMPLEINTERVAL_PDT	PDT_DWORD
#define 	HISTRD_TOTALTIME_PDT		PDT_DWORD
#define 	HISTRD_SC_PDT				PDT_DWORD
#define 	HISTRD_SCINIT_PDT			PDT_DWORD
#define 	HISTRD_TOTALITEM_PDT		PDT_WORD
#define 	HISTRD_VALIDITEM_PDT		PDT_WORD
#define 	HISTRD_ADDING_PDT			PDT_WORD

//Trend parameter order - varible partion
#define 	HISTRD_DATASOURCE		0
#define 	HISTRD_DATAMIN			1
#define 	HISTRD_DATAMAX			2
#define 	HISTRD_DATACACHE		3
#define 	HISTRD_BOOLVAR			4
#define 	HISTRD_PENSTYLE 		5
#define 	HISTRD_PENWIDTH 		6
#define 	HISTRD_PENCLR			7

//Trend parameter type - varible partion
#define 	HISTRD_DATASOURCE_PDT	PDT_REAL
#define 	HISTRD_DATAMIN_PDT		PDT_REAL
#define 	HISTRD_DATAMAX_PDT		PDT_REAL
#define 	HISTRD_DATACACHE_PDT	PDT_LPCOBJ
#define 	HISTRD_BOOLVAR_PDT		PDT_BOOL
#define 	HISTRD_PENSTYLE_PDT 	PDT_int
#define 	HISTRD_PENWIDTH_PDT 	PDT_int
#define 	HISTRD_PENCLR_PDT		PDT_COLORREF

static PDU	sfixPDU[] = {
	_PDU_DEFAULT,

	_ELE_PDU(HISTRD_BORDERSTYLE,	"BorderStyle",		VSOF_int_RECTBS,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_BORDERWIDTH,	"BorderWidth",		VSOF_DWORD_BWIDTH,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_BORDERCLR,		"BorderColor",		VSOF_DWORD_CLR, 	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_BACKCLR,		"BackColor",		VSOF_DWORD_CLR, 	PU_NORMAL, PRI_ALL ),

	_ELE_PDU(HISTRD_LEFTLABEL,		"LeftLabel",		VSOF_BOOL_ONOFF,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_TOPLABEL,		"TopLabel", 		VSOF_BOOL_ONOFF,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_RIGHTLABEL, 	"RightLabel",		VSOF_BOOL_ONOFF,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_BOTTOMLABEL,	"BottomLabel",		VSOF_BOOL_ONOFF,	PU_NORMAL, PRI_ALL ),

	_ELE_PDU(HISTRD_MIN,			"Min",				VSOF_REAL,			PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_MAX,			"Max",				VSOF_REAL,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(HISTRD_HRZLABELCLR,	"HrzLabelColor",	VSOF_DWORD_CLR, 	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_HRZLABELDIV,	"HrzLabelDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(HISTRD_VERLABELCLR,	"VerLabelColor",	VSOF_DWORD_CLR, 	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_VERLABELDIV,	"VerLabelDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(HISTRD_HRZMAJLNSTYLE,	"HrzMajLnStyle",	VSOF_int_LNSTYLE,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_HRZMAJLNWIDTH,	"HrzMajLnWidth",	VSOF_int,			PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_HRZMAJLNCLR,	"HrzMajLnColor",	VSOF_DWORD_CLR, 	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_HRZMAJLNDIV,	"HrzMajLnDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(HISTRD_HRZMINLNSTYLE,	"HrzMinLnStyle",	VSOF_int_LNSTYLE,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_HRZMINLNWIDTH,	"HrzMinLnWidth",	VSOF_int,			PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_HRZMINLNCLR,	"HrzMinLnColor",	VSOF_DWORD_CLR, 	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_HRZMINLNDIV,	"HrzMinLnDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(HISTRD_VERMAJLNSTYLE,	"VerMajLnStyle",	VSOF_int_LNSTYLE,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_VERMAJLNWIDTH,	"VerMajLnWidth",	VSOF_int,			PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_VERMAJLNCLR,	"VerMajLnColor",	VSOF_DWORD_CLR, 	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_VERMAJLNDIV,	"VerMajLnDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(HISTRD_VERMINLNSTYLE,	"VerMinLnStyle",	VSOF_int_LNSTYLE,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_VERMINLNWIDTH,	"VerMinLnWidth",	VSOF_int,			PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_VERMINLNCLR,	"VerMinLnColor",	VSOF_DWORD_CLR, 	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_VERMINLNDIV,	"VerMinLnDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(HISTRD_BEGINDATETIME,	"BeginDateTime",	VSOF_CORETIME,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_ENDDATETIME,	"EndDateTime",		VSOF_CORETIME,		PU_NORMAL, PRI_ALL ),
};

static PDU	svarPDU[] = {
	_ELE_PDU(HISTRD_DATASOURCE, 	"DataSource",	VSOF_REAL,		PU_NORMAL, PRI_ALL|PDUF_NODEFAULT ),
	_ELE_PDU(HISTRD_DATAMIN,		"DataMin",		VSOF_REAL,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_DATAMAX,		"DataMax",		VSOF_REAL,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_DATACACHE,		"DataCache",	VSOF_LPCOBJ,	PU_NIV,    PRI_DEV|PDUF_NODEFAULT ),
	_ELE_PDU(HISTRD_BOOLVAR,		"BoolVar",		VSOF_BOOL,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_PENSTYLE,		"PenStyle", 	VSOF_int,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_PENWIDTH,		"PenWidth", 	VSOF_int,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(HISTRD_PENCLR, 		"PenColor", 	VSOF_DWORD_CLR, PU_NORMAL, PRI_ALL ),
};

const WORD NUM_VAR = (sizeof(svarPDU)/sizeof(svarPDU[0]));
const WORD NUM_FIX = (sizeof(sfixPDU)/sizeof(sfixPDU[0]));

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC HisTrendPaint(WRD wFTU_, HDC hdc_)
{
	_RedrawTrend( wFTU_, hdc_ );

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC HisTrendParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static VPSTRUCT _sVPD = {
		1,			//the default number of var parameters groups
		0,			//the low bound of var counter
		(sizeof(sfixPDU)/sizeof(sfixPDU[0])),
		&( sfixPDU[0] ),
		(sizeof(svarPDU)/sizeof(svarPDU[0])),
		&( svarPDU[0] )
	};

	return DefProcessVarPIC(wCmd_, wFTU_, wNo_, lParam_, TBX_FT, IDS_HISTREND, &_sVPD);
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC HisTrendBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT _lpFinit = (LPFINIT)lParam_;
			LPPU	_lpPrm, _lpPrmVar;
			DWORD	_defStyle;
			WORD	_wNumParam;
			div_t	_r_div;
			int 	_i;

			_lpPrm	= FaceGetParameter( wFTU_ );
			_wNumParam = FaceGetNumParam( wFTU_ );
			_defStyle = STYLE_BOTTOMLABEL|STYLE_LEFTLABEL;

			MFPUWRITE( _lpPrm, HISTRD_LEFTLABEL, BOOL,	TRUE );
			MFPUWRITE( _lpPrm, HISTRD_TOPLABEL, BOOL, FALSE );
			MFPUWRITE( _lpPrm, HISTRD_RIGHTLABEL, BOOL, FALSE );
			MFPUWRITE( _lpPrm, HISTRD_BOTTOMLABEL, BOOL, TRUE );

			//MFPUWRITE(_lpPrm, HISTRD_STYLE,	  DWORD,	  _defStyle);
			MFPUWRITE(_lpPrm, HISTRD_MIN,		REAL,		0);
			MFPUWRITE(_lpPrm, HISTRD_MAX,		REAL,		100);

			MFPUWRITE(_lpPrm, HISTRD_BORDERSTYLE,	int,		_lpFinit->border_pen.lopnStyle );
			MFPUWRITE(_lpPrm, HISTRD_BORDERWIDTH,	DWORD,		_lpFinit->border_pen.lopnWidth.x );
			MFPUWRITE(_lpPrm, HISTRD_BORDERCLR, 	COLORREF,	_lpFinit->border_pen.lopnColor );
			MFPUWRITE(_lpPrm, HISTRD_BACKCLR,		COLORREF,	_lpFinit->back_clr );

			MFPUWRITE(_lpPrm, HISTRD_HRZMAJLNSTYLE, int,		PS_SOLID );
			MFPUWRITE(_lpPrm, HISTRD_HRZMAJLNWIDTH, int,		1 );
			MFPUWRITE(_lpPrm, HISTRD_HRZMAJLNCLR,	COLORREF,	RGB(0,127,0) );

			MFPUWRITE(_lpPrm, HISTRD_HRZMINLNSTYLE, int,		PS_DOT);
			MFPUWRITE(_lpPrm, HISTRD_HRZMINLNWIDTH, int,		1);
			MFPUWRITE(_lpPrm, HISTRD_HRZMINLNCLR,	COLORREF,	RGB(0,127,0) );

			MFPUWRITE(_lpPrm, HISTRD_VERMAJLNSTYLE, int,		PS_SOLID );
			MFPUWRITE(_lpPrm, HISTRD_VERMAJLNWIDTH, int,		1 );
			MFPUWRITE(_lpPrm, HISTRD_VERMAJLNCLR,	COLORREF,	RGB(0,127,0) );

			MFPUWRITE(_lpPrm, HISTRD_VERMINLNSTYLE, int,		PS_DOT );
			MFPUWRITE(_lpPrm, HISTRD_VERMINLNWIDTH, int,		1 );
			MFPUWRITE(_lpPrm, HISTRD_VERMINLNCLR,	COLORREF,	RGB(0,127,0) );

			MFPUWRITE(_lpPrm, HISTRD_HRZLABELCLR,	COLORREF,	_lpFinit->text_clr);
			MFPUWRITE(_lpPrm, HISTRD_VERLABELCLR,	COLORREF,	_lpFinit->text_clr);

			MFPUWRITE(_lpPrm, HISTRD_VERLABELDIV,	WORD,		2);
			MFPUWRITE(_lpPrm, HISTRD_HRZLABELDIV,	WORD,		2);
			MFPUWRITE(_lpPrm, HISTRD_HRZMAJLNDIV,	WORD,		2);
			MFPUWRITE(_lpPrm, HISTRD_HRZMINLNDIV,	WORD,		5);
			MFPUWRITE(_lpPrm, HISTRD_VERMAJLNDIV,	WORD,		2);
			MFPUWRITE(_lpPrm, HISTRD_VERMINLNDIV,	WORD,		3);

			//calculate the number of data source depend on the number of parameter
			_r_div = div( _wNumParam - NUM_FIX, NUM_VAR );
			CORE_ASSERT( _r_div.rem == 0 );

			for(_i=0, _lpPrmVar = _lpPrm + NUM_FIX;
				_i<_r_div.quot;
				_i++, _lpPrmVar += NUM_VAR )
			{
				MFPUWRITE(_lpPrmVar, HISTRD_DATASOURCE, REAL, 0);

				MFPUWRITE(_lpPrmVar, HISTRD_DATAMIN, REAL, 0);
				MFPUWRITE(_lpPrmVar, HISTRD_DATAMAX, REAL, 100);

				MFPUWRITE(_lpPrmVar, HISTRD_DATACACHE, LPCOBJ, NULL);
				MFPUWRITE(_lpPrmVar, HISTRD_PENSTYLE,  int, PS_SOLID );
				MFPUWRITE(_lpPrmVar, HISTRD_PENWIDTH,  int, 1 );
				MFPUWRITE(_lpPrmVar, HISTRD_PENCLR,   COLORREF, RGB(0,255,0) );
			}
			break;
		}

		case SCMD_RUNTIME:
		{
			LPFIA			_lpFia = (LPFIA)lParam_;

			if( _lpFia->MsgCode & MSG_TRIGGER )
				DialogBoxParam( ghInst,
								MAKEINTRESOURCE(DLG_HISTREND_CONFIG),
								hwnd_,
								ConfigDlgProc,
								(LPARAM)wFTU_ );

			return FALSE;
		}

		/*
		case SCMD_GETINVALIDRGN:
		{
			//here, we calculate the dirty PU by self because the data
			//change of data-source should not cause the whole trend to redraw
			wParam_ = _TrendRunDirty( wFTU_, wParam_ );

			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
		}
		*/

		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

	return TRUE;
}

//...................................................................
//...................................................................
static BOOL _near _RedrawTrend(WRD wFTU_, HDC hdc_ )
{
	RECT		_rc, _rcChart, _rcBu;
	TEXTMETRIC	_tm;
	BOOL		_bLeftLabel, _bTopLabel, _bRightLabel, _bBottomLabel;
	DWORD		_dwStyle;
	DWORD		_dwBorderWidth;
	float		_min, _max, _delta_value, _vl;
	WORD		_wNumParam;
	WORD		_wVerLabelDiv;
	CCLR		_clrBk, _clrPen, _clrText;
	COLORREF	_rgbText;
	int 		_nPenWidth, _nPenStyle;
	HPEN		_hPen;
	HBRUSH		_hBrush;
	int 		_i;
	LPPU		_lpPrm, _lpPrmVar;
	//WORD		_wMajorDiv, _wMinorDiv;
	float		_y, _delta;
	char		_szBuffer[32];

	WORD		_nNumPen;
	float		_factor_y, _offset_y, _factor_x, _offset_x;
	float		_data_min, _data_max;	//each pen's max and min
	BOOL		_retClr;	//return value by CclrGetRGB

	div_t		_divt;
	HDC 		_hdcDraw;

#ifdef _USE_OWN_HDC
	HDC 		_hdcMem;
	HBITMAP 	_hbmpMem, _hbmpMemOld;
#endif

	CORETIME	_begin_time, _end_time, _period;

	FaceXycxcyToRect(wFTU_, &_rc);
	CopyRect( &_rcBu, &_rc );

	_lpPrm = FaceGetParameter( wFTU_ );

	MFPUREAD( _lpPrm, HISTRD_LEFTLABEL, BOOL, _bLeftLabel );
	MFPUREAD( _lpPrm, HISTRD_TOPLABEL, BOOL, _bTopLabel );
	MFPUREAD( _lpPrm, HISTRD_RIGHTLABEL, BOOL, _bRightLabel );
	MFPUREAD( _lpPrm, HISTRD_BOTTOMLABEL, BOOL, _bBottomLabel );
	_dwStyle = 0;
	if( _bLeftLabel )	_dwStyle |= STYLE_LEFTLABEL;
	if( _bTopLabel )	_dwStyle |= STYLE_TOPLABEL;
	if( _bRightLabel )	_dwStyle |= STYLE_RIGHTLABEL;
	if( _bBottomLabel ) _dwStyle |= STYLE_BOTTOMLABEL;

	MFPUREAD( _lpPrm, HISTRD_MIN, REAL, _min );
	MFPUREAD( _lpPrm, HISTRD_MAX, REAL, _max );
	if( _max - _min == 0 )
		return FALSE;

	MFPUREAD( _lpPrm, HISTRD_VERLABELDIV, WORD, _wVerLabelDiv );
	if( !_wVerLabelDiv ) _wVerLabelDiv = 1;
	MFPUREAD( _lpPrm, HISTRD_BEGINDATETIME, CORETIME, _begin_time );
	MFPUREAD( _lpPrm, HISTRD_ENDDATETIME, CORETIME, _end_time );
	_period = _end_time - _begin_time;
	if( _period < 0 )  //incorrect time
		return FALSE;

	//CoreIsRunTime()

#ifdef _USE_OWN_HDC
	_rc.right = _rc.right - _rc.left;
	_rc.bottom = _rc.bottom - _rc.top;
	_rc.left = _rc.top = 0;

	_hdcMem = CreateCompatibleDC( hdc_ );
	_hbmpMem = CreateCompatibleBitmap( hdc_, _rc.right, _rc.bottom );
	_hbmpMemOld = SelectObject( _hdcMem, _hbmpMem );

	BitBlt( _hdcMem,
			0, 0, _rc.right, _rc.bottom,
			hdc_,
			_rcBu.left, _rcBu.top,
			SRCCOPY );

	_hdcDraw = _hdcMem;
#else
	_hdcDraw = hdc_;
#endif

	//save current DC before changing the clip
	//rectangle, after drawing data-tracks, we
	//shall restore the current dc;
	SaveDC( _hdcDraw ); 	//limit to draw in face box
	IntersectClipRect( _hdcDraw,
					   _rc.left, _rc.top,
					   _rc.right, _rc.bottom );

	SelectObject( _hdcDraw, GetStockObject( ANSI_VAR_FONT ) );
	SetBkMode( _hdcDraw, TRANSPARENT );

	GetTextMetrics( _hdcDraw, &_tm );

	_CalculateChartRect( _hdcDraw, _lpPrm, &_rc, &_rcChart );

	//draw left labels and right labels
	MFPUREAD( _lpPrm, HISTRD_VERLABELCLR, DWORD, _clrText );
	_retClr = CclrGetRGB( _clrText, &_rgbText );
	SetTextColor( hdc_, _rgbText );

	_delta = ((float)(_rcChart.bottom - _rcChart.top)) / (float)_wVerLabelDiv;
	_delta_value = (_max - _min) / _wVerLabelDiv;
	for( _i=0, _vl = _min, _y = (float)_rcChart.bottom;
		_i<=_wVerLabelDiv;
		_i++, _vl += _delta_value, _y -= _delta )
	{
		size_t		_str_len;
		RECT		_rcLabel;

		gcvt( _vl, 5, _szBuffer );
		_str_len = strlen( _szBuffer );

		//draw the left label
		if( _dwStyle & STYLE_LEFTLABEL )
		{
			if( _i == 0 )
			{
				SetRect(&_rcLabel,
						_rc.left, _y-_tm.tmHeight,
						_rcChart.left - _GIP_X, (int)_y );
				if( _retClr )	//otherwise, transparent
					DrawText(_hdcDraw, _szBuffer, _str_len, &_rcLabel, DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);
			}
			else if( _i == _wVerLabelDiv )
			{
				SetRect(&_rcLabel,
						_rc.left, _y,
						_rcChart.left - _GIP_X, (int)(_y+_tm.tmHeight) );
				if( _retClr )	//otherwise, transparent
					DrawText(_hdcDraw, _szBuffer, _str_len, &_rcLabel, DT_RIGHT|DT_TOP|DT_SINGLELINE);
			}
			else
			{
				SetRect(&_rcLabel,
						_rc.left, (int)(_y-_tm.tmHeight),
						_rcChart.left - _GIP_X, (int)(_y+_tm.tmHeight) );
				if( _retClr )	//otherwise, transparent
					DrawText(_hdcDraw, _szBuffer, _str_len, &_rcLabel, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
			}
		}

		//draw right label
		if( _dwStyle & STYLE_RIGHTLABEL )
		{
			SetRect(&_rcLabel,
					_rcChart.right + _GIP_X,
					_y-_tm.tmHeight,
					_rc.right,
					_y+_tm.tmHeight);
			if( _retClr )	//otherwise, transparent
				DrawText(_hdcDraw, _szBuffer, _str_len, &_rcLabel, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
	}

	//draw top labels and bottom labels
	_TrendDrawHrzLabel( _hdcDraw, _lpPrm, _dwStyle, _begin_time, _end_time, &_rcChart, &_rc);

	//draw the chart WITHOUT border
	MFPUREAD( _lpPrm, HISTRD_BACKCLR, DWORD, _clrBk );
	_hBrush = CoreCreateBrush(_clrBk);
	_hPen = SelectObject(_hdcDraw, GetStockObject(NULL_PEN) );
	_hBrush = SelectObject(_hdcDraw, _hBrush);
	Rectangle( _hdcDraw, _rcChart.left, _rcChart.top, _rcChart.right, _rcChart.bottom );
	SelectObject(_hdcDraw, _hPen);
	_hBrush = SelectObject(_hdcDraw, _hBrush);
	CoreDeleteBrush(_hBrush);

	//draw Hrz line
	_TrendDrawHrzLines(_hdcDraw, _lpPrm, &_rcChart);

	//draw vertical line
	_TrendDrawVerLines(_hdcDraw, _lpPrm, &_rcChart);

	//draw the data track lines
	_wNumParam = FaceGetNumParam(wFTU_);
	_divt = div( _wNumParam - NUM_FIX, NUM_VAR );
	CORE_ASSERT( _divt.rem == 0 );
	_nNumPen = _divt.quot;

	//donot calculate _factor_y and _offset_y here
	//now, each pen has it own _factor_y and offset
	//
	//_factor_y = (_rcChart.top - _rcChart.bottom + 1)/(_max - _min);
	//_offset_y = _rcChart.top - _max * _factor_y;
	//

	//set _lpPrmVar to the beginning of the datasource0
	_lpPrmVar = _lpPrm + NUM_FIX;

	//save current DC before changing the clip
	//rectangle, after drawing data-tracks, we
	//shall restore the current dc;
	SaveDC( _hdcDraw );
	IntersectClipRect( _hdcDraw,
					   _rcChart.left, _rcChart.top,
					   _rcChart.right, _rcChart.bottom );

	_factor_x = (float)(_rcChart.right - 1 - _rcChart.left) / _period;
	_offset_x = _rcChart.left - _factor_x * _begin_time;

	for(_i=0;
		_i<_nNumPen;
		_i++, _lpPrmVar += NUM_VAR )
	{
		LPCOBJ			_lpCobj;
		LPCACHECELL 	_lpcc;
		float			_x, _y, _x1, _y1;
		long			_j;
		BOOL			_bool_var;
		LPCACHEHEAD 	_lpCache;
		int 			_break = 0;

		MFPUREAD( _lpPrmVar, HISTRD_DATACACHE, LPCOBJ, _lpCobj );
		_lpCache = (LPCACHEHEAD)GET_COBJ_DATA( _lpCobj );
		if( !_lpCache ) //no data
			continue;

		_lpcc = &( _lpCache->cc );
		if( _lpCache->num_cc <= 0 )
			continue;

		MFPUREAD( _lpPrmVar, HISTRD_PENSTYLE, int, _nPenStyle );
		MFPUREAD( _lpPrmVar, HISTRD_PENWIDTH, int, _nPenWidth );
		MFPUREAD( _lpPrmVar, HISTRD_PENCLR, DWORD, _clrPen );

		MFPUREAD( _lpPrmVar, HISTRD_DATAMIN, REAL, _data_min );
		MFPUREAD( _lpPrmVar, HISTRD_DATAMAX, REAL, _data_max );

		MFPUREAD( _lpPrmVar, HISTRD_BOOLVAR, BOOL, _bool_var );

		_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrPen);
		_hPen = SelectObject(_hdcDraw, _hPen);

		if( _data_max - _data_min )
		{
			_factor_y = (_rcChart.top - _rcChart.bottom + 1)/(_data_max - _data_min);
			_offset_y = _rcChart.top - _data_max * _factor_y;
		}
		else
		{
			_factor_y = (_rcChart.top - _rcChart.bottom + 1)/(_max - _min);
			_offset_y = _rcChart.top - _max * _factor_y;
		}

		_j = 0; //initialize _j

		//first, find the first point with data, and move
		//the pen to there;
		while( _j < _lpCache->num_cc )
		{
			if( _lpcc->time < 0 )	//means no data here
				_j++, _lpcc++;
			else
				break;
		}

		_x = _factor_x * _lpcc->time + _offset_x;
		_y = ( (float)(_lpcc->val) ) * _factor_y + _offset_y;
		MoveTo(_hdcDraw, _x, _y);

		_break = 0; //set the flag to FALSE initially
		for(_j++, _lpcc++;	//value of _j is set propertily
			_j < _lpCache->num_cc;
			_j++, _lpcc++ )
		{
			if( _lpcc->time < 0 )	//means data are broken here
			{
				_break = 1;
				continue;
			}
			else
			{
				_x1 = _factor_x * _lpcc->time + _offset_x;
				_y1 = ( _lpcc->val )*_factor_y + _offset_y;

				//if _break == 0, excuting the following code
				//is of certain;
				//if _break == 1, this is a exception case
				//because the broken point should be occurred
				//in pair, we samply skip the case here

				if( !_break )
				{
					if( _bool_var )
						LineTo( _hdcDraw, _x1, _y);
					else
						LineTo( _hdcDraw, _x1, _y1 );
				}
				else	// _break > 1
				{
					MoveTo( _hdcDraw, _x1, _y1 );
					_break = 0;
				}

				_x = _x1;
				_y = _y1;
			}	//end if...

		}	//end while
		_hPen = SelectObject(_hdcDraw, _hPen);
		CoreDeletePen(_hPen);
	}
	//restore clipping region
	RestoreDC( _hdcDraw, -1 );

	//draw the chart BORDER
	MFPUREAD( _lpPrm, HISTRD_BORDERSTYLE, int, _nPenStyle );
	MFPUREAD( _lpPrm, HISTRD_BORDERWIDTH, DWORD, _dwBorderWidth );
	MFPUREAD( _lpPrm, HISTRD_BORDERCLR, DWORD, _clrPen );
	CoreDrawBox( _hdcDraw, &_rcChart, _nPenStyle, _dwBorderWidth, _clrPen );

	//restore clipping region
	RestoreDC( _hdcDraw, -1 );

#ifdef _USE_OWN_HDC
	BitBlt( hdc_,
			_rcBu.left, _rcBu.top, _rc.right, _rc.bottom,
			_hdcMem,
			0,0,
			SRCCOPY );
	SelectObject( _hdcMem, _hbmpMemOld );
	DeleteObject( _hbmpMem );
	DeleteDC( _hdcMem );
#endif

	return TRUE;
}

//...................................................................
//...................................................................
static void _near _TrendDrawHrzLines( HDC hdc_, LPPU lpPrm_, LPRECT lprcChart_ )
{
	COLORREF	_clrPen;
	int 		_nPenWidth, _nPenStyle;
	HPEN		_hPen;
	WORD		_wMajorDiv, _wMinorDiv;
	float		_delta, _y, _delta_j, _yj;
	int 		_i, _j;

	//draw the major hrz lines
	MFPUREAD( lpPrm_, HISTRD_HRZMAJLNSTYLE, int, _nPenStyle );
	MFPUREAD( lpPrm_, HISTRD_HRZMAJLNWIDTH, int, _nPenWidth );
	MFPUREAD( lpPrm_, HISTRD_HRZMAJLNCLR, DWORD, _clrPen );
	MFPUREAD( lpPrm_, HISTRD_HRZMAJLNDIV, WORD, _wMajorDiv );
	if( !_wMajorDiv ) _wMajorDiv = 1;

	_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrPen);
	_hPen = SelectObject(hdc_, _hPen);
	_delta = (float)(lprcChart_->bottom - lprcChart_->top) / _wMajorDiv;
	for( _i=1, _y=lprcChart_->top+_delta;
		_i<_wMajorDiv;
		_i++, _y += _delta)
	{
		MoveTo(hdc_, lprcChart_->left, _y);
		LineTo(hdc_, lprcChart_->right, _y);
	}
	_hPen = SelectObject(hdc_, _hPen);
	CoreDeletePen(_hPen);

	//draw the minor hrz lines
	MFPUREAD( lpPrm_, HISTRD_HRZMINLNSTYLE, int, _nPenStyle );
	MFPUREAD( lpPrm_, HISTRD_HRZMINLNWIDTH, int, _nPenWidth );
	MFPUREAD( lpPrm_, HISTRD_HRZMINLNCLR, DWORD, _clrPen );
	MFPUREAD( lpPrm_, HISTRD_HRZMINLNDIV, WORD, _wMinorDiv );
	if( !_wMinorDiv ) _wMinorDiv = 1;

	_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrPen);
	_hPen = SelectObject(hdc_, _hPen);
	_delta_j = _delta / _wMinorDiv;
	for( _i=0, _y=lprcChart_->top;
		_i<_wMajorDiv;
		_i++, _y += _delta)
	{
		for( _j=1, _yj=_y + _delta_j;
			_j<_wMinorDiv;
			_j++, _yj += _delta_j)
		{
			MoveTo(hdc_, lprcChart_->left, _yj);
			LineTo(hdc_, lprcChart_->right, _yj);
		}
	}
	_hPen = SelectObject(hdc_, _hPen);
	CoreDeletePen(_hPen);

	return;
}

//...................................................................
//...................................................................
static void _near _TrendDrawVerLines(HDC	hdc_,
									 LPPU	lpPrm_, 	//beginning of parameters of the trend
									 LPRECT lprc_)		//rcChart_
{
	WORD		_wdv;
	WORD		_wdv_min;
	int 		_nPenWidth, _nPenStyle;
	COLORREF	_clrPen;
	HPEN		_hPen;
	int 		_i, _j, _x, _x1;
	float		_delta, _delta_j;

	MFPUREAD( lpPrm_, HISTRD_VERMAJLNDIV, WORD, _wdv );
	if( _wdv == 0 ) _wdv++;

	//draw the Ver major lines
	MFPUREAD( lpPrm_, HISTRD_VERMAJLNSTYLE, int, _nPenStyle );
	MFPUREAD( lpPrm_, HISTRD_VERMAJLNWIDTH, int, _nPenWidth );
	MFPUREAD( lpPrm_, HISTRD_VERMAJLNCLR, COLORREF, _clrPen );
	_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrPen);
	_hPen = SelectObject(hdc_, _hPen);

	_delta = (lprc_->right - lprc_->left) / _wdv;

	for( _i=1, _x = lprc_->left + _delta;
		_i<_wdv;
		_i++, _x += _delta )
	{
		MoveTo( hdc_, _x, lprc_->top );
		LineTo( hdc_, _x, lprc_->bottom);
	}
	_hPen = SelectObject(hdc_, _hPen);
	CoreDeletePen(_hPen);

	//draw the Ver minor lines
	MFPUREAD( lpPrm_, HISTRD_VERMINLNDIV, WORD, _wdv_min );
	if( _wdv_min == 0 ) _wdv_min++;

	MFPUREAD( lpPrm_, HISTRD_VERMINLNSTYLE, int, _nPenStyle );
	MFPUREAD( lpPrm_, HISTRD_VERMINLNWIDTH, int, _nPenWidth );
	MFPUREAD( lpPrm_, HISTRD_VERMINLNCLR, COLORREF, _clrPen );
	_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrPen);
	_hPen = SelectObject(hdc_, _hPen);

	_delta_j = _delta / _wdv_min;

	for( _i=0, _x = lprc_->left;
		_i<_wdv;
		_i++, _x += _delta )
	{
		for( _j=1, _x1 = _x + _delta_j;
			 _j < _wdv_min;
			 _j++, _x1 += _delta_j )
		{
			MoveTo( hdc_, _x1, lprc_->top );
			LineTo( hdc_, _x1, lprc_->bottom);
		}
	}

	_hPen = SelectObject(hdc_, _hPen);
	CoreDeletePen(_hPen);

	return;
}

//...................................................................
//...................................................................
static void _near _TrendDrawHrzLabel(HDC		hdc_,
									 LPPU		lpPrm_, 		//beginning of parameters of the trend
									 DWORD		dwStyle_,		//the style of the trend
									 CORETIME	ctBeginTime_,
									 CORETIME	ctEndTime_,
									 LPRECT 	lprcChart_,
									 LPRECT 	lprcTrend_)
{
	WORD		_wdv;
	int 		_i;
	int 		_x, _delta_x, _cxChart;
	CORETIME	_tt;		//loop var
	CCLR		_clrText;
	COLORREF	_rgbText;
	char		_szDate[] = "YY-MM-DD";
	char		_szTime[] = "HH:MM:SS";
	int 		_wid_str;
	float		_delta;
	TEXTMETRIC	_tm;
	BOOL		_retClr;	//return value by CclrGetRGB

	if( ctEndTime_ - ctBeginTime_ <= 0 )
		return;

	GetTextMetrics( hdc_, &_tm );

	_cxChart = lprcChart_->right - lprcChart_->left;

#ifndef _WIN32
	_wid_str = LOWORD( GetTextExtent(hdc_, _szDate, strlen(_szDate) ) );
#else
	{
		SIZE	_sz;
		GetTextExtentPoint32(hdc_, _szDate, strlen(_szDate), &_sz );
		_wid_str = _sz.cx; 
	}
#endif

	//draw the bottom labels and top labels
	MFPUREAD( lpPrm_, HISTRD_HRZLABELDIV, WORD, _wdv );
	if( _wdv == 0 ) _wdv++;
	_delta = (float)( ctEndTime_ - ctBeginTime_ ) / _wdv;
	_delta_x = _cxChart / _wdv;

	MFPUREAD( lpPrm_, HISTRD_HRZLABELCLR, DWORD, _clrText );
	_retClr = CclrGetRGB( _clrText, &_rgbText );
	SetTextColor( hdc_, _rgbText );

	for( _i=0, _tt = ctBeginTime_, _x = lprcChart_->left;
		 _i <= _wdv;
		 _i++, _tt += _delta, _x += _delta_x )
	{
		RECT		_rcLabel;

		CoreTimeToAscii( _tt, _szDate, _szTime );

		//draw the bottom label
		if( dwStyle_ & STYLE_BOTTOMLABEL )
		{
			SetRect(&_rcLabel,
					_x - _wid_str,
					lprcTrend_->top,
					_x + _wid_str,
					lprcTrend_->bottom - _tm.tmHeight - _tm.tmExternalLeading );
			if( _retClr )	//otherwise, transparent
				DrawText( hdc_, _szTime, -1, &_rcLabel, DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
			SetRect(&_rcLabel,
					_x - _wid_str,
					lprcTrend_->top,
					_x + _wid_str,
					lprcTrend_->bottom);
			if( _retClr )	//otherwise, transparent
				DrawText( hdc_, _szDate, -1, &_rcLabel, DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
		}

		//draw the top label
		if( dwStyle_ & STYLE_TOPLABEL )
		{
			SetRect(&_rcLabel,
					_x - _wid_str,
					lprcTrend_->top - _tm.tmHeight - _tm.tmExternalLeading,
					_x + _wid_str,
					lprcTrend_->bottom);
			if( _retClr )	//otherwise, transparent
				DrawText( hdc_, _szTime, -1, &_rcLabel, DT_CENTER|DT_TOP|DT_SINGLELINE);
			SetRect(&_rcLabel,
					_x - _wid_str,
					lprcTrend_->top,
					_x + _wid_str,
					lprcTrend_->bottom);
			if( _retClr )	//otherwise, transparent
				DrawText( hdc_, _szDate, -1, &_rcLabel, DT_CENTER|DT_TOP|DT_SINGLELINE);
		}
	}

	return;
}

//...................................................................
//Description:
//
//...................................................................
static void _near CoreTimeToAscii( time_t tt_, LPSTR lpszDate_, LPSTR lpszTime_ )
{
	char	_szTemp[16];
	struct tm * _ptm = gmtime( &tt_ );

	if( !_ptm )
	{
		CORE_ASSERT( FALSE );
		*lpszDate_ = 0;
		*lpszTime_ = 0;
		return;
	}

	sprintf( _szTemp, "%04u-%02u-%02u", _ptm->tm_year + 1900 , _ptm->tm_mon + 1, _ptm->tm_mday );
	sprintf( lpszTime_, "%02u:%02u:%02u", _ptm->tm_hour, _ptm->tm_min, _ptm->tm_sec );
	strcpy( lpszDate_, _szTemp + 2*sizeof(char) );

	return;
}

//...................................................................
//Description:
//
//...................................................................
static BOOL _near _CalculateChartRect(HDC hdc_, LPPU lpPrm_, LPRECT lprc_, LPRECT lprcChart_ )
{
	BOOL		_bLeftLabel, _bTopLabel, _bRightLabel, _bBottomLabel;
	DWORD		_dwStyle;
	WORD		_extent;	//the max width of the ver. labels
	TEXTMETRIC	_tm;
	int 		_i;
	float		_min, _max, _vl, _delta;
	WORD		_wVerLabelDiv;
	char		_szBuffer[32];

	MFPUREAD( lpPrm_, HISTRD_LEFTLABEL, BOOL, _bLeftLabel );
	MFPUREAD( lpPrm_, HISTRD_TOPLABEL, BOOL, _bTopLabel );
	MFPUREAD( lpPrm_, HISTRD_RIGHTLABEL, BOOL, _bRightLabel );
	MFPUREAD( lpPrm_, HISTRD_BOTTOMLABEL, BOOL, _bBottomLabel );
	_dwStyle = 0;
	if( _bLeftLabel )	_dwStyle |= STYLE_LEFTLABEL;
	if( _bTopLabel )	_dwStyle |= STYLE_TOPLABEL;
	if( _bRightLabel )	_dwStyle |= STYLE_RIGHTLABEL;
	if( _bBottomLabel ) _dwStyle |= STYLE_BOTTOMLABEL;
	//MFPUREAD( lpPrm_, HISTRD_STYLE, DWORD, _dwStyle );

	MFPUREAD( lpPrm_, HISTRD_MIN, REAL, _min );
	MFPUREAD( lpPrm_, HISTRD_MAX, REAL, _max );
	MFPUREAD( lpPrm_, HISTRD_VERLABELDIV, WORD, _wVerLabelDiv );
	if( !_wVerLabelDiv ) _wVerLabelDiv = 1;

	GetTextMetrics( hdc_, &_tm );
	_delta = (_max - _min)/(_wVerLabelDiv);

	//get the max width of ver. labels, and the result returns by _extent
	_extent = 0;
	for( _i=0, _vl = _min;
		_i<=_wVerLabelDiv;
		_i++, _vl += _delta )
	{
#ifndef _WIN32
		DWORD		_gte;

		gcvt( _vl, 5, _szBuffer );
		_gte = GetTextExtent( hdc_, _szBuffer, strlen(_szBuffer) );
		_extent = _extent>LOWORD(_gte)? _extent: LOWORD(_gte);
#else
		SIZE	_sz;
		gcvt( _vl, 5, _szBuffer );
		GetTextExtentPoint32( hdc_, _szBuffer, strlen(_szBuffer), &_sz );
		_extent = _extent>_sz.cx? _extent: _sz.cx;
#endif	//_WIN32
	}

	////...BEGIN to calculate the rectangle bounding the chart
	memcpy( lprcChart_, lprc_, sizeof(RECT) );
	if( _dwStyle & STYLE_TOPLABEL )
		lprcChart_->top += (_tm.tmHeight + _tm.tmExternalLeading)*2 + _GIP_Y;
	if( _dwStyle & STYLE_BOTTOMLABEL )
		lprcChart_->bottom -= (_tm.tmHeight + _tm.tmExternalLeading)*2 + _GIP_Y;
	if( _dwStyle & STYLE_LEFTLABEL )
		lprcChart_->left += _extent + _GIP_X;
	if( _dwStyle & STYLE_RIGHTLABEL )
		lprcChart_->right -= _extent + _GIP_X;

	//if it has left label or right label, but no top label
	if( ((_dwStyle & STYLE_TOPLABEL) == NULL)
	   && (_dwStyle & (STYLE_LEFTLABEL | STYLE_RIGHTLABEL)) )
	{
		lprcChart_->top += (_tm.tmHeight+1)/2;
	}

	//if it has left label or right label, but no bottom label
	if( ((_dwStyle & STYLE_BOTTOMLABEL) == NULL)
	   && (_dwStyle & (STYLE_LEFTLABEL | STYLE_RIGHTLABEL)) )
	{
		lprcChart_->bottom -= (_tm.tmHeight+1)/2;
	}
	////...END calculting the _rcChart

	return TRUE;
}

//...................................................................
//Description:
//	This function query the history data
//Entries:
//		BOOL bUpdate_	if this is TRUE, it forces updating
//						data even if the condition un-changed;
//
//Returns:
//	If log data at this time, returns TRUE; otherwise, return FALSE;
//Note:
//...................................................................
static BOOL _near UpdateHisData(WRD wFTU_, BOOL bUpdate_)
{
	unsigned	_i;
	LPPU		_lpPrm;
	DWORD		_num_pen;
	LPPU		_lpPrmVar;
	CORETIME	_begin_time, _end_time;
	HCURSOR 	_hcurSave;

	WORD		_wNumParam;

	_wNumParam = FaceGetNumParam( wFTU_ );
	_lpPrm = FaceGetParameter( wFTU_ );

	MFPUREAD( _lpPrm, HISTRD_BEGINDATETIME, CORETIME, _begin_time );
	MFPUREAD( _lpPrm, HISTRD_ENDDATETIME, CORETIME, _end_time );

	//collect data and flush the trend FTU
	_num_pen = ( _wNumParam - NUM_FIX ) / NUM_VAR;

	//Set the cursor to the hourglass and save the previous cursor.
	_hcurSave = SetCursor( LoadCursor(NULL, IDC_WAIT) );

	for( _i=0, _lpPrmVar = _lpPrm + NUM_FIX;
		 _i < _num_pen;
		 _i++, _lpPrmVar += NUM_VAR )
	{
		WORD			_table;
		WRD 			_wrd;
		LPPU			_lpPU;
		LPCOBJ			_lpCobj;
		LPCACHEHEAD 	_lpCache;

		_lpPU = _lpPrmVar + HISTRD_DATASOURCE;
		_wrd = _lpPU->du.zzk__WRD;

		if( _lpPU->prefix == (PF_TBX|TBX_RT|RTU_VALUE) )
		{
			_table = TBX_RT;
			if( !RealIsValid( _wrd ) )
				continue;
		}
		else if( _lpPU->prefix == (PF_TBX|TBX_TT|TTU_VALUE) )
		{
			_table = TBX_TT;
			if( !TintIsValid( _wrd ) )
				continue;
		}
		else if( _lpPU->prefix == (PF_TBX|TBX_BT|BTU_VALUE) )
		{
			_table = TBX_BT;
			if( !BoolIsValid( _wrd ) )
				continue;
		}
		else
		{
			//illegal data source
			continue;
		}

		MFPUREAD( _lpPrmVar, HISTRD_DATACACHE, LPCOBJ, _lpCobj );
		_lpCache = (LPCACHEHEAD)GET_COBJ_DATA( _lpCobj );

		if( !_lpCache )
		{
			_lpCache = AllocateHisCache( CACHE_MAX_ITEMS );

			if( !_lpCache )
				continue;
			//_lpCobj = CAST_TO_COBJ( _lpCache );
			_lpCobj = (LPCOBJ)(LPVOID)( _lpCache );
			MFPUWRITE( _lpPrmVar, HISTRD_DATACACHE, LPCOBJ, _lpCobj );
		}

		if( !bUpdate_ )
		{
			if(  (_lpCache->begin_time == _begin_time)
			   &&(_lpCache->end_time == _end_time)
			   &&(_lpCache->table == _table)
			   &&(_lpCache->wrd == _wrd) )
			   continue;
		}

		//Note:
		//At this version, we use a sample strategy, for each pen,
		//a fix size cache is allocated, at this version, its size
		//is 800;
		QueryHisData( _lpCache, _table, _wrd, _begin_time, _end_time );
	}

	//Restore the previous cursor.
	SetCursor( _hcurSave );

	return	TRUE;
}

//This is an obsolete function
static WORD _near _TrendRunDirty( WRD wFTU_, WORD dirty_ )
{
	LPPU	_lpPrm;
	WORD	_wNumParam;
	div_t	_r_div;
	WORD	_i;

	if ( dirty_ & DEEP_DIRTY )
		return dirty_;

	_lpPrm = FaceGetParameter( wFTU_ );
	_wNumParam = FaceGetNumParam( wFTU_ );
	CORE_ASSERT( _lpPrm );

	for(_i=0;
		_i<_wNumParam;
		_i++, _lpPrm++ )
	{
		if( PUIsDirty(_lpPrm) )
		{
			if(   (_i == HISTRD_LEFTLABEL )
				||(_i == HISTRD_TOPLABEL )
				||(_i == HISTRD_RIGHTLABEL )
				||(_i == HISTRD_BOTTOMLABEL ) )
				return POSITION_DIRTY;

			if( _i >= NUM_FIX )
			{
				_r_div = div( _i - NUM_FIX, NUM_VAR );
				if( _r_div.rem == HISTRD_DATASOURCE )
					continue;	//ignore data-source dirty
			}
			return NORMAL_DIRTY;
		}
	}

	return NO_DIRTY;
}

typedef struct _tagHISTORY_TREND_CONFIG_STRUCT {
	WRD 	wrd;		//the WRD of the face to be configured
	WRD 	wrdTemp;	//the WRD of the temp face as a buffer
}HTCS, FAR* LPHTCS;


BOOL _Dlg_GetDataTime( HWND hwnd_, WRD wrd_ );
//...................................................................
//...................................................................
#ifdef _WIN32
BOOL CALLBACK
#else
BOOL CALLBACK _export 
#endif
ConfigDlgProc( HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_ )
{
	switch(msg_)
	{
		case WM_INITDIALOG:
		{
			//var, min, max, clr, digit
			static	int _cols[] = {100, 40, 40, 70, 60};

			int 		_num_pen;
			HWND		_hwndCtrl, _hwndForm;
			RECT		_rc;
			FORM		_form;
			CORETIME	_begin_time, _end_time;
			WRD 		_wrd;
			LPPU		_lpPrm;
			LPHTCS		_slpHtcs = NULL;
			WRD 		_wrdTemp;
			struct tm	_tm;
			WORD		_wNumParam;
			POINT		_point1;
			HINSTANCE	_hInst;

			_wrd = (WRD)lParam_;
			_wrdTemp = FaceCopyTemp( _wrd );
			_wNumParam = FaceGetNumParam( _wrdTemp );
			_lpPrm = FaceGetParameter( _wrd );

			//create a buffer structure and initialize it
			_slpHtcs = (LPHTCS)AllocGlobalPtr( sizeof(HTCS) );
			if( !_slpHtcs ) goto _BAD;
			_slpHtcs->wrd = _wrd;
			_slpHtcs->wrdTemp = _wrdTemp;

			/////////
			//add form window to show pens data

			//get form window size and position
			_hwndCtrl = GetDlgItem( hwnd_, IDC_PEN_LIST );
			GetWindowRect( _hwndCtrl, &_rc );
			_point1.x = _rc.left, _point1.y = _rc.top;
			//_point2.x = _rc.right, _point2.bottom = _rc.top;
			ScreenToClient( hwnd_, &_point1 );
			//ScreenToClient( hwnd_, &_point2 );

			_num_pen = ( _wNumParam - NUM_FIX ) / NUM_VAR;

			memset( &_form, 0, sizeof(FORM) );
			_form.Rows = _num_pen;
			_form.Cols = 5;    //var, min, max, color, digit
			_form.WidthRowTitle = 16;
			_form.HeightColTitle = 20;
			_form.bEqualWidthCol = FALSE;
			_form.pColWidth = _cols;
			_form.ColWidth = 60;
			_form.pfnFormData = _PenFormData;
			_form.lData = (LONG)_slpHtcs;

#ifndef _WIN32
			_hInst = (HINSTANCE)GetWindowWord(hwnd_, GWW_HINSTANCE );
#else
			_hInst = (HINSTANCE)GetWindowLong(hwnd_, GWL_HINSTANCE );	
#endif
			_hwndForm = CreateWindow( ZZKFORMCLASSNAME,
									  NULL,
									  WS_CHILD|WS_HSCROLL|WS_VSCROLL|WS_VISIBLE|WS_BORDER|WS_TABSTOP,
									  _point1.x,
									  _point1.y,
									  _rc.right - _rc.left,
									  _rc.bottom - _rc.top,
									  hwnd_,
									  (HMENU)IDC_PENFORM,
									  _hInst,
									  //ghInst,
									  &_form );
			if( !_hwndForm ) goto _BAD;

			MFPUREAD( _lpPrm, HISTRD_BEGINDATETIME, CORETIME, _begin_time );
			CoreTime2tm( _begin_time, &_tm );
			SetDlgItemInt( hwnd_, IDC_BEGIN_YEAR,	_tm.tm_year+1900, TRUE );
			SetDlgItemInt( hwnd_, IDC_BEGIN_MONTH,	_tm.tm_mon+1, TRUE );
			SetDlgItemInt( hwnd_, IDC_BEGIN_DAY,	_tm.tm_mday, TRUE );
			SetDlgItemInt( hwnd_, IDC_BEGIN_HOUR,	_tm.tm_hour, TRUE );
			SetDlgItemInt( hwnd_, IDC_BEGIN_MINUTE, _tm.tm_min, TRUE );
			SetDlgItemInt( hwnd_, IDC_BEGIN_SECOND, _tm.tm_sec, TRUE );

			MFPUREAD( _lpPrm, HISTRD_ENDDATETIME, CORETIME, _end_time );
			CoreTime2tm( _end_time, &_tm );
			SetDlgItemInt( hwnd_, IDC_END_YEAR,   _tm.tm_year+1900, TRUE );
			SetDlgItemInt( hwnd_, IDC_END_MONTH,  _tm.tm_mon+1, TRUE );
			SetDlgItemInt( hwnd_, IDC_END_DAY,	  _tm.tm_mday, TRUE );
			SetDlgItemInt( hwnd_, IDC_END_HOUR,   _tm.tm_hour, TRUE );
			SetDlgItemInt( hwnd_, IDC_END_MINUTE, _tm.tm_min, TRUE );
			SetDlgItemInt( hwnd_, IDC_END_SECOND, _tm.tm_sec, TRUE );

			return TRUE;
_BAD:
			if( _wrdTemp )
				FaceDeleteTemp( _wrdTemp );

			if( _slpHtcs )
				FreeGlobalPtr( _slpHtcs );

			EndDialog( hwnd_, IDCANCEL );
			return TRUE;
		}

		case WM_COMMAND:
		{
			int _id = GET_WM_COMMAND_CTRL_ID( wParam_, lParam_ );

			LPHTCS		_slpHtcs = NULL;

			if( _id == IDC_DELETEPEN )
			{
				HWND _hwndForm = GetDlgItem(hwnd_, IDC_PENFORM);
				LRESULT 	_lv;		//a long value
				int 		_row;

				_slpHtcs = (LPHTCS)SendMessage( GetDlgItem(hwnd_, IDC_PENFORM),
												SM_GET_DATA, 0, 0L );
				_lv = SendMessage( _hwndForm, SM_GET_ACTIVECELL, 0, 0L );

				FaceDeleteParamGroup( _slpHtcs->wrdTemp, (WORD)(NUM_FIX + LOWORD(_lv)*NUM_VAR) );
				_row = (int)SendMessage( _hwndForm, SM_GET_NUMBER_OF_ROW, 0, 0L );
				SendMessage( _hwndForm, SM_SET_NUMBER_OF_ROW, 0, _row - 1 );

				//Update the form window
				InvalidateRect( _hwndForm, NULL, TRUE );
				UpdateWindow( _hwndForm );
			}
			else if( _id == IDC_APPENDPEN )
			{
				int 		_row;
				HWND _hwndForm = GetDlgItem(hwnd_, IDC_PENFORM);

				_slpHtcs = (LPHTCS)SendMessage( GetDlgItem(hwnd_, IDC_PENFORM),
												SM_GET_DATA, 0, 0 );
				FaceAppendParamGroup( _slpHtcs->wrdTemp );
				_row = (int)SendMessage( _hwndForm, SM_GET_NUMBER_OF_ROW, 0, 0 );
				SendMessage( _hwndForm, SM_SET_NUMBER_OF_ROW, 0, _row + 1 );

				//Update the form window
				InvalidateRect( _hwndForm, NULL, TRUE );
				UpdateWindow( _hwndForm );
			}
			else if( _id == IDOK )
			{
				_slpHtcs = (LPHTCS)SendMessage( GetDlgItem(hwnd_, IDC_PENFORM),
												SM_GET_DATA, 0, 0 );

				_Dlg_GetDataTime( hwnd_, _slpHtcs->wrdTemp );
				FaceUpdateParameter( _slpHtcs->wrd, _slpHtcs->wrdTemp );
				UpdateHisData( _slpHtcs->wrd, FALSE );	//NOT force to update
				EndDialog( hwnd_, IDOK );
				FreeGlobalPtr( _slpHtcs );
				return TRUE;
			}
			if( _id == IDC_FORCE_UPDATE )
			{
				_slpHtcs = (LPHTCS)SendMessage( GetDlgItem(hwnd_, IDC_PENFORM),
												SM_GET_DATA, 0, 0 );

				_Dlg_GetDataTime( hwnd_, _slpHtcs->wrdTemp );
				FaceUpdateParameter( _slpHtcs->wrd, _slpHtcs->wrdTemp );
				UpdateHisData( _slpHtcs->wrd, TRUE );	//force to update
				EndDialog( hwnd_, IDOK );
				FreeGlobalPtr( _slpHtcs );
				return TRUE;
			}
			else if( _id == IDCANCEL )
			{
				_slpHtcs = (LPHTCS)SendMessage( GetDlgItem(hwnd_, IDC_PENFORM),
												SM_GET_DATA, 0, 0 );
				//delete buffer FTU
				if( _slpHtcs )
					FaceDeleteTemp( _slpHtcs->wrdTemp );

				EndDialog( hwnd_, IDCANCEL );
				FreeGlobalPtr( _slpHtcs );
				return TRUE;
			}
			else
				return FALSE;
		}

		default:
			return FALSE;
	}
}

BOOL _Dlg_GetDataTime( HWND hwnd_, WRD wrd_ )
{
	LPPU		_lpPrm = FaceGetParameter( wrd_ );
	struct tm	_tm;
	BOOL		_flag = FALSE;
	CORETIME	_ct_begin, _ct_end;

	_tm.tm_isdst = 0;

	_tm.tm_year = (int)GetDlgItemInt( hwnd_, IDC_BEGIN_YEAR,   &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_mon	= (int)GetDlgItemInt( hwnd_, IDC_BEGIN_MONTH,  &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_mday = (int)GetDlgItemInt( hwnd_, IDC_BEGIN_DAY,    &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_hour = (int)GetDlgItemInt( hwnd_, IDC_BEGIN_HOUR,   &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_min	= (int)GetDlgItemInt( hwnd_, IDC_BEGIN_MINUTE, &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_sec	= (int)GetDlgItemInt( hwnd_, IDC_BEGIN_SECOND, &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_year -= 1900;
	_tm.tm_mon -= 1;
	_ct_begin = tm2CoreTime( &_tm );
	if( _ct_begin == -1 )
		goto _ERR;

	_tm.tm_year = (int)GetDlgItemInt( hwnd_, IDC_END_YEAR,	 &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_mon	= (int)GetDlgItemInt( hwnd_, IDC_END_MONTH,  &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_mday = (int)GetDlgItemInt( hwnd_, IDC_END_DAY,	 &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_hour = (int)GetDlgItemInt( hwnd_, IDC_END_HOUR,	 &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_min	= (int)GetDlgItemInt( hwnd_, IDC_END_MINUTE, &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_sec	= (int)GetDlgItemInt( hwnd_, IDC_END_SECOND, &_flag, TRUE ); if( !_flag ) goto _ERR;
	_tm.tm_year -= 1900;
	_tm.tm_mon -= 1;
	_ct_end = tm2CoreTime( &_tm );
	if( _ct_end == -1 )
		goto _ERR;

	if( _ct_end <= _ct_begin )
		goto _ERR2;

	MFPUWRITE( _lpPrm, HISTRD_BEGINDATETIME, CORETIME, _ct_begin );
	MFPUWRITE( _lpPrm, HISTRD_ENDDATETIME, CORETIME, _ct_end );

	return TRUE;

_ERR:
	MessageBox( hwnd_, "Sorry, illegal characters or date/time format ",
					   "Error", MB_ERROR );
	return FALSE;

_ERR2:
	MessageBox( hwnd_, "Sorry, End Date/time must be great than Begin Date/time!",
					   "Error", MB_ERROR );
	return FALSE;
}

BOOL _PenFormData( LPCELLINFO lpCellInfo_ )
{
	if( lpCellInfo_->Event & FORMEV_GETTEXT )
	{
		if( lpCellInfo_->Row == COLTITLE )
			return _PenFormGetColTitle( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Col );
		if( lpCellInfo_->Col == ROWTITLE )
			return _PenFormGetRowTitle( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Row );

		return _PenFormGetData( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Row, lpCellInfo_->Col );
	}

	if( lpCellInfo_->Event & FORMEV_SETTEXT )
		return _PenFormSetData( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->Row, lpCellInfo_->Col );

	if( lpCellInfo_->Event & FORMEV_CANEDIT )
		return TRUE;
	
	if( lpCellInfo_->Event & FORMEV_GETEDITTEXT )
		return _PenFormGetData( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Row, lpCellInfo_->Col );

	return FALSE;
}

_DLLEXP BOOL _DLLFUNC _PenFormGetData( LONG lData_, LPSTR lpvalstr_, DWORD size_, UINT row_, UINT col_ )
{
	LPHTCS				_lpHtcs = ( LPHTCS )lData_;
	WORD				_base = NUM_VAR * row_ + NUM_FIX;
	BOOL				_ret = FALSE;
	DECODE				_decode;
	GETVALUESTRING		_gvs;

	if( !_lpHtcs ) return FALSE;

	_gvs.lpszBuffer = lpvalstr_;
	_gvs.wSize = size_;
	_decode.wAccess = 0;

	switch( col_ )
	{
		case 0: //variable
			if( StampDecode( IDS_HISTREND, _lpHtcs->wrdTemp, (WORD)(_base + HISTRD_DATASOURCE), &_decode) )
			{
				CombineDecode( lpvalstr_, &_decode );
				_ret = TRUE;
			}
			else
				_ret = FALSE;

			break;

		case 1: //min
			_ret = StampGetValueString( IDS_HISTREND, _lpHtcs->wrdTemp, (WORD)(_base + HISTRD_DATAMIN), &_gvs);
			break;

		case 2: //max
			_ret = StampGetValueString( IDS_HISTREND,  _lpHtcs->wrdTemp, (WORD)(_base + HISTRD_DATAMAX), &_gvs);
			break;

		case 3: //color
			_ret = StampGetValueString( IDS_HISTREND,  _lpHtcs->wrdTemp, (WORD)(_base + HISTRD_PENCLR), &_gvs);
			break;

		case 4: //digit
			_ret = StampGetValueString( IDS_HISTREND,  _lpHtcs->wrdTemp, (WORD)(_base + HISTRD_BOOLVAR), &_gvs);
			break;

		default:
			_ret = FALSE;
	}

	return _ret;
}

_DLLEXP BOOL _DLLFUNC _PenFormSetData( LONG lData_, LPSTR lpvalstr_, UINT row_, UINT col_ )
{
	LPHTCS				_lpHtcs = ( LPHTCS )lData_;
	WORD				_base = NUM_VAR * row_ + NUM_FIX;
	BOOL				_ret = FALSE;
	ENCODE				_encode;

	_encode.lpszBuffer = lpvalstr_;
	_encode.wAccess = 0;

	if( !_lpHtcs ) return FALSE;

	switch( col_ )
	{
		case 0: //variable
			if( StampEncode( IDS_HISTREND, _lpHtcs->wrdTemp, (WORD)(_base + HISTRD_DATASOURCE), &_encode) )
				_ret = TRUE;
			else
				_ret = FALSE;

			break;

		case 1: //min
			_ret = StampSetValueString( IDS_HISTREND,  _lpHtcs->wrdTemp, (WORD)(_base + HISTRD_DATAMIN), lpvalstr_ );
			break;

		case 2: //max
			_ret = StampSetValueString( IDS_HISTREND,  _lpHtcs->wrdTemp, (WORD)(_base + HISTRD_DATAMAX), lpvalstr_ );
			break;

		case 3: //color
			_ret = StampSetValueString( IDS_HISTREND,  _lpHtcs->wrdTemp, (WORD)(_base + HISTRD_PENCLR), lpvalstr_ );
			break;

		case 4: //digit
			_ret = StampSetValueString( IDS_HISTREND,  _lpHtcs->wrdTemp, (WORD)(_base + HISTRD_BOOLVAR), lpvalstr_ );
			break;

		default:
			_ret = FALSE;
	}

	return _ret;
}


#pragma argsused
_DLLEXP BOOL _DLLFUNC _PenFormGetRowTitle( LONG lData_, LPSTR szBuffer_, DWORD dwSize_, UINT wRow_ )
{
	sprintf( szBuffer_, "%d", wRow_ + 1 );
	return TRUE;
}

#pragma argsused
_DLLEXP BOOL _DLLFUNC _PenFormGetColTitle( LONG lData_, LPSTR szBuffer_, DWORD dwSize_, UINT wCol_ )
{
	BOOL	_ret = TRUE;

	switch( wCol_ )
	{
		case 0: //variable
			strcpy( szBuffer_, "Var" );
			break;

		case 1: //min
			strcpy( szBuffer_, "Min" );
			break;

		case 2: //max
			strcpy( szBuffer_, "Max" );
			break;

		case 3: //color
			strcpy( szBuffer_, "Color" );
			break;

		case 4: //digit
			strcpy( szBuffer_, "Digit" );
			break;

		default:
			_ret = FALSE;
	}

	return _ret;
}

/*
//...................................................................
//...................................................................
static LPPU near _get_termin_pu(LPPU lpPU_)
{
_again:
	if( (lpPU_->prefix & PF_MASK) == PF_MPX )
	{
		lpPU_ = (gMTInfo.lpBase + lpPU_->du.zzk__WRD)->lpParam + ( (lpPU_->prefix) & 0x0FFF );
		goto _again;
	}
	else if( (lpPU_->prefix & PF_MASK) == PF_FPX )
	{
		lpPU_ = (gFTInfo.lpBase + lpPU_->du.zzk__WRD)->lpParam + ( (lpPU_->prefix) & 0x0FFF );
		goto _again;
	}
	else
		return lpPU_;
}
*/

