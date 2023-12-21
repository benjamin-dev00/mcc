//===================================================================
//
//Zheng zhikai 	, 97-6-19
//Note:
//	Because the value string for DataLinkMatrix has two
//	meanings: value and data-link, but the FORM window
//	can only show ONE kind of value string, how to decide
//	which value string should be return in functions
//		_LinkMatrixFORMGetData
//		_LinkMatrixFORMSetData
//	At this version, I use a TEMPERORY TIP to solve this
//	problem:
//		if ( LINKMATRIX->Nomeaning )
//			show data value of the data-link matrix
//		else
//			show data link of the data-link matrix
//	and the LINKMATRIX->Nomeaning is set and reset by the
//	state of two radio check boxes in dialog box;
//===================================================================
//addtemp
#include "tfalgo.h"
#include "tfmcv.h"
#include "creal.h"
#include "tfres.h"
#include "form.h"

extern HINSTANCE	ghInst;
static int _DlgDVStruct_InitDialog( HWND hwnd_, LPCOBJ FAR* );
static int _DlgCVStruct_InitDialog( HWND hwnd_, LPCOBJ FAR* );
static int _DlgMVStruct_InitDialog( HWND hwnd_, LPCOBJ FAR* );
const char		_MVDiscription[NUMBER_MVPARAM][20] =
				{
					"TAG",
					"BaseName",
					"Name",
					"Value",
					"Unit",
					"HScale",
					"LScale",
					"Delt",
					"Set",
					"Ini",
					"DelMax",
					"DelMin",
					"Max",
					"Min",
					"High",
					"Low",
					"FMax",
					"FMin",
					"FHigh",
					"FLow",
					"FXSet",
					"FNSet",
					"FHSet",
					"FLSet",
					"LPIni",
					"LPIndex",
					"LPOut",
					"R",
					"RBroken",
					"S",
					"SL",
					"Sbroken",
					"Cor_R",
					"Cor_S",
					"RealR",
					"RealS",
					"AlarmMax",
					"AlarmMin",
					"DisplayHigh",
					"DisplayLow",
					"Display",
					"Sample",
					"SteadyField",
					"History",
					"LastIRV",
					"LastHigh",
					"LastLow",
					"RConMode",
					"Prior",
					"HOverCount",
					"LOverCount",
					"ConMode",
					"FaultPriority",
					"PDirtyCount",
					"RDirtyCount",
					"CTHistory",
					"Bad",
					"State",
					"Dist",
                    "Satuation",
					"Alarm",
					"KeyMV",
					"SetSatuation",
					"IRVWrong",
					"ApartAM",
					"A/M",
					"Hope_A/M",
					"HEdge",
					"LEdge",
					"RDirty",
					"PDirty",
					"SDirty",
					"FRBroken",
					"FPBroken",
					"HOver",
					"LOver",
					"LastCM",
					"RMode",
					"SMode",
					"ErrMode",
					"Control_Mode",
					"Priv",
				};
DWORD		_type_MV[NUMBER_MVPARAM] =
			{
					VSOF_LPCOBJ,  //tag
					VSOF_LPCOBJ,  //Name in Database
					VSOF_LPCOBJ,  //True Name
					VSOF_REAL,  //Value
					VSOF_LPCOBJ,  //Unit
					VSOF_REAL,  //Scale(H)
					VSOF_REAL,  //Scale(L)
					VSOF_REAL,  //Deltu
					VSOF_REAL,  //Set
					VSOF_REAL,  //Ini
					VSOF_REAL,  //DelMax 
					VSOF_REAL,  //DelMin
					VSOF_REAL,  //Max
					VSOF_REAL,  //Min
					VSOF_REAL,  //High
					VSOF_REAL,  //Low
					VSOF_REAL,  //FMax
					VSOF_REAL,  //FMin
					VSOF_REAL,  //FHigh
					VSOF_REAL,  //FLow
					VSOF_REAL,  //FXSetpoint
					VSOF_REAL,  //FNSetpoint
					VSOF_REAL,  //FHSetpoint
					VSOF_REAL,  //FLSetpoint
					VSOF_REAL,  //LP Ini
					VSOF_REAL,  //LP Index
					VSOF_REAL,  //SteadOptim value
					VSOF_REAL,	//R Set Value
					VSOF_REAL,	//R Broken Value
					VSOF_REAL,  //S Set Value
					VSOF_REAL,  //SL Set Value
					VSOF_REAL,	//S Broken Value
					VSOF_REAL,	//R Correct
					VSOF_REAL,  //S Correct
					VSOF_REAL,  //Real R Value
					VSOF_REAL,	//Real S Value
					VSOF_REAL,	//Alarm-max
					VSOF_REAL,	//Alarm-min
					VSOF_REAL,	//DisplayHigh
					VSOF_REAL,	//DisplayLow
					VSOF_REAL,	//Display
					VSOF_REAL,	//Sample
					VSOF_REAL,	//SteadyField
					VSOF_REAL,	//History
					VSOF_REAL,	//LastIrv
					VSOF_REAL,	//LastHigh
					VSOF_REAL,	//LastLow
					VSOF_WORD,  //Real Control_mode
					VSOF_WORD,	//Priority
					VSOF_WORD,	//HighOver
					VSOF_WORD,	//LowOver
					VSOF_BOOL,  //Control_mode
					VSOF_WORD,  //FaultPriority
					VSOF_WORD,  //PDirtyCount
					VSOF_WORD,  //RDirtyCount
					VSOF_WORD,  //CTHistory
					VSOF_WORD,	//Bad
					VSOF_BOOL,	//Valid State
					VSOF_BOOL,	//Dist State
					VSOF_BOOL,	//Satuation State
					VSOF_BOOL,	//Alarm State
                    VSOF_BOOL,	//Key MV
					VSOF_BOOL,	//SetSatuation
					VSOF_BOOL,	//IrvWrong
					VSOF_BOOL,	//ApartAM
					VSOF_BOOL,	//A/M State
					VSOF_BOOL,	//A/M State expected
					VSOF_BOOL,	//Edge
					VSOF_BOOL,	//Edge
					VSOF_BOOL,	//RDirty
					VSOF_BOOL,	//PDirty
					VSOF_BOOL,	//SDirty
					VSOF_BOOL,	//RBroken
					VSOF_BOOL,	//PBroken
					VSOF_BOOL,	//HOver
					VSOF_BOOL,	//LOver
					VSOF_BOOL,	//LastCM
					VSOF_BOOL,  //R CorrectMode
					VSOF_BOOL,  //S CorrectMode
					VSOF_BOOL,  //Error Control Mode
					VSOF_BOOL,	//ControlMode
					VSOF_LPCOBJ,//PrivMV
				};

const char		_CVDiscription[NUMBER_CVPARAM][20] =
				{
					"TAG",
					"BaseName",
					"Name",
					"Value",
					"Unit",
					"HScale",
					"LScale",
					"Set",
					"Ini",
					"Max",
					"Min",
					"High",
					"Low",
					"FMax",
					"FMin",
					"FHigh",
					"FLow",
					"FXSet",
					"FNSet",
					"FHSet",
					"FLSet",
					"LPIni",
					"LPIndex",
					"LPOut",
					"Q",
					"QL",
					"Qbroken",
					"Cor_Q",
					"RealQ",
					"AlarmMax",
					"AlarmMin",
					"Error",
					"DisplayHigh",
					"DisplayLow",
					"Display",
					"SteadyDelt",
					"SteadyField",
					"LastSet",
					"LastHigh",
					"LastLow",
					"IniError",
					"LFault",
					"HFault",
					"LSet",
					"HSet",
					"RConMode",
					"Prior",
					"NoLP",
					"HOvercount",
					"LOverCOunt",
					"ConMode",
					"FaultPriority",
					"CTHistory",
					"Dirty",
					"DirtyCount",
					"DelayCount",
					"Bad",
					"N",
					"State",
					"Alarm",
					"KeyCV",
					"HOver",
					"LOver",
					"LPWrong",
					"NoIll",
					"HEdge",
					"LEdge",
					"QMode",
					"ErrMode",
					"Limit",
					"Broken",
					"SDirty",
					"LastCM",
					"ControlMode",
					"Priv",
				};
DWORD		_type_CV[NUMBER_CVPARAM] =
				{
					VSOF_LPCOBJ,  //tag
					VSOF_LPCOBJ,  //Name in Database
					VSOF_LPCOBJ,  //True Name
					VSOF_REAL,  //Value
					VSOF_LPCOBJ,  //Unit
					VSOF_REAL,  //Scale(H)
					VSOF_REAL,  //Scale(L)
					VSOF_REAL,  //Set
					VSOF_REAL,  //Ini
					VSOF_REAL,  //Max
					VSOF_REAL,  //Min
					VSOF_REAL,  //High
					VSOF_REAL,  //Low
					VSOF_REAL,  //FMax
					VSOF_REAL,  //FMin
					VSOF_REAL,  //FHigh
					VSOF_REAL,  //FLow
					VSOF_REAL,  //FXSet
					VSOF_REAL,  //FNSEt
					VSOF_REAL,  //FHSet
					VSOF_REAL,  //FLSet
					VSOF_REAL,  //LP Ini
					VSOF_REAL,  //LP Index
					VSOF_REAL,  //SteadOptim value
					VSOF_REAL,	//Q Set Value
					VSOF_REAL,	//QL Set Value
					VSOF_REAL,	//Q Broken Value
					VSOF_REAL,	//Q Correct
					VSOF_REAL,  //Real Q Value
					VSOF_REAL,  //Alarm_max
					VSOF_REAL,  //Alarm_min
					VSOF_REAL,  //Error
					VSOF_REAL,  //DisplayHigh
					VSOF_REAL,  //DisplayLow
					VSOF_REAL,  //Display
					VSOF_REAL,  //SteadyDelt
					VSOF_REAL,  //SteadyField
					VSOF_REAL,  //LastSet
					VSOF_REAL,  //LastHigh
					VSOF_REAL,  //LastLow
					VSOF_REAL,  //IniError
					VSOF_REAL,  //FLow
					VSOF_REAL,  //FHigh
					VSOF_REAL,  //FLowSet
					VSOF_REAL,  //FHighSet
					VSOF_WORD,	//Real Setpoint/field Control
					VSOF_WORD,	//Priority
					VSOF_BOOL,  //NoLP
					VSOF_WORD,  //HOverCount
					VSOF_WORD,  //LOverCount
					VSOF_BOOL,	//Setpoint/field Control
					VSOF_WORD,	//FaultPriority
					VSOF_WORD,	//CTHistory
					VSOF_WORD,	//Dirty
					VSOF_WORD,	//DirtyCount
					VSOF_WORD,	//DealyCount
					VSOF_WORD,	//Bad
					VSOF_WORD,	//N
					VSOF_BOOL,	//InValid State
					VSOF_BOOL,	//Alarm State
					VSOF_BOOL,	//Key CV
					VSOF_BOOL,  //HOver
					VSOF_BOOL,  //Lover
					VSOF_BOOL,  //LPWrong
					VSOF_BOOL,  //NoIll
					VSOF_BOOL,  //HEdge
					VSOF_BOOL,  //LEdge
					VSOF_BOOL,  //Q CorrectMode
					VSOF_BOOL,  //Error Control Mode
					VSOF_BOOL,	//Limit State
					VSOF_BOOL,	//BRoken
					VSOF_BOOL,	//SDirty
					VSOF_BOOL,	//LastCM
					VSOF_BOOL,  //ControlMode
					VSOF_LPCOBJ,	//PrivCV
			};

const char		_DVDiscription[NUMBER_DVPARAM][20] =
				{
					"TAG",
					"BaseName",
					"Name",
					"Value",
					"Unit",
					"HScale",
					"LScale",
					"Ini",
					"Delt",
					"Hist",
					"DisplayHigh",
					"DisplayLow",
					"Display",
					"SteadyField",
					"State",
					"Alarm",
					"Bad",
					"Priv",
				};
DWORD		_type_DV[NUMBER_DVPARAM] =
				{
					VSOF_LPCOBJ,  //tag
					VSOF_LPCOBJ,  //Name in Database
					VSOF_LPCOBJ,  //True Name
					VSOF_REAL,  //Value
					VSOF_LPCOBJ,  //Unit
					VSOF_REAL,  //Scale(H)
					VSOF_REAL,  //Scale(H)
					VSOF_REAL,  //Ini
					VSOF_REAL,  //Delt
					VSOF_REAL,  //History
					VSOF_REAL,  //DisplayHigh
					VSOF_REAL,  //DisplayLow
					VSOF_REAL,  //Display
					VSOF_REAL,  //SteadyField
					VSOF_BOOL,	//Valid State
					VSOF_BOOL,	//Alarm
					VSOF_WORD,	//BAD
					VSOF_LPCOBJ,//PrivDV
			};		

//...................................................................
//...................................................................
LPLINKMATRIX _DLLFUNC MVStructAlloc( int row, int col )
{
	LPLINKMATRIX	_lpLinkMatrix;
	LPCOBJ			_lpCobj;
	int				_i, _j;

	_lpCobj = CobjAlloc( IDCOBJ_MVSTRUCT, (long)row * (long)col * (long)sizeof(PU) * 1L + sizeof(LINKMATRIX) + sizeof(COBJ) );

	_lpLinkMatrix = COBJ_2_LINKMATRIX( _lpCobj );
	_lpLinkMatrix->row = row;
	_lpLinkMatrix->col = col;
	for(_i = 0; _i < _lpLinkMatrix->row; _i++ )
		for(_j = 0; _j < _lpLinkMatrix->col; _j++ )
		{
			if( _i < NUMBER_MVPARAM )
			{
				if( _type_MV[_i] == VSOF_REAL )
				{
					DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_REAL;
					DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__REAL = 0;
				}
				else
					if( _type_MV[_i] == VSOF_LPCOBJ )
					{
						DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_LPCOBJ;
						DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__LPCOBJ = 0;
					}
					else
						if( _type_MV[_i] == VSOF_WORD )
						{
							DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_WORD;
							DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__WORD = 0;
						}
						else
							if( _type_MV[_i] == VSOF_BOOL )
							{
								DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_BOOL;
								DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__BOOL = 0;
							}
			}
			else
			{
				DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_REAL;
				DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__REAL = 0;
			}
		}

	return _lpLinkMatrix;
}
//...................................................................
//...................................................................
LPLINKMATRIX _DLLFUNC CVStructAlloc( int row, int col )
{
	LPLINKMATRIX	_lpLinkMatrix;
	LPCOBJ			_lpCobj;
	int				_i, _j;


	_lpCobj = CobjAlloc( IDCOBJ_CVSTRUCT, (long)row * (long)col * (long)sizeof(PU) * 1L + sizeof(LINKMATRIX) + sizeof(COBJ) );

	_lpLinkMatrix = COBJ_2_LINKMATRIX( _lpCobj );
	_lpLinkMatrix->row = row;
	_lpLinkMatrix->col = col;
	for(_i = 0; _i < _lpLinkMatrix->row; _i++ )
		for(_j = 0; _j < _lpLinkMatrix->col; _j++ )
		{
			if( _i < NUMBER_CVPARAM )
			{
				if( _type_CV[_i] == VSOF_REAL )
				{
					DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_REAL;
					DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__REAL = 0;
				}
				else
					if( _type_CV[_i] == VSOF_LPCOBJ )
					{
						DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_LPCOBJ;
						DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__LPCOBJ = 0;
					}
					else
						if( _type_CV[_i] == VSOF_WORD )
						{
							DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_WORD;
							DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__WORD = 0;
						}
						else
							if( _type_CV[_i] == VSOF_BOOL )
							{
								DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_BOOL;
								DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__BOOL = 0;
							}
			}
			else
			{
				DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_REAL;
				DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__REAL = 0;
			}
		}

	return _lpLinkMatrix;
}
//...................................................................
//...................................................................
LPLINKMATRIX _DLLFUNC DVStructAlloc( int row, int col )
{
	LPLINKMATRIX	_lpLinkMatrix;
	LPCOBJ			_lpCobj;
	int				_i, _j;

	_lpCobj = CobjAlloc( IDCOBJ_DVSTRUCT, (long)row * (long)col * (long)sizeof(PU) * 1L + sizeof(LINKMATRIX) + sizeof(COBJ) );

	_lpLinkMatrix = COBJ_2_LINKMATRIX( _lpCobj );
	_lpLinkMatrix->row = row;
	_lpLinkMatrix->col = col;
	for(_i = 0; _i < _lpLinkMatrix->row; _i++ )
		for(_j = 0; _j < _lpLinkMatrix->col; _j++ )
		{
			if( _j < NUMBER_DVPARAM )
			{
				if( _type_DV[_i] == VSOF_REAL )
				{	
					DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_REAL;
					DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__REAL = 0;
				}
				else
					if( _type_DV[_i] == VSOF_LPCOBJ )
					{
						DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_LPCOBJ;
						DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__LPCOBJ = 0;
					}
					else
						if( _type_DV[_i] == VSOF_WORD )
						{
							DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_WORD;
							DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__WORD = 0;
						}
						else
							if( _type_DV[_i] == VSOF_BOOL )
							{
								DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_BOOL;
								DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__BOOL = 0;
							}
			}
			else
			{
				DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->prefix = PF_IMM|IMM_REAL;
				DLM_GET_LPPU( _lpLinkMatrix, _i, _j )->du.zzk__REAL = 0;
			}
		}

	return _lpLinkMatrix;
}

//...................................................................
//...................................................................
//...................................................................
//...................................................................
#pragma argsused
BOOL _DLLFUNC DlgMVStructProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_)
{
	switch(msg_)
	{
		case WM_INITDIALOG:
			return _DlgMVStruct_InitDialog( hwnd_, (LPCOBJ FAR*)lParam_ );

		case WM_COMMAND:
			switch(wParam_)
			{
				case IDC_DATALINK:
				case IDC_VALUE:
				{
					BOOL			_bChecked;
					HWND			_hwndFORM;
					LPLINKMATRIX	_lpLinkMatrix;
					LPFORM			_lpFORM;

					if( HIWORD(lParam_ ) != BN_CLICKED )
						break;

					if( IsDlgButtonChecked( hwnd_, IDC_VALUE ) )
						_bChecked = TRUE;
					else
						_bChecked = FALSE;

					_hwndFORM = GetDlgItem( hwnd_, IDC_MATRIX_SHEET );
					_lpFORM = (LPFORM)GetWindowLong( _hwndFORM, 0 );
					_lpLinkMatrix = (LPLINKMATRIX)_lpFORM->lData;
					if( !_lpLinkMatrix )
						break;

					if( (BOOL)(_lpLinkMatrix->Nomeaning[0]) != _bChecked )
					{
						_lpLinkMatrix->Nomeaning[0] = _bChecked;
						InvalidateRect( _hwndFORM, NULL, TRUE );	//redraw the FORM window
					}
					break;
				}

				case IDOK:
					if( GetFocus() == GetDlgItem( hwnd_, IDC_MATRIX_SHEET ) )
					{
						SendDlgItemMessage( hwnd_, IDC_MATRIX_SHEET, WM_KEYDOWN, VK_RETURN, 0 );
						SendDlgItemMessage( hwnd_, IDC_MATRIX_SHEET, WM_KEYUP, VK_RETURN, 0 );
					}
					else
						EndDialog(hwnd_, IDOK);
					return TRUE;

				case IDCANCEL:
					EndDialog(hwnd_, IDCANCEL);
					return TRUE;

				default:
					return FALSE;
			}

		default:
			return FALSE;
	}
}
//...................................................................
//-----------------------------------------------------------------------//...................................................................
#pragma argsused
BOOL _DLLFUNC DlgCVStructProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_)
{
	switch(msg_)
	{
		case WM_INITDIALOG:
			return _DlgCVStruct_InitDialog( hwnd_, (LPCOBJ FAR*)lParam_ );

		case WM_COMMAND:
			switch(wParam_)
			{
				case IDC_DATALINK:
				case IDC_VALUE:
				{
					BOOL			_bChecked;
					HWND			_hwndFORM;
					LPLINKMATRIX	_lpLinkMatrix;
					LPFORM			_lpFORM;

					if( HIWORD(lParam_ ) != BN_CLICKED )
						break;

					if( IsDlgButtonChecked( hwnd_, IDC_VALUE ) )
						_bChecked = TRUE;
					else
						_bChecked = FALSE;

					_hwndFORM = GetDlgItem( hwnd_, IDC_MATRIX_SHEET );
					_lpFORM = (LPFORM)GetWindowLong( _hwndFORM, 0 );
					_lpLinkMatrix = (LPLINKMATRIX)_lpFORM->lData;
					if( !_lpLinkMatrix )
						break;

					if( (BOOL)(_lpLinkMatrix->Nomeaning[0]) != _bChecked )
					{
						_lpLinkMatrix->Nomeaning[0] = _bChecked;
						InvalidateRect( _hwndFORM, NULL, TRUE );	//redraw the FORM window
					}
					break;
				}

				case IDOK:
					if( GetFocus() == GetDlgItem( hwnd_, IDC_MATRIX_SHEET ) )
					{
						SendDlgItemMessage( hwnd_, IDC_MATRIX_SHEET, WM_KEYDOWN, VK_RETURN, 0 );
						SendDlgItemMessage( hwnd_, IDC_MATRIX_SHEET, WM_KEYUP, VK_RETURN, 0 );
					}
					else
						EndDialog(hwnd_, IDOK);
					return TRUE;

				case IDCANCEL:
					EndDialog(hwnd_, IDCANCEL);
					return TRUE;

				default:
					return FALSE;
			}

		default:
			return FALSE;
	}
}
//...................................................................
//...................................................................
//...................................................................
//...................................................................
#pragma argsused
BOOL _DLLFUNC DlgDVStructProc(HWND hwnd_, UINT msg_, WPARAM wParam_, LPARAM lParam_)
{
	switch(msg_)
	{
		case WM_INITDIALOG:
			return _DlgDVStruct_InitDialog( hwnd_, (LPCOBJ FAR*)lParam_ );

		case WM_COMMAND:
			switch(wParam_)
			{
				case IDC_DATALINK:
				case IDC_VALUE:
				{
					BOOL			_bChecked;
					HWND			_hwndFORM;
					LPLINKMATRIX	_lpLinkMatrix;
					LPFORM			_lpFORM;

					if( HIWORD(lParam_ ) != BN_CLICKED )
						break;

					if( IsDlgButtonChecked( hwnd_, IDC_VALUE ) )
						_bChecked = TRUE;
					else
						_bChecked = FALSE;

					_hwndFORM = GetDlgItem( hwnd_, IDC_MATRIX_SHEET );
					_lpFORM = (LPFORM)GetWindowLong( _hwndFORM, 0 );
					_lpLinkMatrix = (LPLINKMATRIX)(_lpFORM->lData);
					if( !_lpLinkMatrix )
						break;

					if( (BOOL)(_lpLinkMatrix->Nomeaning[0]) != _bChecked )
					{
						_lpLinkMatrix->Nomeaning[0] = _bChecked;
						InvalidateRect( _hwndFORM, NULL, TRUE );	//redraw the FORM window
					}
					break;
				}

				case IDOK:
					if( GetFocus() == GetDlgItem( hwnd_, IDC_MATRIX_DEFAULT ) )
					{
						SendDlgItemMessage( hwnd_, IDC_MATRIX_DEFAULT, WM_KEYDOWN, VK_RETURN, 0 );
						SendDlgItemMessage( hwnd_, IDC_MATRIX_DEFAULT, WM_KEYUP, VK_RETURN, 0 );
					}
					else
						EndDialog(hwnd_, IDOK);
					return TRUE;

				case IDCANCEL:
					EndDialog(hwnd_, IDCANCEL);
					return TRUE;

				default:
					return FALSE;
			}

		default:
			return FALSE;
	}
}

static int _DlgMVStruct_InitDialog( HWND hwnd_, LPCOBJ FAR* lplp_ )
{
	char		_szBuffer[128];
	//SimplePU	_spu;
	//LPCOBJ		_lpCobj;
	LPMATRIX	_lpLinkMatrix;
	RECT		_rcDefault;
//	HWND		_hwndFORM;
	POINT		_ptUpperLeft, _ptLowerRight;
	DWORD  		_dwStyle;

	_lpLinkMatrix = COBJ_2_MATRIX( (*lplp_) );
	_dwStyle = WS_BORDER|WS_CHILD|WS_VISIBLE|WS_TABSTOP|SHS_VERLINE|SHS_HORLINE;
	if( !_lpLinkMatrix )
		goto _BAD;

	SendDlgItemMessage(hwnd_, IDC_DATALINK, BM_SETCHECK, 1, 0);
	_lpLinkMatrix->Nomeaning[0] = 0;
	
	GetWindowRect( GetDlgItem(hwnd_, IDC_MATRIX_DEFAULT), &_rcDefault );
	_ptUpperLeft.x = _rcDefault.left;
	_ptUpperLeft.y = _rcDefault.top;
	_ptLowerRight.x = _rcDefault.right;
	_ptLowerRight.y = _rcDefault.bottom;
	ScreenToClient( hwnd_, &_ptUpperLeft );
	ScreenToClient( hwnd_, &_ptLowerRight );
	_MVWindow_Create( hwnd_, _dwStyle,
						  (HINSTANCE)GetWindowLong(hwnd_, GWL_HINSTANCE),
//						(HINSTANCE)GetCurrentTask(),
						  _ptUpperLeft.x,
						  _ptUpperLeft.y,
						  _ptLowerRight.x - _ptUpperLeft.x,
						  _ptLowerRight.y  - _ptUpperLeft.y,
						  (HMENU)IDC_MATRIX_SHEET,
						  (LPCOBJ FAR* ) lplp_ );

//	SetFocus( _hwndFORM ); //set focus by myself

	sprintf( _szBuffer, "MV-Row:%d", _lpLinkMatrix->row );
	SendDlgItemMessage( hwnd_, IDC_MATRIX_ROW, WM_SETTEXT, 0, (LPARAM)_szBuffer );
	sprintf( _szBuffer, "MV-Col:%d", _lpLinkMatrix->col );
	SendDlgItemMessage( hwnd_, IDC_MATRIX_COL, WM_SETTEXT, 0, (LPARAM)_szBuffer );
	return FALSE;

_BAD:
	EndDialog( hwnd_, IDCANCEL );
	return TRUE;
}
static int _DlgCVStruct_InitDialog( HWND hwnd_, LPCOBJ FAR* lplp_ )
{
	char		_szBuffer[128];
	//SimplePU	_spu;
	//LPCOBJ		_lpCobj;
	LPMATRIX	_lpLinkMatrix;
	RECT		_rcDefault;
//	HWND		_hwndFORM;
	POINT		_ptUpperLeft, _ptLowerRight;
	DWORD  		_dwStyle;

	_lpLinkMatrix = COBJ_2_MATRIX( (*lplp_) );
	_dwStyle = WS_BORDER|WS_CHILD|WS_VISIBLE|WS_TABSTOP|SHS_VERLINE|SHS_HORLINE;
	if( !_lpLinkMatrix )
		goto _BAD;

	SendDlgItemMessage(hwnd_, IDC_DATALINK, BM_SETCHECK, 1, 0);
	_lpLinkMatrix->Nomeaning[0] = 0;
	
	GetWindowRect( GetDlgItem(hwnd_, IDC_MATRIX_DEFAULT), &_rcDefault );
	_ptUpperLeft.x = _rcDefault.left;
	_ptUpperLeft.y = _rcDefault.top;
	_ptLowerRight.x = _rcDefault.right;
	_ptLowerRight.y = _rcDefault.bottom;
	ScreenToClient( hwnd_, &_ptUpperLeft );
	ScreenToClient( hwnd_, &_ptLowerRight );
	_CVWindow_Create( hwnd_, _dwStyle,
						  (HINSTANCE)GetWindowLong(hwnd_, GWL_HINSTANCE),
						   //	ghInst,
						  _ptUpperLeft.x,
						  _ptUpperLeft.y,
						  _ptLowerRight.x - _ptUpperLeft.x,
						  _ptLowerRight.y  - _ptUpperLeft.y,
						  (HMENU)IDC_MATRIX_SHEET,
						  (LPCOBJ FAR* ) lplp_ );

//	SetFocus( _hwndFORM ); //set focus by myself

	sprintf( _szBuffer, "CV-Row:%d", _lpLinkMatrix->row );
	SendDlgItemMessage( hwnd_, IDC_MATRIX_ROW, WM_SETTEXT, 0, (LPARAM)_szBuffer );
	sprintf( _szBuffer, "CV-Col:%d", _lpLinkMatrix->col );
	SendDlgItemMessage( hwnd_, IDC_MATRIX_COL, WM_SETTEXT, 0, (LPARAM)_szBuffer );
	return FALSE;

_BAD:
	EndDialog( hwnd_, IDCANCEL );
	return TRUE;
}
static int _DlgDVStruct_InitDialog( HWND hwnd_, LPCOBJ FAR* lplp_ )
{
	char		_szBuffer[128];
	//SimplePU	_spu;
	//LPCOBJ		_lpCobj;
	LPMATRIX	_lpLinkMatrix;
	RECT		_rcDefault;
	POINT		_ptUpperLeft, _ptLowerRight;
	DWORD  		_dwStyle;

	_lpLinkMatrix = COBJ_2_MATRIX( (*lplp_) );
	_dwStyle = WS_BORDER|WS_CHILD|WS_VISIBLE|WS_TABSTOP|SHS_VERLINE|SHS_HORLINE;
	if( !_lpLinkMatrix )
		goto _BAD;

	SendDlgItemMessage(hwnd_, IDC_DATALINK, BM_SETCHECK, 1, 0);
	_lpLinkMatrix->Nomeaning[0] = 0;
	
	GetWindowRect( GetDlgItem(hwnd_, IDC_MATRIX_DEFAULT), &_rcDefault );
	_ptUpperLeft.x = _rcDefault.left;
	_ptUpperLeft.y = _rcDefault.top;
	_ptLowerRight.x = _rcDefault.right;
	_ptLowerRight.y = _rcDefault.bottom;
	ScreenToClient( hwnd_, &_ptUpperLeft );
	ScreenToClient( hwnd_, &_ptLowerRight );
	_DVWindow_Create( hwnd_, _dwStyle,
						  (HINSTANCE)GetWindowLong(hwnd_, GWL_HINSTANCE),
						  _ptUpperLeft.x,
						  _ptUpperLeft.y,
						  _ptLowerRight.x - _ptUpperLeft.x,
						  _ptLowerRight.y  - _ptUpperLeft.y,
						  (HMENU)IDC_MATRIX_SHEET,
						  (LPCOBJ FAR*) lplp_ );

//	SetFocus( _hwndFORM ); //set focus by myself

	sprintf( _szBuffer, "DV-Row:%d", _lpLinkMatrix->row );
	SendDlgItemMessage( hwnd_, IDC_MATRIX_ROW, WM_SETTEXT, 0, (LPARAM)_szBuffer );
	sprintf( _szBuffer, "DV-Col:%d", _lpLinkMatrix->col );
	SendDlgItemMessage( hwnd_, IDC_MATRIX_COL, WM_SETTEXT, 0, (LPARAM)_szBuffer );
	return FALSE;

_BAD:
	EndDialog( hwnd_, IDCANCEL );
	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _MVStructFORMGetData( LONG lData_, LPSTR lpvalstr_, DWORD size_, UINT row_, UINT col_ )
{
	LPLINKMATRIX	_lpLinkMatrix = (LPLINKMATRIX)lData_;
	LPPU			_lptempPU;
	GETVALUESTRING	_gvs;
	DWORD			_ret_gvs;

	_gvs.lpszBuffer = lpvalstr_;
	_gvs.wSize = (WORD)size_;

	if(  ( row_ >= (WORD)_lpLinkMatrix->row )
		||( col_ >= (WORD)_lpLinkMatrix->col ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	//_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );
	_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );
	if( _lpLinkMatrix->Nomeaning[0] )		//show value
	{
		if( row_ <= MV_UNIT )
		{
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, MV_VALUE, col_ );
			if( row_ == MV_VALUE )
				_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_MV[row_] );
			else
			{
				switch( row_ )
				{
					case MV_TAG:
						{
							_lptempPU->prefix += ( RTU_TAG - RTU_VALUE );
							_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_MV[row_] );
							_lptempPU->prefix -= ( RTU_TAG - RTU_VALUE );
							break;
						}
					case MV_NAME:
						{
							_lptempPU->prefix += ( RTU_COMMENT - RTU_VALUE );
							_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_MV[row_] );
							_lptempPU->prefix -= ( RTU_COMMENT - RTU_VALUE );
							break;
						}
					case MV_UNIT:
						{
							_lptempPU->prefix += ( RTU_UNIT - RTU_VALUE );
							_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_MV[row_] );
							_lptempPU->prefix -= ( RTU_UNIT - RTU_VALUE );
							break;
						}
					case MV_DATANAME:
						{
							strcpy( lpvalstr_, "" );	
							_ret_gvs = 4;
							break;
						}								
					_ret_gvs = 2;
				}
			}
		}
		else
		{
			if( row_ < NUMBER_MVPARAM )
			{
				if( row_ == MV_PRIV )
				{
					//_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_MV[row_] );
					CORE_ASSERT(_lptempPU->prefix == (PF_IMM|IMM_LPCOBJ) );
					sprintf( _gvs.lpszBuffer, "%Fp", _lptempPU->du.zzk__LPCOBJ );
					_ret_gvs = 2;
				}
				else
				{
					_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_MV[row_] );
				}
			}
			else
				_ret_gvs = PUGetValueString( _lptempPU, &_gvs, VSOF_REAL );
		}
		if( 0 == _ret_gvs )
			strcpy(lpvalstr_, "#ERR");	//invalid value
	}
	else	//show data link
	{
		DECODE	_decode;

		_decode.wAccess = 0;

		if( PUDecode( _lptempPU, &_decode ) )
			CombineDecode(lpvalstr_, &_decode);
		else
			strcpy( lpvalstr_, "#ERR" );	//invalid data-link
	}

	return TRUE;
}



//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _MVStructFORMSetData( LONG lData_, LPSTR lpvalstr_, UINT row_, UINT col_ )
{
	LPLINKMATRIX	_lpLinkMatrix = (LPLINKMATRIX)lData_;
	LPPU			_lptempPU;

	if(  ( row_ >= (WORD)_lpLinkMatrix->row )
		||( col_ >= (WORD)_lpLinkMatrix->col ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );
	if( _lpLinkMatrix->Nomeaning[0] )		//set value
	{
		if( row_ <= MV_UNIT )
		{
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, MV_VALUE, col_ );
			if( row_ == MV_VALUE )
				PUSetValueString( _lptempPU, lpvalstr_, _type_MV[row_] );
/*			else
				if( row_ == MV_HSCALE )
				{
					_lptempPU->prefix += ( RTU_MAX - RTU_VALUE);
					PUSetValueString( _lptempPU, lpvalstr_, _type_MV[row_] );
					_lptempPU->prefix -= ( RTU_MAX - RTU_VALUE);
				}
				else
					if( row_ == MV_LSCALE )
					{
						_lptempPU->prefix += ( RTU_MIN - RTU_VALUE);
						PUSetValueString( _lptempPU, lpvalstr_, _type_MV[row_] );
						_lptempPU->prefix -= ( RTU_MIN - RTU_VALUE);
					}*/
				return TRUE;
		}
		else
			if( row_ < NUMBER_MVPARAM )
			{
				if( row_ != MV_PRIV )
					return (BOOL)PUSetValueString( _lptempPU, lpvalstr_, _type_MV[row_] );
				else
                	return FALSE;
			}
			else
				return (BOOL)PUSetValueString( _lptempPU, lpvalstr_, VSOF_REAL );
	}
	else	//set data link
	{
		ENCODE	_encode;
		_encode.lpszBuffer = lpvalstr_;
		_encode.wAccess = 0;
		_encode.wError = 0;
		if( row_ < NUMBER_MVPARAM )
		{
			if( row_ != MV_VALUE && row_ <= MV_UNIT )
			{
				if( _type_MV[row_] == VSOF_REAL )
					strcpy(lpvalstr_, "real");
				if( _type_MV[row_] == VSOF_LPCOBJ )
					strcpy(lpvalstr_, "cobj");
				if( _type_MV[row_] == VSOF_BOOL )
					strcpy(lpvalstr_, "bool");
				if( _type_MV[row_] == VSOF_WORD )
					strcpy(lpvalstr_, "word");
			}
		}

		return PUEncode( _lptempPU, &_encode );
	}                                          
}

//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _MVStructFORMGetColTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wCol_ )
{
	sprintf( szBuffer_, "%i", wCol_ + 1 );
	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _MVStructFORMGetRowTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wRow_)
{
	sprintf( szBuffer_, "%s", _MVDiscription[wRow_] );
	return TRUE;
}
//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _CVStructFORMGetData( LONG lData_, LPSTR lpvalstr_, DWORD size_, UINT row_, UINT col_ )
{
	LPLINKMATRIX	_lpLinkMatrix = (LPLINKMATRIX)lData_;
	LPPU				_lptempPU;
	GETVALUESTRING	_gvs;
	DWORD			_ret_gvs;

	_gvs.lpszBuffer = lpvalstr_;
	_gvs.wSize = (WORD)size_;

	if(  ( row_ >= (WORD)_lpLinkMatrix->row )
		||( col_ >= (WORD)_lpLinkMatrix->col ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );
	if( _lpLinkMatrix->Nomeaning[0] )		//show value
	{
		if( row_ <= CV_UNIT )
		{
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, CV_VALUE, col_ );
			if( row_ == CV_VALUE )
				_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_CV[row_] );
			else
/*				if( row_ == CV_HSCALE )
				{
					_lptempPU->prefix += ( RTU_MAX - RTU_VALUE );
					_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_CV[row_] );
					_lptempPU->prefix -= ( RTU_MAX - RTU_VALUE );
				}
				else
				{
					if( row_ == CV_LSCALE )
					{
						_lptempPU->prefix += ( RTU_MIN - RTU_VALUE );
						_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_CV[row_] );
						_lptempPU->prefix -= ( RTU_MIN - RTU_VALUE );
					}
					else*/
			{
				switch( row_ )
				{
					case CV_TAG:
						{
							_lptempPU->prefix += ( RTU_TAG - RTU_VALUE );
							_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_CV[row_] );
							_lptempPU->prefix -= ( RTU_TAG - RTU_VALUE );
							break;
						}
					case CV_NAME:
						{
							_lptempPU->prefix += ( RTU_COMMENT - RTU_VALUE );
							_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_CV[row_] );
							_lptempPU->prefix -= ( RTU_COMMENT - RTU_VALUE );
							break;
						}
					case CV_UNIT:
						{
							_lptempPU->prefix += ( RTU_UNIT - RTU_VALUE );
							_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_CV[row_] );
							_lptempPU->prefix -= ( RTU_UNIT - RTU_VALUE );
							break;
						}
					case CV_DATANAME:
						{
							strcpy( lpvalstr_, "" );	
							_ret_gvs = 4;
							break;
						}								
					_ret_gvs = 2;
				}
			}
		}
		else
		{
			if( row_ < NUMBER_CVPARAM )
			{
				if( row_ < CV_PRIV )
					_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_CV[row_] );
				else
					if( _type_CV[row_] != VSOF_LPCOBJ )
						_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_CV[row_] );
					else
					{
						CORE_ASSERT(_lptempPU->prefix == (PF_IMM|IMM_LPCOBJ) );
						sprintf( _gvs.lpszBuffer, "%Fp", _lptempPU->du.zzk__LPCOBJ );
						_ret_gvs = 2;
					}

			}
			else
				_ret_gvs = PUGetValueString( _lptempPU, &_gvs, VSOF_REAL );
		}
		if( 0 == _ret_gvs )
			strcpy(lpvalstr_, "#ERR");	//invalid value
	}
	else	//show data link
	{
		DECODE	_decode;

		_decode.wAccess = 0;

	if( PUDecode( _lptempPU, &_decode ) )
		CombineDecode(lpvalstr_, &_decode);
	else
		strcpy( lpvalstr_, "#ERR" );	//invalid data-link
	}

	return TRUE;
}



//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _CVStructFORMSetData( LONG lData_, LPSTR lpvalstr_, UINT row_, UINT col_ )
{
	LPLINKMATRIX	_lpLinkMatrix = (LPLINKMATRIX)lData_;
	LPPU			_lptempPU;

	if(  ( row_ >= (WORD)_lpLinkMatrix->row )
		||( col_ >= (WORD)_lpLinkMatrix->col ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );
	if( _lpLinkMatrix->Nomeaning[0] )		//set value
	{
		if( row_ <= CV_UNIT )
		{
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, CV_VALUE, col_ );
			if( row_ == CV_VALUE )
				PUSetValueString( _lptempPU, lpvalstr_, _type_CV[row_] );
/*			else
				if( row_ == CV_HSCALE )
				{
					_lptempPU->prefix += ( RTU_MAX - RTU_VALUE);
					PUSetValueString( _lptempPU, lpvalstr_, _type_CV[row_] );
					_lptempPU->prefix -= ( RTU_MAX - RTU_VALUE);
				}
				else
					if( row_ == CV_LSCALE )
					{
						_lptempPU->prefix += ( RTU_MIN - RTU_VALUE);
						PUSetValueString( _lptempPU, lpvalstr_, _type_CV[row_] );
						_lptempPU->prefix -= ( RTU_MIN - RTU_VALUE);
					}*/
				return TRUE;
		}
		else
			if( row_ < NUMBER_CVPARAM )
				if( row_ == CV_PRIV )
					return FALSE;
				else
					return (BOOL)PUSetValueString( _lptempPU, lpvalstr_, _type_CV[row_] );
			else
				return (BOOL)PUSetValueString( _lptempPU, lpvalstr_, VSOF_REAL );
	}
	else	//set data link
	{
		ENCODE	_encode;
		_encode.lpszBuffer = lpvalstr_;
		_encode.wAccess = 0;
		_encode.wError = 0;
		if( row_ < NUMBER_CVPARAM )
		{
			if( row_ <= CV_UNIT && row_ != CV_VALUE  )
			{
				if( _type_DV[row_] == VSOF_REAL )
					strcpy(lpvalstr_, "real");
				if( _type_DV[row_] == VSOF_LPCOBJ )
					strcpy(lpvalstr_, "cobj");
				if( _type_DV[row_] == VSOF_BOOL )
					strcpy(lpvalstr_, "bool");
				if( _type_DV[row_] == VSOF_WORD )
					strcpy(lpvalstr_, "word");
			}
		}

		return PUEncode( _lptempPU, &_encode );
	}
}

//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _CVStructFORMGetColTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wCol_)
{
	sprintf( szBuffer_, "%i", wCol_ + 1 );
	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _CVStructFORMGetRowTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wRow_)
{
	sprintf( szBuffer_, "%s", _CVDiscription[wRow_] );
	return TRUE;
}
//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _DVStructFORMGetData( LONG lData_, LPSTR lpvalstr_, DWORD size_, UINT row_, UINT col_ )
{
	LPLINKMATRIX	_lpLinkMatrix = (LPLINKMATRIX)lData_;
	LPPU			_lptempPU;
	GETVALUESTRING	_gvs;
	DWORD			_ret_gvs;

	_gvs.lpszBuffer = lpvalstr_;
	_gvs.wSize = (WORD)size_;

	if(  ( row_ >= (DWORD)_lpLinkMatrix->row )
		||( col_ >= (DWORD)_lpLinkMatrix->col ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	//_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );
	_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );
	if( _lpLinkMatrix->Nomeaning[0] )		//show value
	{
		if( row_ <= DV_UNIT )
		{
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, DV_VALUE, col_ );
			if( row_ == DV_VALUE )
				_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_DV[row_] );
			else
/*				if( row_ == DV_HSCALE )
				{
					_lptempPU->prefix += ( RTU_MAX - RTU_VALUE );
					_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_DV[row_] );
					_lptempPU->prefix -= ( RTU_MAX - RTU_VALUE );
				}
				else
				{
					if( row_ == DV_LSCALE )
					{
						_lptempPU->prefix += ( RTU_MIN - RTU_VALUE );
						_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_DV[row_] );
						_lptempPU->prefix -= ( RTU_MIN - RTU_VALUE );
					}
					else*/
			{
				switch( row_ )
				{
					case DV_TAG:
						{
							_lptempPU->prefix += ( RTU_TAG - RTU_VALUE );
							_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_DV[row_] );
							_lptempPU->prefix -= ( RTU_TAG - RTU_VALUE );
							break;
						}
					case DV_NAME:
						{
							_lptempPU->prefix += ( RTU_COMMENT - RTU_VALUE );
							_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_DV[row_] );
							_lptempPU->prefix -= ( RTU_COMMENT - RTU_VALUE );
							break;
						}
					case DV_UNIT:
						{
							_lptempPU->prefix += ( RTU_UNIT - RTU_VALUE );
							_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_DV[row_] );
							_lptempPU->prefix -= ( RTU_UNIT - RTU_VALUE );
							break;
						}
					case DV_DATANAME:
						{
							strcpy( lpvalstr_, "" );	
							_ret_gvs = 4;
							break;
						}								
					_ret_gvs = 2;
				}
			}
		}
		else
		{
			if( row_ < NUMBER_DVPARAM )
			{
				if( row_ < DV_PRIV )
					_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_DV[row_] );
				else
					if( _type_DV[row_] != VSOF_LPCOBJ )
						_ret_gvs = PUGetValueString( _lptempPU, &_gvs, _type_DV[row_] );
					else
					{
						CORE_ASSERT(_lptempPU->prefix == (PF_IMM|IMM_LPCOBJ) );
						sprintf( _gvs.lpszBuffer, "%Fp", _lptempPU->du.zzk__LPCOBJ );
						_ret_gvs = 2;
					}

			}
			else
				_ret_gvs = PUGetValueString( _lptempPU, &_gvs, VSOF_REAL );
		}
		if( 0 == _ret_gvs )
			strcpy(lpvalstr_, "#ERR");	//invalid value
	}
	else	//show data link
	{
		DECODE	_decode;

		_decode.wAccess = 0;

		if( PUDecode( _lptempPU, &_decode ) )
			CombineDecode(lpvalstr_, &_decode);
		else
			strcpy( lpvalstr_, "#ERR" );	//invalid data-link
	}

	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _DVStructFORMSetData( LONG lData_, LPSTR lpvalstr_, UINT row_, UINT col_ )
{
	LPLINKMATRIX	_lpLinkMatrix = (LPLINKMATRIX)lData_;
	LPPU			_lptempPU;

	if(  ( row_ >= (DWORD)_lpLinkMatrix->row )
		||( col_ >= (DWORD)_lpLinkMatrix->col ) )
	{
		CORE_ASSERT( FALSE );
		return FALSE;
	}

	_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, row_, col_ );

	if( _lpLinkMatrix->Nomeaning[0] )		//set value
	{
		if( row_ <= DV_UNIT )
		{
			_lptempPU = DLM_GET_LPPU( _lpLinkMatrix, DV_VALUE, col_ );
			if( row_ == DV_VALUE )
				PUSetValueString( _lptempPU, lpvalstr_, _type_DV[row_] );
			/*else
				if( row_ == DV_HSCALE )
				{
					_lptempPU->prefix += ( RTU_MAX - RTU_VALUE);
					PUSetValueString( _lptempPU, lpvalstr_, _type_DV[row_] );
					_lptempPU->prefix -= ( RTU_MAX - RTU_VALUE);
				}
				else
					if( row_ == DV_LSCALE )
					{
						_lptempPU->prefix += ( RTU_MIN - RTU_VALUE);
						PUSetValueString( _lptempPU, lpvalstr_, _type_DV[row_] );
						_lptempPU->prefix -= ( RTU_MIN - RTU_VALUE);
					}*/
				return TRUE;
		}
		else
			if( row_ < NUMBER_DVPARAM )
				if( row_ == DV_PRIV )
					return FALSE;
				else
					return (BOOL)PUSetValueString( _lptempPU, lpvalstr_, _type_DV[row_] );
			else
				return (BOOL)PUSetValueString( _lptempPU, lpvalstr_, VSOF_REAL );
	}
	else	//set data link
	{
		ENCODE	_encode;
		_encode.lpszBuffer = lpvalstr_;
		_encode.wAccess = 0;
		_encode.wError = 0;
		if( row_ < NUMBER_DVPARAM )
		{
			if( row_ <= DV_UNIT && row_ != DV_VALUE  )
			{
				if( _type_DV[row_] == VSOF_REAL )
					strcpy(lpvalstr_, "real");
				if( _type_DV[row_] == VSOF_LPCOBJ )
					strcpy(lpvalstr_, "cobj");
				if( _type_DV[row_] == VSOF_BOOL )
					strcpy(lpvalstr_, "bool");
				if( _type_DV[row_] == VSOF_WORD )
					strcpy(lpvalstr_, "word");
			}
		}

		return PUEncode( _lptempPU, &_encode );
	}
}

//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _DVStructFORMGetColTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wCol_)
{
	sprintf( szBuffer_, "%i", wCol_ + 1 );
	return TRUE;
}

//...................................................................
//...................................................................
#pragma argsused
LPFORMDATA _DVStructFORMGetRowTitle( LONG lData_, LPSTR szBuffer_, DWORD wSize_, UINT wRow_)
{
	sprintf( szBuffer_, "%s", _DVDiscription[wRow_] );
	return TRUE;
}

LPFORMDATA _DVStructFORMData( LPCELLINFO lpCellInfo_ )
{
	if( lpCellInfo_->Event & FORMEV_GETTEXT ||
			lpCellInfo_->Event & FORMEV_GETEDITTEXT )	
	{ 
		if( lpCellInfo_->Row == COLTITLE )
			return _DVStructFORMGetColTitle( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Col );
		if( lpCellInfo_->Col == ROWTITLE )
			return _DVStructFORMGetRowTitle( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Row );

		return _DVStructFORMGetData( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Row, lpCellInfo_->Col );
	}

	if( lpCellInfo_->Event & FORMEV_SETTEXT )
		return _DVStructFORMSetData( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->Row, lpCellInfo_->Col );

	if( lpCellInfo_->Event & FORMEV_CANEDIT )
		return TRUE;
	return FALSE;
}
LPFORMDATA _MVStructFORMData( LPCELLINFO lpCellInfo_ )
{
	if( lpCellInfo_->Event & FORMEV_GETTEXT || 
			lpCellInfo_->Event & FORMEV_GETEDITTEXT )	
	{
		if( lpCellInfo_->Row == COLTITLE )
			return _MVStructFORMGetColTitle( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Col );
		if( lpCellInfo_->Col == ROWTITLE )
			return _MVStructFORMGetRowTitle( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Row );

		return _MVStructFORMGetData( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Row, lpCellInfo_->Col );
	}

	if( lpCellInfo_->Event & FORMEV_SETTEXT )
		return _MVStructFORMSetData( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->Row, lpCellInfo_->Col );

	if( lpCellInfo_->Event & FORMEV_CANEDIT )
		return TRUE;
	return FALSE;
}
LPFORMDATA _CVStructFORMData( LPCELLINFO lpCellInfo_ )
{
	if( lpCellInfo_->Event & FORMEV_GETTEXT ||
			lpCellInfo_->Event & FORMEV_GETEDITTEXT )
	{
		if( lpCellInfo_->Row == COLTITLE )
			return _CVStructFORMGetColTitle( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Col );
		if( lpCellInfo_->Col == ROWTITLE )
			return _CVStructFORMGetRowTitle( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Row );

		return _CVStructFORMGetData( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->TextSize, lpCellInfo_->Row, lpCellInfo_->Col );
	}

	if( lpCellInfo_->Event & FORMEV_SETTEXT )
		return _CVStructFORMSetData( lpCellInfo_->lData, lpCellInfo_->lpText, lpCellInfo_->Row, lpCellInfo_->Col );

	if( lpCellInfo_->Event & FORMEV_CANEDIT )
		return TRUE;
	return FALSE;
}
