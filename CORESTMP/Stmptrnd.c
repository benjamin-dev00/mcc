//===================================================================
//
//Zheng zhikai	96-3-6
//===================================================================
#include "prehead.h"

#include "coreall.h"

#include "stmpdef.h"

#include <dos.h>
#include <math.h>
#include <time.h>

#pragma warning( disable : 4244 )	//'conversion' conversion from 'type1' to 'type2', possible loss of data

//the trend style: 
#define	STYLE_LEFTLABEL			0x00000008L
#define	STYLE_TOPLABEL			0x00000004L
#define	STYLE_RIGHTLABEL		0x00000002L
#define	STYLE_BOTTOMLABEL		0x00000001L

//define the gip between the trend chart and lebal
#define _GIP_X					(4)
#define _GIP_Y					(4)

static const unsigned long msInDay = 24L*60L*60L*1000L;
//static const unsigned long msInHour = 60L*60L*1000L;

static DWORD GetSlideTimeOffset( LPPU lpPrm_ );
static WORD OnRunTimeMsg( WRD wrd_, HWND hwnd_, LPFIA lpFia_ );

static int _near _GetCurPenMMClr( WRD wFTU_, REAL* pMin_, REAL* pMax_, CCLR* pClr_ );
static WORD _near _TrendRunDirty( WRD wFTU_, WORD dirty_ );
static BOOL _near _LogData(WRD wFTU_);
static BOOL _near _RedrawTrend(WRD wFTU_, HDC hdc_);
static BOOL _near _CalculateChartRect( HDC hdc_, WRD, LPRECT lprc_, LPRECT lprcChart_ );

static void _near _TrendDrawVerLines(HDC 	hdc_,
									 LPPU	lpPrm_,		//beginning of parameters of the trend
									 long	_Tc,		//now time, the ms escaped from 00:00:00
									 DWORD	lTotalTime_,
									 LPRECT	lprc_);		//rcChart_
static void _near _TrendDrawHrzLabel(HDC 	hdc_,
									 LPPU	lpPrm_,			//beginning of parameters of the trend
									 DWORD	dwStyle_,		//the style of the trend
									 long	_Tc,			//time now, the ms escaped from 00:00:00
									 DWORD	lTotalTime_,
									 LPRECT lprcTrend_,		//the rect of the whole trend face
									 LPRECT	lprcChar_);		//the rect of the chart of the trend: rcChart_
static void _near msToAsciiTime(long ms, LPSTR lpszBuffer_ );

enum _trend_param
{
	TRENDS_BORDERSTYLE = FACE_FIRSTPARAM,		//border line style
	TRENDS_BORDERWIDTH,		//border line width
	TRENDS_BORDERCLR,		//border line color
	TRENDS_BACKCLR,			//back color of chart

	TRENDS_LEFTLABEL,
	TRENDS_TOPLABEL,
	TRENDS_RIGHTLABEL,
	TRENDS_BOTTOMLABEL,

	TRENDS_HRZLABELCLR,		//horizontal label color
	TRENDS_HRZLABELDIV,		//horizontal label divisor

	TRENDS_VERLABELDIV,		//vertical label divisor

	TRENDS_HRZMAJLNSTYLE,	//horizontal major line style
	TRENDS_HRZMAJLNWIDTH,	//horizontal major line width
	TRENDS_HRZMAJLNCLR,		//horizontal major line color
	TRENDS_HRZMAJLNDIV,		//horizontal major line divisor

	TRENDS_HRZMINLNSTYLE,	//horizontal minor line style
	TRENDS_HRZMINLNWIDTH,	//horizontal minor line width
	TRENDS_HRZMINLNCLR,		//horizontal minor line color
	TRENDS_HRZMINLNDIV,		//horizontal minor line divisor

	TRENDS_VERMAJLNSTYLE,	//vertical major line style
	TRENDS_VERMAJLNWIDTH,	//vertical major line width
	TRENDS_VERMAJLNCLR,		//vertical major line color
	TRENDS_VERMAJLNDIV,		//vertical major line divisor

	TRENDS_VERMINLNSTYLE,	//vertical minor line style
	TRENDS_VERMINLNWIDTH,	//vertical minor line width
	TRENDS_VERMINLNCLR,		//vertical minor line color
	TRENDS_VERMINLNDIV,		//vertical minor line divisor

	TRENDS_SLIDESTYLE,		//slide line style
	TRENDS_SLIDEWIDTH,		//style line width
	TRENDS_SLIDECLR,		//style line color
	TRENDS_SLIDETIME,		//style line time, which is offset from the trend start-point time

	TRENDS_SAMPLEINTERVAL,	//sample interval (in ms.)
	TRENDS_TOTALTIME,		//total time (in ms.)
	TRENDS_SC,				//step counter( decease counter )
	TRENDS_SCINIT,			//initialize value of SC
	TRENDS_TOTALITEM,		//total item in data cache
	TRENDS_VALIDITEM,		//valid item in data cache
	TRENDS_ADDING,			//adding point in data cache

	TRENDS_CURRENTPEN,		//current pen
};

///////
#define		TRENDS_LEFTLABEL_PDT		PDT_BOOL
#define		TRENDS_TOPLABEL_PDT			PDT_BOOL
#define		TRENDS_RIGHTLABEL_PDT		PDT_BOOL
#define		TRENDS_BOTTOMLABEL_PDT		PDT_BOOL
#define 	TRENDS_BACKCLR_PDT			PDT_COLORREF
#define 	TRENDS_BORDERSTYLE_PDT		PDT_int
#define 	TRENDS_BORDERWIDTH_PDT		PDT_DWORD
#define 	TRENDS_BORDERCLR_PDT		PDT_COLORREF
#define 	TRENDS_HRZLABELCLR_PDT		PDT_COLORREF
#define 	TRENDS_HRZMAJLNSTYLE_PDT	PDT_int
#define 	TRENDS_HRZMAJLNWIDTH_PDT	PDT_int
#define 	TRENDS_HRZMAJLNCLR_PDT		PDT_COLORREF
#define 	TRENDS_HRZMINLNSTYLE_PDT	PDT_int
#define 	TRENDS_HRZMINLNWIDTH_PDT	PDT_int
#define 	TRENDS_HRZMINLNCLR_PDT		PDT_COLORREF
#define 	TRENDS_VERMAJLNSTYLE_PDT	PDT_int
#define 	TRENDS_VERMAJLNWIDTH_PDT	PDT_int
#define 	TRENDS_VERMAJLNCLR_PDT		PDT_COLORREF
#define 	TRENDS_VERMINLNSTYLE_PDT	PDT_int
#define 	TRENDS_VERMINLNWIDTH_PDT	PDT_int
#define 	TRENDS_VERMINLNCLR_PDT		PDT_COLORREF
#define 	TRENDS_HRZLABELDIV_PDT  	PDT_WORD
#define 	TRENDS_VERLABELDIV_PDT  	PDT_WORD
#define 	TRENDS_HRZMAJLNDIV_PDT		PDT_WORD
#define 	TRENDS_HRZMINLNDIV_PDT		PDT_WORD
#define 	TRENDS_VERMAJLNDIV_PDT		PDT_WORD
#define 	TRENDS_VERMINLNDIV_PDT		PDT_WORD
#define 	TRENDS_SLIDESTYLE_PDT		PDT_int
#define 	TRENDS_SLIDEWIDTH_PDT		PDT_int
#define 	TRENDS_SLIDECLR_PDT			PDT_COLORREF
#define 	TRENDS_SLIDETIME_PDT		PDT_DWORD
#define 	TRENDS_SAMPLEINTERVAL_PDT	PDT_DWORD
#define		TRENDS_TOTALTIME_PDT		PDT_DWORD
#define		TRENDS_SC_PDT				PDT_DWORD
#define		TRENDS_SCINIT_PDT			PDT_DWORD
#define		TRENDS_TOTALITEM_PDT		PDT_WORD
#define		TRENDS_VALIDITEM_PDT		PDT_WORD
#define		TRENDS_ADDING_PDT			PDT_WORD
#define 	TRENDS_CURRENTPEN_PDT		PDT_int

//Trend parameter order - varible partion
#define		TRENDS_DATASOURCE		0
#define     TRENDS_MIN				1
#define     TRENDS_MAX				2
#define		TRENDS_DATACACHE		3
#define		TRENDS_SLIDEVALUE		4
#define		TRENDS_PENSTYLE			5
#define		TRENDS_PENWIDTH			6
#define		TRENDS_PENCLR			7

//Trend parameter type - varible partion
#define		TRENDS_DATASOURCE_PDT	PDT_REAL
#define		TRENDS_MIN_PDT			PDT_REAL
#define		TRENDS_MAX_PDT			PDT_REAL
#define		TRENDS_DATACACHE_PDT	PDT_FPTR
#define		TRENDS_SLIDEVALUE_PDT	PDT_REAL
#define		TRENDS_PENSTYLE_PDT		PDT_int
#define		TRENDS_PENWIDTH_PDT		PDT_int
#define		TRENDS_PENCLR_PDT		PDT_COLORREF

static PDU	sfixPDU[] = {
	_PDU_DEFAULT,

	_ELE_PDU(TRENDS_BORDERSTYLE, 	"BorderStyle", 		VSOF_int_RECTBS,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_BORDERWIDTH, 	"BorderWidth", 		VSOF_DWORD_BWIDTH,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_BORDERCLR,		"BorderColor", 		VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_BACKCLR,		"BackColor", 		VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),

	_ELE_PDU(TRENDS_LEFTLABEL,		"LeftLabel",		VSOF_BOOL_ONOFF,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_TOPLABEL,		"TopLabel",			VSOF_BOOL_ONOFF,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_RIGHTLABEL,		"RightLabel",		VSOF_BOOL_ONOFF,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_BOTTOMLABEL,	"BottomLabel",		VSOF_BOOL_ONOFF,	PU_NORMAL, PRI_ALL ),

	_ELE_PDU(TRENDS_HRZLABELCLR,	"HrzLabelColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_HRZLABELDIV,	"HrzLabelDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(TRENDS_VERLABELDIV,	"VerLabelDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(TRENDS_HRZMAJLNSTYLE,	"HrzMajLnStyle", 	VSOF_int_LNSTYLE,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_HRZMAJLNWIDTH,	"HrzMajLnWidth", 	VSOF_int,			PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_HRZMAJLNCLR,	"HrzMajLnColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_HRZMAJLNDIV,	"HrzMajLnDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(TRENDS_HRZMINLNSTYLE,	"HrzMinLnStyle", 	VSOF_int_LNSTYLE,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_HRZMINLNWIDTH,	"HrzMinLnWidth", 	VSOF_int,			PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_HRZMINLNCLR,	"HrzMinLnColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_HRZMINLNDIV,	"HrzMinLnDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(TRENDS_VERMAJLNSTYLE,	"VerMajLnStyle", 	VSOF_int_LNSTYLE,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_VERMAJLNWIDTH,	"VerMajLnWidth", 	VSOF_int,			PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_VERMAJLNCLR,	"VerMajLnColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_VERMAJLNDIV,	"VerMajLnDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(TRENDS_VERMINLNSTYLE,	"VerMinLnStyle", 	VSOF_int_LNSTYLE,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_VERMINLNWIDTH,	"VerMinLnWidth", 	VSOF_int,			PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_VERMINLNCLR,	"VerMinLnColor", 	VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_VERMINLNDIV,	"VerMinLnDiv",		VSOF_WORD,			PU_NORMAL, PRI_ALL ),

	_ELE_PDU(TRENDS_SLIDESTYLE,		"SlideStyle", 		VSOF_int_LNSTYLE,	PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_SLIDEWIDTH,		"SlideWidth", 		VSOF_int,			PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_SLIDECLR,		"SlideColor", 		VSOF_DWORD_CLR,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_SLIDETIME,		"SlideTimeOffset", 	VSOF_DWORD_MS,		PU_NORMAL, PRI_ALL ),

	_ELE_PDU(TRENDS_SAMPLEINTERVAL,	"SampleTime",		VSOF_DWORD_MS,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_TOTALTIME,		"TotalTime",		VSOF_DWORD_MS,		PU_NORMAL, PRI_ALL ),
	_ELE_PDU(TRENDS_SC,				"SC",				VSOF_DWORD, 		PU_NIV|PU_IGNOREDIRTY,		PRI_DEV|PDUF_NODEFAULT ),	//step counter( decease counter )
	_ELE_PDU(TRENDS_SCINIT,			"SCInit",			VSOF_DWORD, 		PU_NIV,						PRI_DEV|PDUF_NODEFAULT ),	//initialize value of step counter
	_ELE_PDU(TRENDS_TOTALITEM,		"TotalItem",		VSOF_WORD, 			PU_NIV,						PRI_DEV|PDUF_NODEFAULT ),	//the sizeof the data cache
	_ELE_PDU(TRENDS_VALIDITEM,		"ValidItem",		VSOF_WORD, 			PU_NIV|PU_IGNOREDIRTY,		PRI_DEV|PDUF_NODEFAULT ),	//the number of valid data in cache
	_ELE_PDU(TRENDS_ADDING,			"Adding",			VSOF_WORD, 			PU_NIV|PU_IGNOREDIRTY,		PRI_DEV|PDUF_NODEFAULT ),

	_ELE_PDU(TRENDS_CURRENTPEN,		"CurrentPen",		VSOF_int,			PU_NIV,	   PRI_DEV|PDUF_NODEFAULT ),
};

static PDU	svarPDU[] = {
	_ELE_PDU(TRENDS_DATASOURCE,		"DataSource",	VSOF_REAL, 			PU_IGNOREDIRTY,	PRI_ALL|PDUF_NODEFAULT ),
	_ELE_PDU(TRENDS_MIN,			"Min",			VSOF_REAL, 			PU_NORMAL,		PRI_ALL ),
	_ELE_PDU(TRENDS_MAX,			"Max",			VSOF_REAL, 			PU_NORMAL,		PRI_ALL ),
	_ELE_PDU(TRENDS_DATACACHE,		"DataCache",	VSOF_FPTR,			PU_NIV,			PRI_DEV|PDUF_NODEFAULT ),
	_ELE_PDU(TRENDS_SLIDEVALUE,		"SlideValue",	VSOF_REAL, 			PU_NIV,			PRI_ALL ),
	_ELE_PDU(TRENDS_PENSTYLE, 		"PenStyle", 	VSOF_int_LNSTYLE, 	PU_NORMAL,		PRI_ALL ),
	_ELE_PDU(TRENDS_PENWIDTH, 		"PenWidth", 	VSOF_int, 			PU_NORMAL,		PRI_ALL ),
	_ELE_PDU(TRENDS_PENCLR,   		"PenColor", 	VSOF_DWORD_CLR,		PU_NORMAL,		PRI_ALL )
};

#define  NUM_VAR  (sizeof(svarPDU)/sizeof(svarPDU[0]))
#define  NUM_FIX  (sizeof(sfixPDU)/sizeof(sfixPDU[0]))

//...................................................................
//...................................................................
_DLLEXP BOOL _DLLFUNC TrendPaint(WRD wFTU_, HDC hdc_)
{
	_RedrawTrend( wFTU_, hdc_ );

	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC TrendParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static VPSTRUCT	_sVPD = {
		1,			//the default number of var parameters groups
		0,			//the low bound of var counter
		NUM_FIX,
		&( sfixPDU[0] ),
		NUM_VAR,
		&( svarPDU[0] )
	};

	switch( wCmd_ )
	{
		case PIC_CREATE:
		{
			STATUS _status = FaceGetStatus( wFTU_ );
			_status |= FTUBIT_WANTTICK;
			FaceSetStatus( wFTU_, _status );
			return DefProcessVarPIC(wCmd_, wFTU_, wNo_, lParam_, TBX_FT, IDS_TREND, &_sVPD);
		}

		case PIC_INITIATE:
		{
			//Here:
			//	calculate the TotalItem again, if the new TotalItem
			//	is equal to the old value, ONLY allocate the caches
			//	for the pens which are without the cache. Otherwise,
			//	free all the old caches and allocate new cache for
			//	all pens.

			DWORD		_dwSCInit;
			DWORD		_dwSampleTime, _dwTotalTime;
			WORD		_wNumPen, _wTotalItem_old, _wTotalItem;
			LPPU		_lpPrm = FaceGetParameter(wFTU_);
			LPPU		_lpPrmVar;
			unsigned	_i;
			RECT		_rc;

			FaceXycxcyToRect(wFTU_, &_rc);
			FaceSetRect( wFTU_, &_rc );		//set new face rect

			MFPUREAD( _lpPrm, TRENDS_TOTALITEM, WORD, _wTotalItem_old );
			MFPUREAD( _lpPrm, TRENDS_SAMPLEINTERVAL, DWORD, _dwSampleTime );
			MFPUREAD( _lpPrm, TRENDS_TOTALTIME, DWORD, _dwTotalTime );

			//calculate new TotalItem
			_wTotalItem = (WORD)(_dwTotalTime / _dwSampleTime) + 1;

			if( _wTotalItem != _wTotalItem_old )	//write the new value
				MFPUWRITE( _lpPrm, TRENDS_TOTALITEM, WORD, _wTotalItem );

			_dwSCInit = _dwSampleTime / lParam_;
			if( _dwSCInit )
				_dwSCInit --;	//Note: zero is valid value
			MFPUWRITE( _lpPrm, TRENDS_SCINIT, DWORD, _dwSCInit );

			_wNumPen = ( FaceGetNumParam( wFTU_ ) - NUM_FIX ) / NUM_VAR;

			//check the caches
			for( _i=0, _lpPrmVar = _lpPrm + NUM_FIX;
				 _i<_wNumPen;
				 _i++, _lpPrmVar += NUM_VAR)
			{
				FPTR	_lpData;

				MFPUREAD( _lpPrmVar, TRENDS_DATACACHE, FPTR, _lpData );
				if( _lpData )
				{
					if( _wTotalItem == _wTotalItem_old )
					{
						//at the old version, this initialization is only for the new
						//caches, now, we initialize these caches too
						MFPUWRITE( _lpPrm, TRENDS_ADDING, WORD, 0);
						MFPUWRITE( _lpPrm, TRENDS_VALIDITEM, WORD, 0);
						continue;	//skip this and to next
					}
					else
					{
						FragFree( _lpData );
						MFPUWRITE( _lpPrmVar, TRENDS_DATACACHE, FPTR, NULL );
					}
				}

				//now, the cache is not allocated or has been freed
				_lpData = FragAlloc( _wTotalItem * sizeof(REAL) );
				if( _lpData == NULL )
				{
					return FALSE; 	//allocate memory failed
				}
				else
				{
					MFPUWRITE( _lpPrmVar, TRENDS_DATACACHE, FPTR, _lpData );

					//initialize the new cache
					MFPUWRITE( _lpPrm, TRENDS_ADDING, WORD, 0);
					MFPUWRITE( _lpPrm, TRENDS_VALIDITEM, WORD, 0);
				}

			}	//end for

			return TRUE;
		}

		default:
			return DefProcessVarPIC(wCmd_, wFTU_, wNo_, lParam_, TBX_FT, IDS_TREND, &_sVPD);
	}
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC TrendBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_INIT:
		{
			LPFINIT	_lpFinit = (LPFINIT)lParam_;
			LPPU	_lpPrm, _lpPrmVar;
			DWORD	_defStyle;
			WORD	_wNumParam;
			div_t	_r_div;
			int		_i;

			_lpPrm  = FaceGetParameter( wFTU_ );
			_wNumParam = FaceGetNumParam( wFTU_ );
			_defStyle = STYLE_BOTTOMLABEL|STYLE_LEFTLABEL;

			MFPUWRITE( _lpPrm, TRENDS_LEFTLABEL, BOOL,  TRUE );
			MFPUWRITE( _lpPrm, TRENDS_TOPLABEL, BOOL, FALSE );
			MFPUWRITE( _lpPrm, TRENDS_RIGHTLABEL, BOOL, FALSE );
			MFPUWRITE( _lpPrm, TRENDS_BOTTOMLABEL, BOOL, TRUE );

			MFPUWRITE(_lpPrm, TRENDS_BORDERSTYLE,	int, 		_lpFinit->border_pen.lopnStyle );
			MFPUWRITE(_lpPrm, TRENDS_BORDERWIDTH,	DWORD, 		_lpFinit->border_pen.lopnWidth.x );
			MFPUWRITE(_lpPrm, TRENDS_BORDERCLR, 	COLORREF,	_lpFinit->border_pen.lopnColor );
			MFPUWRITE(_lpPrm, TRENDS_BACKCLR,		COLORREF,	_lpFinit->back_clr);

			MFPUWRITE(_lpPrm, TRENDS_HRZMAJLNSTYLE,	int, 		PS_SOLID );
			MFPUWRITE(_lpPrm, TRENDS_HRZMAJLNWIDTH,	int, 		1 );
			MFPUWRITE(_lpPrm, TRENDS_HRZMAJLNCLR, 	COLORREF,	RGB(0,127,0) );

			MFPUWRITE(_lpPrm, TRENDS_HRZMINLNSTYLE,	int, 		PS_DOT);
			MFPUWRITE(_lpPrm, TRENDS_HRZMINLNWIDTH,	int, 		1);
			MFPUWRITE(_lpPrm, TRENDS_HRZMINLNCLR, 	COLORREF,	RGB(0,127,0) );

			MFPUWRITE(_lpPrm, TRENDS_VERMAJLNSTYLE,	int, 		PS_SOLID );
			MFPUWRITE(_lpPrm, TRENDS_VERMAJLNWIDTH,	int, 		1 );
			MFPUWRITE(_lpPrm, TRENDS_VERMAJLNCLR, 	COLORREF,	RGB(0,127,0) );

			MFPUWRITE(_lpPrm, TRENDS_VERMINLNSTYLE,	int, 		PS_DOT );
			MFPUWRITE(_lpPrm, TRENDS_VERMINLNWIDTH,	int, 		1 );
			MFPUWRITE(_lpPrm, TRENDS_VERMINLNCLR, 	COLORREF,	RGB(0,127,0) );

			MFPUWRITE(_lpPrm, TRENDS_SLIDESTYLE,	int, 		PS_DOT );
			MFPUWRITE(_lpPrm, TRENDS_SLIDEWIDTH,	int, 		1 );
			MFPUWRITE(_lpPrm, TRENDS_SLIDECLR, 		COLORREF,	RGB(255,255,0) );

			MFPUWRITE(_lpPrm, TRENDS_HRZLABELCLR,	COLORREF,	_lpFinit->text_clr);

			MFPUWRITE(_lpPrm, TRENDS_VERLABELDIV,	WORD,		2);
			MFPUWRITE(_lpPrm, TRENDS_HRZLABELDIV,	WORD,		2);
			MFPUWRITE(_lpPrm, TRENDS_HRZMAJLNDIV,	WORD,		2);
			MFPUWRITE(_lpPrm, TRENDS_HRZMINLNDIV,	WORD,		5);
			MFPUWRITE(_lpPrm, TRENDS_VERMAJLNDIV,	WORD,		2);
			MFPUWRITE(_lpPrm, TRENDS_VERMINLNDIV,	WORD,		3);

			//calculate the number of data source depend on the number of parameter
			_r_div = div( _wNumParam - NUM_FIX, NUM_VAR );
			CORE_ASSERT( _r_div.rem == 0 );

			MFPUWRITE(_lpPrm, TRENDS_TOTALTIME,			DWORD, 1L*60L*1000L );	//1 minutes
			MFPUWRITE(_lpPrm, TRENDS_SAMPLEINTERVAL,	DWORD, 500L );		//0.5 seconds
			MFPUWRITE(_lpPrm, TRENDS_SLIDETIME, 		DWORD,	30L*1000L );	//half total time

			MFPUWRITE(_lpPrm, TRENDS_TOTALITEM, WORD, NULL);
			MFPUWRITE(_lpPrm, TRENDS_VALIDITEM, WORD, NULL);
			MFPUWRITE(_lpPrm, TRENDS_ADDING, WORD, NULL);

			for(_i=0, _lpPrmVar = _lpPrm + NUM_FIX;
				_i<_r_div.quot;
				_i++, _lpPrmVar += NUM_VAR )
			{
				MFPUWRITE(_lpPrmVar, TRENDS_DATASOURCE, REAL, 0);

				MFPUWRITE(_lpPrmVar, TRENDS_MIN, REAL, 0);
				MFPUWRITE(_lpPrmVar, TRENDS_MAX, REAL, 100);

				MFPUWRITE(_lpPrmVar, TRENDS_DATACACHE, FPTR, NULL);

				MFPUWRITE(_lpPrmVar, TRENDS_SLIDEVALUE, REAL, 0);

				MFPUWRITE(_lpPrmVar, TRENDS_PENSTYLE,  int,	PS_SOLID );
				MFPUWRITE(_lpPrmVar, TRENDS_PENWIDTH,  int, 1 );
				MFPUWRITE(_lpPrmVar, TRENDS_PENCLR,	  COLORREF, RGB(0,255,0) );
			}
			break;
		}

		case SCMD_RUNTIME:
		{
			LPPU		_lpPrm;
			int			_currentPen;

			WORD		_wNumParam;
			WORD		_nNumPen;
			div_t		_divt;
			LPFIA		_lpFia = (LPFIA)lParam_;

			if(    (_lpFia->MsgCode & MSG_TRIGGER) 
				&& (_lpFia->msg != WM_LBUTTONDOWN) )	//this message is used for drag slide line
			{
				_lpPrm  = FaceGetParameter( wFTU_ );
				_wNumParam = FaceGetNumParam(wFTU_);
				_divt = div( _wNumParam - NUM_FIX, NUM_VAR );
				CORE_ASSERT( _divt.rem == 0 );
				_nNumPen = _divt.quot;

				MFPUREAD( _lpPrm, TRENDS_CURRENTPEN, int, _currentPen );
				_currentPen++;
				if( (_currentPen<0) || (_currentPen>=_nNumPen) )
					_currentPen = 0;
				MFPUWRITE( _lpPrm, TRENDS_CURRENTPEN, int, _currentPen );

				return FALSE;
			}
			else if(_lpFia->msg == WM_LBUTTONDOWN)
			{
				return RT_CAPTURE;
			}
			else
				return FALSE;
		}

		case SCMD_RUNTIME_INPUTMSG:
		{
			//must be dragging slide line
			return OnRunTimeMsg( wFTU_, hwnd_, (LPFIA)lParam_ );
		}

		case SCMD_TICK:
		{
			if( _LogData( wFTU_ ) )	//need to redraw
			{
				FaceSetRedraw( wFTU_ );
			}
			return 0L;
		}

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
	DWORD       _dwStyle;
	DWORD		_dwBorderWidth;
	float		_min, _max, _delta_value, _vl;
	WORD		_wNumParam;
	WORD		_wVerLabelDiv;
	CCLR		_clrBk, _clrPen, _clrText;
	COLORREF	_rgbText;
	int			_nPenWidth, _nPenStyle;
	HPEN		_hPen;
	HBRUSH		_hBrush;
	int			_i, _j;
	LPPU		_lpPrm, _lpPrmVar;
	WORD		_wMajorDiv, _wMinorDiv;
	float		_yj, _y, _delta, _delta_j;
	char		_szBuffer[32];
	DWORD		_lTotalTime;
	WORD		_wTotalItem, _wValidItem;
	DWORD		_dwSlideTimeOffset;

	WORD		_nNumPen;
	float		_fact, _offset;
	float		_data_min, _data_max;	//each pen's max and min
	WORD		_wAdding;

	div_t		_divt;
	long		_Tc;		//time now, escaped ms from 00:00:00

	HDC			_hdcDraw;
	BOOL		_retClr;	//return value by CclrGetRGB

#ifdef _USE_OWN_HDC
	HDC			_hdcMem;
	HBITMAP		_hbmpMem, _hbmpMemOld;
#endif

	//the meaning of the following four variables
	//references to tech. Note;
	DWORD		_sc;
	WORD		_wNN;
	float		_cit_f;
	long		_cit_l;

	int			_currentPen;

	FaceXycxcyToRect(wFTU_, &_rc);
	CopyRect( &_rcBu, &_rc );

	_lpPrm = FaceGetParameter( wFTU_ );
	MFPUREAD( _lpPrm, TRENDS_LEFTLABEL, BOOL, _bLeftLabel );
	MFPUREAD( _lpPrm, TRENDS_TOPLABEL, BOOL, _bTopLabel );
	MFPUREAD( _lpPrm, TRENDS_RIGHTLABEL, BOOL, _bRightLabel );
	MFPUREAD( _lpPrm, TRENDS_BOTTOMLABEL, BOOL, _bBottomLabel );
	_dwStyle = 0;
	if( _bLeftLabel )	_dwStyle |= STYLE_LEFTLABEL;
	if( _bTopLabel )	_dwStyle |= STYLE_TOPLABEL;
	if( _bRightLabel )	_dwStyle |= STYLE_RIGHTLABEL;
	if( _bBottomLabel )	_dwStyle |= STYLE_BOTTOMLABEL;

	_wNumParam = FaceGetNumParam(wFTU_);
	_divt = div( _wNumParam - NUM_FIX, NUM_VAR );
	CORE_ASSERT( _divt.rem == 0 );
	_nNumPen = _divt.quot;

	MFPUREAD( _lpPrm, TRENDS_VERLABELDIV, WORD, _wVerLabelDiv );
	if( !_wVerLabelDiv ) _wVerLabelDiv = 1;
	MFPUREAD( _lpPrm, TRENDS_TOTALITEM, WORD, _wTotalItem);
	MFPUREAD( _lpPrm, TRENDS_VALIDITEM, WORD, _wValidItem);
	MFPUREAD( _lpPrm, TRENDS_ADDING, WORD, _wAdding);

	if( CoreIsRunTime() )
	{
#ifdef WIN32
		SYSTEMTIME	_st;
		GetLocalTime(&_st);
		_Tc = (((_st.wHour*60L + _st.wMinute)*60L + _st.wSecond)*1000L + _st.wMilliseconds);
#else
		struct dostime_t	_curTime;
		_dos_gettime( &_curTime );
		_Tc = (((_curTime.hour*60L + _curTime.minute)*60L + _curTime.second)*100L + _curTime.hsecond)*10L;
#endif	
	}
	else	//build-time
	{
		//set the time begin at 00:00:00
		MFPUREAD( _lpPrm, TRENDS_TOTALTIME, DWORD, _Tc);
	}
	
	_hdcDraw = hdc_;

	//save current DC before changing the clip
	//rectangle, after drawing data-tracks, we
	//shall restore the current dc;
	SaveDC( _hdcDraw );		//limit to draw in face box
	IntersectClipRect( _hdcDraw,
					   _rc.left, _rc.top,
					   _rc.right, _rc.bottom );

	SelectObject( _hdcDraw, GetStockObject( ANSI_VAR_FONT ) );
	SetBkMode( _hdcDraw, TRANSPARENT );

	GetTextMetrics( _hdcDraw, &_tm );

	_CalculateChartRect( _hdcDraw, wFTU_, &_rc, &_rcChart );

	//draw left labels and right labels
	//
	_currentPen = _GetCurPenMMClr( wFTU_, &_min, &_max, &_clrText );
	_retClr = CclrGetRGB( _clrText, &_rgbText );
	SetTextColor( hdc_, _rgbText );

	_delta = (_rcChart.bottom - _rcChart.top) / _wVerLabelDiv;
	_delta_value = (_max - _min) / _wVerLabelDiv;
	for( _i=0, _vl = _min, _y = _rcChart.bottom;
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
						_rcChart.left - _GIP_X,	_y );
				if( _retClr )	//otherwise, transparent
					DrawText(_hdcDraw, _szBuffer, _str_len, &_rcLabel, DT_RIGHT|DT_BOTTOM|DT_SINGLELINE);
			}
			else if( _i == _wVerLabelDiv )
			{
				SetRect(&_rcLabel,
						_rc.left, _y,
						_rcChart.left - _GIP_X, _y+_tm.tmHeight);
				if( _retClr )	//otherwise, transparent
					DrawText(_hdcDraw, _szBuffer, _str_len, &_rcLabel, DT_RIGHT|DT_TOP|DT_SINGLELINE);
			}
			else
			{
				SetRect(&_rcLabel,
						_rc.left, _y-_tm.tmHeight,
						_rcChart.left - _GIP_X, _y+_tm.tmHeight);
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
	MFPUREAD( _lpPrm, TRENDS_TOTALTIME, DWORD, _lTotalTime );
	_TrendDrawHrzLabel( _hdcDraw, _lpPrm, _dwStyle, _Tc, _lTotalTime, &_rc, &_rcChart);

	//draw the chart WITHOUT border
	MFPUREAD( _lpPrm, TRENDS_BACKCLR, DWORD, _clrBk );
	_hBrush = CoreCreateBrush(_clrBk);
	_hPen = SelectObject(_hdcDraw, GetStockObject(NULL_PEN) );
	_hBrush = SelectObject(_hdcDraw, _hBrush);
	Rectangle( _hdcDraw, _rcChart.left, _rcChart.top, _rcChart.right, _rcChart.bottom );
	SelectObject(_hdcDraw, _hPen);
	_hBrush = SelectObject(_hdcDraw, _hBrush);
	CoreDeleteBrush(_hBrush);

	//draw the major hrz lines
	MFPUREAD( _lpPrm, TRENDS_HRZMAJLNSTYLE, int, _nPenStyle );
	MFPUREAD( _lpPrm, TRENDS_HRZMAJLNWIDTH, int, _nPenWidth );
	MFPUREAD( _lpPrm, TRENDS_HRZMAJLNCLR, DWORD, _clrPen );
	MFPUREAD( _lpPrm, TRENDS_HRZMAJLNDIV, WORD, _wMajorDiv );
	if( !_wMajorDiv ) _wMajorDiv = 1;

	_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrPen);
	_hPen = SelectObject(_hdcDraw, _hPen);
	_delta = (_rcChart.bottom - _rcChart.top) / _wMajorDiv;
	for( _i=1, _y=_rcChart.top+_delta;
		_i<_wMajorDiv;
		_i++, _y += _delta)
	{
		MoveTo(_hdcDraw, _rcChart.left, _y);
		LineTo(_hdcDraw, _rcChart.right, _y);
	}
	_hPen = SelectObject(_hdcDraw, _hPen);
	CoreDeletePen(_hPen);

	//draw the minor hrz lines
	MFPUREAD( _lpPrm, TRENDS_HRZMINLNSTYLE, int, _nPenStyle );
	MFPUREAD( _lpPrm, TRENDS_HRZMINLNWIDTH, int, _nPenWidth );
	MFPUREAD( _lpPrm, TRENDS_HRZMINLNCLR, DWORD, _clrPen );
	MFPUREAD( _lpPrm, TRENDS_HRZMINLNDIV, WORD, _wMinorDiv );
	if( !_wMinorDiv ) _wMinorDiv = 1;

	_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrPen);
	_hPen = SelectObject(_hdcDraw, _hPen);
	_delta_j = _delta / _wMinorDiv;
	for( _i=0, _y=_rcChart.top;
		_i<_wMajorDiv;
		_i++, _y += _delta)
	{
		for( _j=1, _yj=_y + _delta_j;
			_j<_wMinorDiv;
			_j++, _yj += _delta_j)
		{
			MoveTo(_hdcDraw, _rcChart.left, _yj);
			LineTo(_hdcDraw, _rcChart.right, _yj);
		}
	}
	_hPen = SelectObject(_hdcDraw, _hPen);
	CoreDeletePen(_hPen);

	//draw vertical line
	_TrendDrawVerLines(_hdcDraw, _lpPrm, _Tc, _lTotalTime, &_rcChart);

	//draw the data track lines
	//

	//dont calculate _fact and _offset here
	//now, each pen has it own _fact and offset
	//
	//_fact = (_rcChart.top - _rcChart.bottom + 1)/(_max - _min);
	//_offset = _rcChart.top - _max * _fact;
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

	//get the total count of log data from the core ticks
	//to avoid the overflow while do following calculation;
	//we only use the LOWORD of the result; the details of
	//this section code references to Tech. note;
	MFPUREAD( _lpPrm, TRENDS_SCINIT, DWORD, _sc);
	_wNN = LOWORD( (DWORD)( CoreGetTicks()/(_sc+1) ) );

	for(_i=0;
		_i<_nNumPen;
		_i++, _lpPrmVar += NUM_VAR )
	{
		REAL _far*	_lpData;
		float		_x, _y;
		int			_j;
		WORD		_w;
		float	_delta_x;

		MFPUREAD( _lpPrmVar, TRENDS_PENSTYLE, int, _nPenStyle );
		MFPUREAD( _lpPrmVar, TRENDS_PENWIDTH, int, _nPenWidth );
		MFPUREAD( _lpPrmVar, TRENDS_PENCLR, DWORD, _clrPen );

		MFPUREAD( _lpPrmVar, TRENDS_MIN, REAL, _data_min );
		MFPUREAD( _lpPrmVar, TRENDS_MAX, REAL, _data_max );

		_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrPen);
		_hPen = SelectObject(_hdcDraw, _hPen);

		MFPUREAD( _lpPrmVar, TRENDS_DATACACHE, FPTR, _lpData);
		if( !_lpData )	//no data
		{
			if( !CoreIsRunTime() )
			{
				MoveTo( _hdcDraw, _rcChart.left, _rcChart.top + (_rcChart.bottom - _rcChart.top) *_i /_nNumPen );
				LineTo( _hdcDraw, _rcChart.right, _rcChart.top + (_rcChart.bottom - _rcChart.top) *(_i+1) /_nNumPen );
			}
		}
		else
		{
			_w = _wAdding;
			_delta_x = ( (float)(_rcChart.right - 1 - _rcChart.left) )/( (float)(_wTotalItem-1) );

			if( _w )
				_w --;
			else
				_w = _wTotalItem - 1;

			_cit_f = _delta_x * _wNN - _rcChart.right * 1.0 - 1.0;
			_cit_l = _cit_f;

			if( _data_max - _data_min )
			{
				_fact = (float)(_rcChart.top - _rcChart.bottom + 1)/(float)(_data_max - _data_min);
				_offset = _rcChart.top - _data_max * _fact;
			}
			else
				continue;	//this pen is invalid

			_x = _delta_x * _wNN - _cit_l;
			_y = ( (float)(_lpData[_w]) ) * _fact + _offset;
			MoveTo(_hdcDraw, _x, _y);

			//LOGMSG2( "_wNN=%d, _x=%f", _wNN, _x );

			for(_j=1; _j<_wValidItem; _j++)
			{
				if( _w )
					_w --;
				else
					_w = _wTotalItem - 1;

				_y = ( (float)(_lpData[_w]) )*_fact + _offset;
				_x -= _delta_x;
				LineTo(_hdcDraw, _x, _y);
			}
		}	//end if the data cache is valid

		_hPen = SelectObject(_hdcDraw, _hPen);
		CoreDeletePen(_hPen);
	}
	//restore clipping region
	RestoreDC( _hdcDraw, -1 );

	//draw the chart BORDER
	MFPUREAD( _lpPrm, TRENDS_BORDERSTYLE, int, _nPenStyle );
	MFPUREAD( _lpPrm, TRENDS_BORDERWIDTH, DWORD, _dwBorderWidth );
	MFPUREAD( _lpPrm, TRENDS_BORDERCLR, DWORD, _clrPen );
	CoreDrawBox( _hdcDraw, &_rcChart, _nPenStyle, _dwBorderWidth, _clrPen );

	//at the last, draw slide line
	MFPUREAD( _lpPrm, TRENDS_SLIDESTYLE, int, _nPenStyle );
	MFPUREAD( _lpPrm, TRENDS_SLIDEWIDTH, int, _nPenWidth );
	MFPUREAD( _lpPrm, TRENDS_SLIDECLR, DWORD, _clrPen );
	_dwSlideTimeOffset = GetSlideTimeOffset(_lpPrm);
	if( _dwSlideTimeOffset>0 && _dwSlideTimeOffset<_lTotalTime )
	{
		int	_slide_x;

		_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrPen);
		_hPen = SelectObject(_hdcDraw, _hPen);

		_slide_x = _rcChart.left + (_rcChart.right - _rcChart.left) * _dwSlideTimeOffset / _lTotalTime;

		MoveTo( _hdcDraw, _slide_x, _rcChart.top );
		LineTo( _hdcDraw, _slide_x, _rcChart.bottom );

		_hPen = SelectObject(_hdcDraw, _hPen);
		CoreDeletePen(_hPen);
	}
	//otherwise, slip draw it

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
static void _near _TrendDrawVerLines(HDC 	hdc_,
									 LPPU	lpPrm_,		//beginning of parameters of the trend
									 long	_Tc,		//now time, the ms escaped from 00:00:00
									 DWORD	lTotalTime_,
									 LPRECT	lprc_)		//rcChart_
{
	ldiv_t		_qr;
	int			_cDiv, _cRem, _cdiv;
	WORD		_wdv;
	long		_Tdiv;			//all in ms
	int			_cxChart;
	int			_nPenWidth, _nPenStyle;
	COLORREF	_clrPen;
	HPEN		_hPen;
	int			_i, _j, _x;
	WORD		_wdv_min;
	long		_tdiv;

	_cxChart = lprc_->right - lprc_->left;

	MFPUREAD( lpPrm_, TRENDS_VERMAJLNDIV, WORD, _wdv );
	

	if( _wdv == 0 )	_wdv++;
	_Tdiv = lTotalTime_ / _wdv;
	if( !_Tdiv )
		return;

	//draw the Vertical Major lines
	_qr = ldiv(_Tc, _Tdiv);
	_cRem = (int)( ((float)_qr.rem/(float)lTotalTime_) * (float)_cxChart );
	_cDiv = (int)( ((float)_Tdiv/(float)lTotalTime_) * (float)_cxChart );

	//draw the Ver major lines
	MFPUREAD( lpPrm_, TRENDS_VERMAJLNSTYLE, int, _nPenStyle );
	MFPUREAD( lpPrm_, TRENDS_VERMAJLNWIDTH, int, _nPenWidth );
	MFPUREAD( lpPrm_, TRENDS_VERMAJLNCLR, COLORREF, _clrPen );
	_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrPen);
	_hPen = SelectObject(hdc_, _hPen);

	for( _i = lprc_->right - _cRem;
		 _i >= lprc_->left;
		 _i -= _cDiv )
	{
		MoveTo( hdc_, _i, lprc_->top );
		LineTo( hdc_, _i, lprc_->bottom);
	}
	_hPen = SelectObject(hdc_, _hPen);
	CoreDeletePen(_hPen);

	//draw the Ver minor lines
	MFPUREAD( lpPrm_, TRENDS_VERMINLNDIV, WORD, _wdv_min );
	if( !_wdv_min ) _wdv_min = 1;

	if( _wdv_min == 0 )	_wdv_min++;
	_tdiv = _Tdiv / _wdv_min;
	_cdiv = (int)( ((float)_tdiv/(float)lTotalTime_) * (float)_cxChart );
	MFPUREAD( lpPrm_, TRENDS_VERMINLNSTYLE, int, _nPenStyle );
	MFPUREAD( lpPrm_, TRENDS_VERMINLNWIDTH, int, _nPenWidth );
	MFPUREAD( lpPrm_, TRENDS_VERMINLNCLR, COLORREF, _clrPen );
	_hPen = CoreCreatePen(_nPenStyle, _nPenWidth, _clrPen);
	_hPen = SelectObject(hdc_, _hPen);
	for( _i = lprc_->right - _cRem + _cDiv;
		 _i > lprc_->left;
		 _i -= _cDiv )
	{
		for( _j=1, _x = _i - _cdiv;
			 _j<_wdv_min;
			 _j++, _x -= _cdiv )
		{
			if( (_x < lprc_->right) && (_x >= lprc_->left) )
			{
				MoveTo( hdc_, _x, lprc_->top );
				LineTo( hdc_, _x, lprc_->bottom);
			}
		}
	}
	_hPen = SelectObject(hdc_, _hPen);
	CoreDeletePen(_hPen);

	return;
}

//...................................................................
//...................................................................
static void _near _TrendDrawHrzLabel(HDC 	hdc_,
									 LPPU	lpPrm_,			//beginning of parameters of the trend
									 DWORD	dwStyle_,		//the style of the trend
									 long	_Tc,			//time now, the ms escaped from 00:00:00
									 DWORD	lTotalTime_,
									 LPRECT lprcTrend_,		//the rect of the whole trend face
									 LPRECT	lprcChart_)		//the rect of the chart of the trend: rcChart_
{
	ldiv_t		_qr;
	int			_cDiv, _cRem;
	WORD		_wdv;
	long		_Tdiv;			//all in ms
	int			_cxChart;
	int			_i;
	long		_tt;		//loop var
	CCLR		_clrText;
	COLORREF	_rgbText;
	char		_szBuffer[] = "HH:MM:SS";
	int			_wid_str;
	BOOL		_retClr;	//return value by CclrGetRGB

	_cxChart = lprcChart_->right - lprcChart_->left;

#ifndef _WIN32
	_wid_str = LOWORD( GetTextExtent(hdc_, _szBuffer, strlen(_szBuffer) ) );
#else
	{
		SIZE	_sz;
		GetTextExtentPoint32(hdc_, _szBuffer, strlen(_szBuffer), &_sz );
		_wid_str = _sz.cx; 
	}
#endif	//_WIN32

	//draw the bottom labels and top labels
	MFPUREAD( lpPrm_, TRENDS_HRZLABELDIV, WORD, _wdv );
	if( _wdv == 0 )	_wdv++;
	_Tdiv = lTotalTime_ / _wdv;
	if( !_Tdiv ) 
		return;

	_qr = ldiv(_Tc, _Tdiv);
	_cRem = (int)( ((float)_qr.rem/(float)lTotalTime_) * (float)_cxChart );
	_cDiv = (int)( ((float)_Tdiv/(float)lTotalTime_) * (float)_cxChart );

	MFPUREAD( lpPrm_, TRENDS_HRZLABELCLR, DWORD, _clrText );
	_retClr = CclrGetRGB( _clrText, &_rgbText );
	SetTextColor( hdc_, _rgbText );

	//to void the following _tt minus error
	_Tc += msInDay * 2;

	for( _i = lprcChart_->right - _cRem, _tt = _Tc - _qr.rem;
		 _i > lprcChart_->left;
		 _i -= _cDiv, _tt -= _Tdiv )
	{
		size_t		_str_len;
		RECT		_rcLabel;

		msToAsciiTime( _tt, _szBuffer );
		_str_len = strlen( _szBuffer );

		SetRect(&_rcLabel,
				_i - _wid_str,
				lprcTrend_->top,
				_i + _wid_str,
				lprcTrend_->bottom);

		//draw the bottom label
		if( dwStyle_ & STYLE_BOTTOMLABEL )
			if( _retClr )	//otherwise, transparent
				DrawText( hdc_, _szBuffer, _str_len, &_rcLabel, DT_CENTER|DT_BOTTOM|DT_SINGLELINE);

		//draw the top label
		if( dwStyle_ & STYLE_TOPLABEL )
			if( _retClr )	//otherwise, transparent
				DrawText( hdc_, _szBuffer, _str_len, &_rcLabel, DT_CENTER|DT_TOP|DT_SINGLELINE);
	}

	return;
}


//...................................................................
//Description:
//
//...................................................................
static void _near msToAsciiTime(long ms_, LPSTR lpszBuffer_ )
{
	int		_sec, _min, _hour;
	ldiv_t	_dv;

	ms_ = ms_ % msInDay;
	if( ms_ < 0 )
		ms_ += msInDay;

	ms_ /= 1000;		//to second
	_dv = ldiv( ms_, 60 );
	_sec = (int)_dv.rem;
	_dv = ldiv( _dv.quot, 60 );
	_min = (int)_dv.rem;
	_hour = (int)_dv.quot;

	sprintf( lpszBuffer_, "%02u:%02u:%02u", _hour, _min, _sec );
	return;
}

//...................................................................
//Description:
//
//...................................................................
static BOOL _near _CalculateChartRect(HDC hdc_, WRD wFTU_, LPRECT lprc_, LPRECT lprcChart_ )
{
	LPPU		_lpPrm;
	BOOL		_bLeftLabel, _bTopLabel, _bRightLabel, _bBottomLabel;
	DWORD       _dwStyle;
	WORD		_extent;	//the max width of the ver. labels
	TEXTMETRIC	_tm;
	int			_i;
	float		_min, _max, _vl, _delta;
	WORD		_wVerLabelDiv;
	char		_szBuffer[32];
	CCLR		_clrText;

	_lpPrm = FaceGetParameter( wFTU_ );
	MFPUREAD( _lpPrm, TRENDS_LEFTLABEL, BOOL, _bLeftLabel );
	MFPUREAD( _lpPrm, TRENDS_TOPLABEL, BOOL, _bTopLabel );
	MFPUREAD( _lpPrm, TRENDS_RIGHTLABEL, BOOL, _bRightLabel );
	MFPUREAD( _lpPrm, TRENDS_BOTTOMLABEL, BOOL, _bBottomLabel );
	_dwStyle = 0;
	if( _bLeftLabel )	_dwStyle |= STYLE_LEFTLABEL;
	if( _bTopLabel )	_dwStyle |= STYLE_TOPLABEL;
	if( _bRightLabel )	_dwStyle |= STYLE_RIGHTLABEL;
	if( _bBottomLabel )	_dwStyle |= STYLE_BOTTOMLABEL;

	_GetCurPenMMClr( wFTU_, &_min, &_max, &_clrText );

	MFPUREAD( _lpPrm, TRENDS_VERLABELDIV, WORD, _wVerLabelDiv );
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
		lprcChart_->top += _tm.tmHeight + _GIP_Y;
	if( _dwStyle & STYLE_BOTTOMLABEL )
		lprcChart_->bottom -= _tm.tmHeight + _GIP_Y;
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
//	This function logs data tracks at every sample time
//Returns:
//	If log data at this time, returns TRUE; otherwise, return FALSE;
//Note:
//...................................................................
static BOOL _near _LogData(WRD wFTU_)
{
	unsigned 	_i;
	LPPU		_lppu;
	DWORD		_sc;
	int			_slide;
	DWORD		_lTotalTime;
	DWORD		_lSideTime;
	
	_lppu = FaceGetParameter( wFTU_ );
	MFPUREAD( _lppu, TRENDS_SC, DWORD, _sc );
	MFPUREAD( _lppu, TRENDS_TOTALTIME, DWORD, _lTotalTime );

	if( _sc == 0 )
	{	//read data to cache and increase _flush_count by one
		DWORD 		_num_pen;
		LPPU		_lppuVar;
		WORD		_wTotalItem, _wValidItem, _wAdding;

		WORD		_wNumParam = FaceGetNumParam( wFTU_ );

		//collect data and flush the trend FTU
		_num_pen = ( _wNumParam - NUM_FIX ) / NUM_VAR;

		MFPUREAD( _lppu, TRENDS_TOTALITEM, WORD, _wTotalItem);
		MFPUREAD( _lppu, TRENDS_VALIDITEM, WORD, _wValidItem);
		MFPUREAD( _lppu, TRENDS_ADDING,    WORD, _wAdding);

		if( _wValidItem < _wTotalItem )
		{
			_wValidItem++;
			MFPUWRITE( _lppu, TRENDS_VALIDITEM, WORD, _wValidItem);
		}

		_lSideTime = GetSlideTimeOffset(_lppu);
		_slide = _wAdding - ((float)( _lTotalTime - _lSideTime ) / _lTotalTime) * (_wTotalItem - 1);
		if( _slide < 0)
		{
			if(_wValidItem >= _wTotalItem)
			{
				CORE_ASSERT( _wValidItem == _wTotalItem );
				_slide += _wTotalItem;
			}
			//otherwise, let it be
		}

		for(_i=0, _lppuVar = _lppu + NUM_FIX;
			_i<_num_pen;
			_i++, _lppuVar += NUM_VAR )
		{
			REAL		_value;
			LPVOID		_lpCache;
			REAL _far*	_lpData;

			MFPUREAD( _lppuVar, TRENDS_DATASOURCE, REAL, _value);
			MFPUREAD( _lppuVar, TRENDS_DATACACHE, FPTR, _lpCache);
			_lpData = (REAL far*)_lpCache;

			if( _lpData )
				_lpData[_wAdding] = _value;

			if( _slide >= 0 )	//it means it's invalid
			{
				MFPUWRITE(_lppuVar, TRENDS_SLIDEVALUE, REAL, _lpData[_slide]);
			}
			else
			{
				MFPUWRITE(_lppuVar, TRENDS_SLIDEVALUE, REAL, 0);
			}
		}

		_wAdding++;
		if( _wAdding >= _wTotalItem )
			_wAdding = 0;
		MFPUWRITE( _lppu, TRENDS_ADDING, WORD, _wAdding);

		//reset the counter initial value
		MFPUREAD( _lppu, TRENDS_SCINIT, DWORD, _sc);
		MFPUWRITE( _lppu, TRENDS_SC, DWORD, _sc);
		return  TRUE;
	}
	else
	{
		_sc--;
		MFPUWRITE( _lppu, TRENDS_SC, DWORD, _sc );
		return  FALSE;
	}
}

static int _near _GetCurPenMMClr( WRD wFTU_, REAL* pMin_, REAL* pMax_, CCLR* pClr_ )
{
	LPPU		_lpPrm, _lpPrmVar;
	int			_currentPen;
	DWORD		_clrPen;
	REAL		_min, _max;

	WORD		_wNumParam;
	WORD		_nNumPen;
	div_t		_divt;

	_lpPrm = FaceGetParameter(wFTU_);
	_wNumParam = FaceGetNumParam(wFTU_);
	_divt = div( _wNumParam - NUM_FIX, NUM_VAR );
	CORE_ASSERT( _divt.rem == 0 );
	_nNumPen = _divt.quot;

	MFPUREAD( _lpPrm, TRENDS_CURRENTPEN, int, _currentPen );
	if( (_currentPen<0) || (_currentPen>=_nNumPen) )
	{
		_currentPen = 0;
		//write the correct value
		MFPUWRITE( _lpPrm, TRENDS_CURRENTPEN, int, _currentPen );
	}

	_lpPrmVar = _lpPrm + NUM_FIX + _currentPen*NUM_VAR;
	MFPUREAD( _lpPrmVar, TRENDS_PENCLR, DWORD, _clrPen );
	MFPUREAD( _lpPrmVar, TRENDS_MIN, REAL, _min );
	MFPUREAD( _lpPrmVar, TRENDS_MAX, REAL, _max );

	*pMin_ = _min;
	*pMax_ = _max;
	*pClr_ = _clrPen;

	return _currentPen;
}

//This is an obsolete function
static WORD _near _TrendRunDirty( WRD wFTU_, WORD dirty_ )
{
	LPPU	_lppu;
	WORD	_wNumParam;
	div_t	_r_div;
	WORD	_i;

	if ( dirty_ & DEEP_DIRTY )
		return dirty_;

	_lppu = FaceGetParameter( wFTU_ );
	_wNumParam = FaceGetNumParam( wFTU_ );
	CORE_ASSERT( _lppu );

	for(_i=0;
		_i<_wNumParam;
		_i++, _lppu++ )
	{
		if( PUIsDirty(_lppu) )
		{
			if(   (_i == TRENDS_LEFTLABEL )
				||(_i == TRENDS_TOPLABEL )
				||(_i == TRENDS_RIGHTLABEL )
				||(_i == TRENDS_BOTTOMLABEL ) )
				return POSITION_DIRTY;

			if(   (_i >= TRENDS_SAMPLEINTERVAL)
				&&(_i <= TRENDS_ADDING) )
			{
				CORE_ASSERT( _i != TRENDS_SAMPLEINTERVAL );
				CORE_ASSERT( _i != TRENDS_TOTALTIME );
				CORE_ASSERT( _i != TRENDS_SCINIT );
				continue;
			}

			if( _i >= NUM_FIX )
			{
				_r_div = div( _i - NUM_FIX, NUM_VAR );
				if( _r_div.rem == TRENDS_DATASOURCE )
					continue;	//ignore data-source dirty
			}
			return NORMAL_DIRTY;
		}
	}

	return NO_DIRTY;
}

static DWORD GetSlideTimeOffset( LPPU lpPrm_ )
{
	DWORD	_dwSlideTimeOffset;
	DWORD	_lTotalTime;

	MFPUREAD( lpPrm_, TRENDS_TOTALTIME, DWORD, _lTotalTime );
	MFPUREAD( lpPrm_, TRENDS_SLIDETIME, DWORD, _dwSlideTimeOffset );
	if( (long)_dwSlideTimeOffset < 0 )
	{
		_dwSlideTimeOffset = 0;
		MFPUWRITE( lpPrm_, TRENDS_SLIDETIME, DWORD, _dwSlideTimeOffset );
	}

	if( _dwSlideTimeOffset > _lTotalTime )
	{
		_dwSlideTimeOffset = _lTotalTime;
		MFPUWRITE( lpPrm_, TRENDS_SLIDETIME, DWORD, _dwSlideTimeOffset );
	}

	return _dwSlideTimeOffset;
}

typedef struct _tag_SLIDEINFO{
	int		nChartWidth;
	DWORD	dwSlideTimeOffset;
}SLIDEINFO, _FAR* LPSLIDEINFO;

static WORD OnRunTimeMsg( WRD wrd_, HWND hwnd_, LPFIA lpFia_ )
{
	LPFIA			_lpFia = lpFia_;
	WORD			_ret = FIA_PROCESSED;
	LPSLIDEINFO		_lpsi = NULL;
	LPPU			_lpPrm;

	DWORD			_lTotalTime;

	_lpPrm = FaceGetParameter( wrd_ );

	switch( _lpFia->msg )
	{
		case WM_LBUTTONDOWN:
		{
			HDC		_hdc;
			RECT	_rc, _rcChart;
			int		_slide_x;

			CORE_DEBUG0( "Trend: WM_LBUTTONDOWN\r\n" );

			//init. _lpFia->dtPrimary and _lpFia->dtLast
			_lpFia->dtPrimary.x = X(_lpFia->lParam);
			_lpFia->dtPrimary.y = Y(_lpFia->lParam);
			_lpFia->dtLast.x = X(_lpFia->lParam);
			_lpFia->dtLast.y =Y(_lpFia->lParam);

			_lpsi = (LPSLIDEINFO)malloc( sizeof(SLIDEINFO) );
			if( !_lpsi )
				return FIA_PROCESSEND;
			else
				_lpFia->lpPrivate = _lpsi;

			_lpsi->dwSlideTimeOffset = GetSlideTimeOffset( _lpPrm );
			FaceXycxcyToRect(wrd_, &_rc);
			_hdc = GetDC( hwnd_ );
			_CalculateChartRect( _hdc, wrd_, &_rc, &_rcChart );
			ReleaseDC( hwnd_, _hdc );
			_lpsi->nChartWidth = _rcChart.right - _rcChart.left;

			MFPUREAD( _lpPrm, TRENDS_TOTALTIME, DWORD, _lTotalTime );
			_slide_x = _rcChart.left + (_rcChart.right - _rcChart.left) * _lpsi->dwSlideTimeOffset / _lTotalTime;
			if( abs( _slide_x - _lpFia->dtLast.x ) > 2*_lpFia->wPrecision )
			{
				//the mouse is not on slide line
				free( _lpFia->lpPrivate );
				_lpFia->lpPrivate = NULL;
				return FIA_PROCESSEND; 
			}
			
			break;
		}

		case WM_MOUSEMOVE:
		{
			int		_iSlideTimeOffset;

			CORE_DEBUG0( "Trend: WM_MOUSEMOVE\r\n" );

			_lpFia->dtLast.x = X(_lpFia->lParam);
			_lpFia->dtLast.y =Y(_lpFia->lParam);

			_lpsi = (LPSLIDEINFO)_lpFia->lpPrivate;
			CORE_ASSERT( _lpsi );

			MFPUREAD( _lpPrm, TRENDS_TOTALTIME, DWORD, _lTotalTime );
			_iSlideTimeOffset = (float)(_lpFia->dtLast.x - _lpFia->dtPrimary.x) * _lTotalTime / _lpsi->nChartWidth + _lpsi->dwSlideTimeOffset;
			if( _iSlideTimeOffset < 0 )
				_iSlideTimeOffset = 0;
			if( (DWORD)_iSlideTimeOffset > _lTotalTime )
				_iSlideTimeOffset = _lTotalTime;

			MFPUWRITE( _lpPrm, TRENDS_SLIDETIME, DWORD, _iSlideTimeOffset );
			UpdateCoreWindow();
			
			break;
		}

		case WM_LBUTTONUP:
		{
			CORE_DEBUG0( "Trend: WM_LBUTTONUP\r\n" );

			free( _lpFia->lpPrivate );
			_lpFia->lpPrivate = NULL;

			_ret = FIA_PROCESSEND;
			break;
		}
	}

	return _ret;
}
