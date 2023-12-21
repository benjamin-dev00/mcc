//===================================================================
//
//Zheng zhikai	96-8-12
//===================================================================
#include "prehead.h"

#include "coreall.h"
#include "stmpdef.h"


//...................................................................
//...................................................................
#pragma argsused
_DLLEXP BOOL _DLLFUNC CombinePaint(WRD wFTU_, HDC hdc_)
{
	//need to do nothing
	return TRUE;
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC CombineParam(PIC wCmd_, WRD wFTU_, WORD wNo_, LONG lParam_)
{
	static PDU _sPDU[] =
	{
		_PDU_DEFAULT
	};

	return _DefStampParam(wCmd_,
						  wFTU_,
						  wNo_,
						  lParam_,
						  sizeof(_sPDU)/sizeof(PDU),
						  IDS_COMBINE,
						  &(_sPDU[0]) );
}

//...................................................................
//...................................................................
_DLLEXP DWORD _DLLFUNC CombineBuild(SCMD wCmd_, WRD wFTU_, HWND hwnd_, WORD wParam_, LONG lParam_)
{
	switch(wCmd_)
	{
		case SCMD_INIT:
			return TRUE;

		case SCMD_GETREGION:
		{
			if( IsFaceSelect( wFTU_ ) )
			{
				RECT	_rc;

				FaceXycxcyToRect( wFTU_, &_rc);

				wParam_ ++;
				InflateRect( &_rc, wParam_, wParam_ );

				return (WORD)CreateRectRgnIndirect( &_rc );
			}
			else
			{
				//because the Combinor is transparent
				return NULL;
			}
		}

		/*
		case SCMD_UPDATE:
		{
			if( IsFaceSelect( wFTU_ ) )
				DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );

			return TRUE;
		}
		*/

		case SCMD_CONSTRUCT:
			return TRUE;

		case SCMD_CONSTRUCT_INPUTMSG:
			return FIA_PROCESSEND;

		case SCMD_GETINVALIDRGN:
			//although this is transparent face, using default process
			//handle is still a simple way!
		case SCMD_EDIT:
		case SCMD_EDIT_INPUTMSG:
		case SCMD_MOVE:
		default:
			return DefStampBuild( wCmd_, wFTU_, hwnd_, wParam_, lParam_ );
	}

}

