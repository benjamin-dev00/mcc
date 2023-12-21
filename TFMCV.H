#ifndef MCV_COMPLIED
#define MCV_COMPLIED
#include "cpu.h"
#include "tfmat.h"
#include "obj.h"
#include "form.h"
#define		NUMBER_MVPARAM  82
#define		NUMBER_CVPARAM  75
#define		NUMBER_DVPARAM  18
#define		NUMBER_MVCOBJ	5
#define		NUMBER_MVREAL	43
#define		NUMBER_MVWORD	10
#define		NUMBER_MVBOOL	24
#define		NUMBER_CVCOBJ	5
#define		NUMBER_CVREAL	41
#define		NUMBER_CVWORD	13
#define		NUMBER_CVBOOL	16

#define		NUMBER_DVREAL	10
#define		NUMBER_DVBOOL	2
#define		NUMBER_DVCOBJ	5

// word is 0, not 1 here
#define		NUMBER_DVWORD	1

#define		MV_TAG				0
#define		MV_DATANAME			1
#define		MV_NAME				2
#define		MV_VALUE			3
#define		MV_UNIT				4
#define		MV_HSCALE			5
#define		MV_LSCALE			6
#define		MV_DELT				7
#define		MV_SET				8
#define		MV_INI				9
#define		MV_DELMAX			10
#define		MV_DELMIN			11
#define		MV_MAX				12
#define		MV_MIN				13
#define		MV_HIGH				14
#define		MV_LOW				15
#define		MV_FMAX				16
#define		MV_FMIN				17
#define		MV_FHIGH			18
#define		MV_FLOW				19
#define		MV_FXSET			20
#define		MV_FNSET			21
#define		MV_FHSET			22
#define		MV_FLSET		    23
#define		MV_LPINI			24
#define		MV_LPINDEX			25
#define		MV_LPOUT			26
#define		MV_R				27
#define		MV_RBROKEN			28
#define		MV_S				29
#define		MV_SL				30
#define		MV_SBROKEN			31
#define		MV_DELR				32
#define		MV_DELS				33
#define		MV_REALR			34
#define		MV_REALS			35
#define		MV_ALARMMAX			36
#define		MV_ALARMMIN			37
#define		MV_DISPLAYH			38
#define		MV_DISPLAYL			39
#define		MV_DISPLAY			40
#define		MV_SAMPLE			41
#define		MV_STEADYFIELD		42
#define		MV_MHISTORY			43
#define		MV_MLASTIRV			44
#define		MV_MLASTHIGH		45
#define		MV_MLASTLOW			46
#define		MV_RCONTROLMODE		47
#define		MV_PRIORITY			48
#define		MV_HOVERCOUNT		49
#define		MV_LOVERCOUNT		50
#define		MV_CONTROLMODE		51
#define     MV_FPRIORITY		52
#define		MV_PDirtyCount		53
#define		MV_RDirtyCount		54
#define		MV_CTHistory		55
#define		MV_BAD				56
#define		MV_STATE			57
#define		MV_DIST				58
#define		MV_SATUATION		59
#define		MV_ALARM			60
#define		MV_KEYMV			61
#define		MV_MSETSATUATION	62
#define		MV_IRVWRONG			63
#define		MV_MAPARTAM			64
#define		MV_AM				65
#define		MV_HOPEAM			66
#define		MV_HEDGE			67
#define		MV_LEDGE			68
#define		MV_RDIRTY			69
#define		MV_PDIRTY			70
#define		MV_SDIRTY			71
#define		MV_FLAGRBROKEN		72
#define		MV_PBROKEN			73
#define		MV_HOVER			74
#define		MV_LOVER			75
#define		MV_LASTCM			76
#define		MV_RMODE			77
#define		MV_SMODE			78
#define		MV_ERRMODE			79
#define		MV_MCM				80
#define		MV_PRIV				81

#define		CV_TAG				0
#define		CV_DATANAME			1
#define		CV_NAME				2
#define		CV_VALUE			3
#define		CV_UNIT				4
#define		CV_HSCALE			5
#define		CV_LSCALE			6
#define		CV_SET		   		7
#define		CV_INI				8
#define		CV_MAX				9
#define		CV_MIN				10
#define		CV_HIGH				11
#define		CV_LOW				12
#define		CV_FMAX				13
#define		CV_FMIN				14
#define		CV_FHIGH			15
#define		CV_FLOW				16
#define		CV_FXSET			17
#define		CV_FNSET			18
#define		CV_FHSET			19
#define		CV_FLSET			20
#define		CV_LPINI			21
#define		CV_LPINDEX			22
#define		CV_LPOUT			23
#define		CV_Q				24
#define		CV_QL				25
#define		CV_QBROKEN			26
#define		CV_DELQ				27
#define		CV_REALQ			28
#define		CV_ALARMMAX			29
#define		CV_ALARMMIN			30
#define		CV_ERROR			31
#define		CV_DISPLAYH			32
#define		CV_DISPLAYL			33
#define		CV_DISPLAY			34
#define		CV_STEADYDELT		35
#define		CV_STEADYFIELD		36
#define     CV_LASTSET			37
#define     CV_LASTHIGH			38
#define     CV_LASTLOW			39
#define		CV_INIERROR			40
#define		CV_LFAULT			41
#define		CV_HFAULT			42
#define		CV_LSET				43
#define		CV_HSET				44
#define		CV_RCONTROLMODE		45
#define		CV_PRIORITY			46
#define		CV_NOLP				47
#define		CV_HOVERCOUNT		48
#define		CV_LOVERCOUNT		49
#define		CV_CONTROLMODE 		50
#define     CV_FPRIORITY		51
#define     CV_CTHISTORY		52
#define     CV_DIRTY			53
#define     CV_DIRTYCOUNT		54
#define     CV_DELAYCOUNT		55
#define		CV_BAD				56
#define		CV_N				57
#define		CV_STATE			58
#define		CV_ALARM			59
#define		CV_KEYCV			60
#define		CV_HOVER			61
#define		CV_LOVER			62
#define		CV_LPWRONG			63
#define		CV_NOILL			64
#define		CV_HEDGE			65
#define		CV_LEDGE			66
#define		CV_QMODE			67
#define		CV_ERRMODE			68
#define		CV_LIMIT			69
#define		CV_BROKEN			70
#define		CV_SDIRTY			71
#define		CV_LASTCM			72
#define		CV_CM				73
#define		CV_PRIV				74


#define		DV_TAG				0
#define		DV_DATANAME			1
#define		DV_NAME				2
#define		DV_VALUE			3
#define		DV_UNIT				4
#define		DV_HSCALE			5
#define		DV_LSCALE			6
#define		DV_INI				7
#define		DV_DELT				8
#define		DV_HISTORY			9
#define		DV_DISPLAYH			10
#define		DV_DISPLAYL			11
#define		DV_DISPLAY			12
#define		DV_STEADYFIELD		13
#define		DV_STATE			14
#define		DV_ALARM			15
#define		DV_BAD				16
#define		DV_PRIV				17
LPFORMDATA _MVStructFORMGetData( LONG lData_, LPSTR lpvalstr_, DWORD size_, UINT row_, UINT col_ );
LPFORMDATA _MVStructFORMSetData( LONG lData_, LPSTR lpvalstr_, UINT row_, UINT col_ );
LPFORMDATA _MVStructFORMGetRowTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wRow_);
LPFORMDATA _MVStructFORMGetColTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wCol_);
BOOL _DLLFUNC DlgMVStructProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

LPFORMDATA _CVStructFORMGetData( LONG lData_, LPSTR lpvalstr_, DWORD size_, UINT row_, UINT col_ );
LPFORMDATA _CVStructFORMSetData( LONG lData_, LPSTR lpvalstr_, UINT row_, UINT col_ );
LPFORMDATA _CVStructFORMGetRowTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wRow_);
LPFORMDATA _CVStructFORMGetColTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wCol_);
BOOL _DLLFUNC DlgCVStructProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

LPFORMDATA _DVStructFORMGetData( LONG lData_, LPSTR lpvalstr_, DWORD size_, UINT row_, UINT col_ );
LPFORMDATA _DVStructFORMSetData( LONG lData_, LPSTR lpvalstr_, UINT row_, UINT col_ );
LPFORMDATA _DVStructFORMGetRowTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wRow_);
LPFORMDATA _DVStructFORMGetColTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wCol_);
BOOL _DLLFUNC DlgDVStructProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_);

LPLINKMATRIX _DLLFUNC MVStructAlloc( int row, int col );
DWORD ObjectMVStruct(LPPU lpPU_, WORD wMode_, HWND hwnd_);
LPLINKMATRIX _DLLFUNC CVStructAlloc( int row, int col );
DWORD ObjectCVStruct(LPPU lpPU_, WORD wMode_, HWND hwnd_);
LPLINKMATRIX _DLLFUNC DVStructAlloc( int row, int col );
DWORD ObjectDVStruct(LPPU lpPU_, WORD wMode_, HWND hwnd_);
LPFORMDATA _TAGVALUEFORMGetData( LONG lData_, LPSTR lpvalstr_, WORD size_, WORD row_, WORD col_ );
LPFORMDATA _TAGVALUEFORMSetData( LONG lData_, LPCSTR lpvalstr_, WORD row_, WORD col_ );
LPFORMDATA _TAGVALUEFORMGetRowTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wRow_);
LPFORMDATA _TAGVALUEFORMGetColTitle( LONG lData_, LPSTR szBuffer_, WORD wSize_, WORD wCol_);



#define	READMCDSTRUCT(__lpPUBase_, __order_, __MCDparam_, __type_, __value_ ) 	\
	{										 		\
		SimplePU		__pu_;                      \
		LPLINKMATRIX	_lpLinkMatrix;										\
		LPPU			_lptempPU;											\
		_lpLinkMatrix = COBJ_2_LINKMATRIX( __lpPUBase_ );					\
		_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, __MCDparam_, __order_ );	\
		__pu_.prefix = PF_IMM|IMM_##__type_;       	\
		__pu_.du.zzk__FULL = NULL;                  	\
		PURead( &__pu_, (_lptempPU) );       			\
		(__value_) = __pu_.du.zzk__##__type_;			\
	}
#define	WRITEMCDSTRUCT(__lpPUBase_, __order_, __MCDparam_, __type_, __value_ ) 	\
	{																		\
		LPLINKMATRIX	_lpLinkMatrix;										\
		LPPU			_lptempPU;											\
		SimplePU		__pu_;                       						\
		_lpLinkMatrix = COBJ_2_LINKMATRIX( __lpPUBase_ );					\
		_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, __MCDparam_, __order_ );	\
		__pu_.prefix = PF_IMM|IMM_##__type_;       	\
		__pu_.du.zzk__##__type_ = __value_;         	\
		PUWrite( (_lptempPU), &__pu_);       			\
}

HWND _DLLFUNC _MVWindow_Create( HWND hwndParent_,
							 DWORD	dwStyle_,
							 HINSTANCE	hInst_,
							 int x_,
							 int y_,
							 int cx_,
							 int cy_,
							 HMENU id_,
							 LPCOBJ FAR* lplp_ );
HWND _DLLFUNC _CVWindow_Create( HWND hwndParent_,
							 DWORD	dwStyle_,
							 HINSTANCE	hInst_,
							 int x_,
							 int y_,
							 int cx_,
							 int cy_,
							 HMENU id_,
							 LPCOBJ FAR* lplp_ );
HWND _DLLFUNC _DVWindow_Create( HWND hwndParent_,
							 DWORD	dwStyle_,
							 HINSTANCE	hInst_,
							 int x_,
							 int y_,
							 int cx_,
							 int cy_,
							 HMENU id_,
							 LPCOBJ FAR* lplp_ );
//void _DLLFUNC CobjKeepCopy( LPCOBJ lpCobjDest_, LPCOBJ lpCobjSource_ );

LPFORMDATA _MVStructFORMData( LPCELLINFO lpCellInfo_ );
LPFORMDATA _CVStructFORMData( LPCELLINFO lpCellInfo_ );
LPFORMDATA _DVStructFORMData( LPCELLINFO lpCellInfo_ );
ACRET SetGlobalParameter( LPCOBJPRIV __lpSource__, WORD __param__, LPPU __lpPUBase__ );
#endif