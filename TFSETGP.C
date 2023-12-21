#include "tfalgo.h" 
#include "tfcmdmc.h"
ACRET SetGlobalParameter( LPCOBJPRIV __lpSource__, WORD __param__, LPPU __lpPUBase__ )
{
	SimplePU __pu__;
	BYTE FAR *		 __lpStart;

	if( __param__ < NUMBER_GLOBALPARAMWORD )
	{
		if( __param__ <= MDMCIRVD_FLAGAM )
		{
			__pu__.prefix = PF_IMM|IMM_WORD;
			__pu__.du.zzk__FULL = NULL;
			PURead( &__pu__, (__lpPUBase__+__param__) );
			 ( * (WORD *)( (BYTE FAR*) (__lpSource__) + __param__ * sizeof(WORD) ) ) = 
				__pu__.du.zzk__WORD;
		}
		else
			if( __param__ == MDMCIRVD_FLAGAM )
			{
				__pu__.prefix = PF_IMM|IMM_BOOL;
				__pu__.du.zzk__FULL = NULL;
				PURead( &__pu__, (__lpPUBase__+__param__) );
				__lpSource__->Flag_AM = __pu__.du.zzk__BOOL;
			}
			else
			{
				if( __param__ == MDMCIRVD_FREECOUNT )
				{
					__pu__.prefix = PF_IMM|IMM_WORD;
					__pu__.du.zzk__FULL = NULL;
					PURead( &__pu__, (__lpPUBase__+__param__) );
					__lpSource__->Freecount = __pu__.du.zzk__WORD;
				}
				else
				{
					__pu__.prefix = PF_IMM|IMM_WORD;
					__pu__.du.zzk__FULL = NULL;
					PURead( &__pu__, (__lpPUBase__+__param__) );
					__lpStart = (BYTE FAR*)&( __lpSource__->MaxDelay );
					( * (WORD *)( (BYTE FAR*) (__lpStart ) + ( __param__ - MDMCIRVD_MAXDELAY ) * sizeof(WORD) ) ) = 
						__pu__.du.zzk__WORD;
				}
			}
	}
	else
	{
		if( __param__ < ( NUMBER_GLOBALPARAMWORD + NUMBER_GLOBALPARAMBOOL )  )
		{
			__lpStart = (BYTE FAR*)&( __lpSource__->Flag_CControl );
			__pu__.prefix = PF_IMM|IMM_BOOL;
			__pu__.du.zzk__FULL = NULL;  
			PURead( &__pu__, (__lpPUBase__+__param__) );
			 ( * (BOOL *)( (BYTE FAR*) (__lpStart) + ( __param__ - NUMBER_GLOBALPARAMWORD ) * sizeof(BOOL) ) ) = 
					__pu__.du.zzk__BOOL;
		}
		else
		{
			if( __param__ < ( NUMBER_GLOBALPARAMWORD + NUMBER_GLOBALPARAMBOOL + NUMBER_GLOBALPARAMDOUBLE ) )
			{
				__lpStart = (BYTE FAR*)&( __lpSource__->LpK );
				__pu__.prefix = PF_IMM|IMM_REAL;
				__pu__.du.zzk__FULL = NULL;  
				PURead( &__pu__, (__lpPUBase__+__param__) );
				 ( * (double *)( (BYTE FAR*) (__lpStart) + ( __param__ - NUMBER_GLOBALPARAMWORD - NUMBER_GLOBALPARAMBOOL ) * sizeof(double) ) ) = 
						__pu__.du.zzk__REAL;
			}
			else
			{
				if( __param__ < ( NUMBER_GLOBALPARAMBOOL + NUMBER_GLOBALPARAMWORD + NUMBER_GLOBALPARAMDOUBLE + NUMBER_GLOBALPARAMCOBJ ) )
				{
					BYTE FAR* _lptemp;
					__lpStart = &( __lpSource__->LPG );
					__pu__.prefix = PF_IMM|IMM_LPCOBJ;
					__pu__.du.zzk__FULL = NULL;  
					PURead( &__pu__, (__lpPUBase__+__param__) );
					_lptemp = (BYTE FAR*) (__lpStart) + ( __param__ - NUMBER_GLOBALPARAMWORD - NUMBER_GLOBALPARAMBOOL - NUMBER_GLOBALPARAMDOUBLE ) * sizeof(LPCOBJ);
					if( __pu__.du.zzk__LPCOBJ )
						( * (LPCOBJ *)( _lptemp ) ) = 
							__pu__.du.zzk__LPCOBJ;
				}
			}
		}
	}
/*	if( __param__ >= NUMBER_GLOBALPARAMDISPLAY )
	{
		if( __param__ == MDMCIRVD_Modu )
		{
			__pu__.prefix = PF_IMM|IMM_WORD;
			__pu__.du.zzk__FULL = NULL;  
			PURead( &__pu__, (__lpPUBase__+__param__) );
			__lpSource__->wModu = __pu__.du.zzk__WORD;
		}
		if( __param__ == MDMCIRVD_Save )
		{
			__pu__.prefix = PF_IMM|IMM_BOOL;
			__pu__.du.zzk__FULL = NULL;  
			PURead( &__pu__, (__lpPUBase__+__param__) );
			__lpSource__->Flag_Save = __pu__.du.zzk__BOOL;
		}
		if( __param__ > MDMCIRVD_Save )
		{
			if( __param__ <= MDMCIRVD_LPPredD )
			{
				__lpStart = &( __lpSource__->LPINMV );
				__pu__.prefix = PF_IMM|IMM_LPCOBJ;
				__pu__.du.zzk__FULL = NULL;  
				PURead( &__pu__, (__lpPUBase__+__param__) );
				 ( * (LPCOBJ *)( (BYTE FAR*) (__lpStart) + (__param__ - MDMCIRVD_INMV ) * sizeof(LPCOBJ) ) ) = 
						__pu__.du.zzk__LPCOBJ;
			}
		}
		if( __param__ == MDMCIRVD_DFILTER )
		{
			__pu__.prefix = PF_IMM|IMM_REAL;
			__pu__.du.zzk__FULL = NULL;  
			PURead( &__pu__, (__lpPUBase__+__param__) );
			__lpSource__->DFilter = __pu__.du.zzk__REAL;
		}
		if( __param__ == MDMCIRVD_N1 )
		{
			__pu__.prefix = PF_IMM|IMM_WORD;
			__pu__.du.zzk__FULL = NULL;  
			PURead( &__pu__, (__lpPUBase__+__param__) );
			__lpSource__->N1 = __pu__.du.zzk__WORD;
		}
		if( __param__ == MDMCIRVD_N2 )
		{
			__pu__.prefix = PF_IMM|IMM_WORD;
			__pu__.du.zzk__FULL = NULL;  
			PURead( &__pu__, (__lpPUBase__+__param__) );
			__lpSource__->N2 = __pu__.du.zzk__WORD;
		}
		if( __param__ == MDMCIRVD_ERRORCODE )
		{
			__pu__.prefix = PF_IMM|IMM_WORD;
			__pu__.du.zzk__FULL = NULL;  
			PURead( &__pu__, (__lpPUBase__+__param__) );
			__lpSource__->ErrorCode = __pu__.du.zzk__WORD;
		}
		if( __param__ == MDMCIRVD_INISTART )
		{
			__pu__.prefix = PF_IMM|IMM_BOOL;
			__pu__.du.zzk__FULL = NULL;  
			PURead( &__pu__, (__lpPUBase__+__param__) );
			__lpSource__->IniStart = __pu__.du.zzk__BOOL;
		}
		if( __param__ == MDMCIRVD_STEADYSTEP )
		{
			__pu__.prefix = PF_IMM|IMM_LPCOBJ;
			__pu__.du.zzk__FULL = NULL;  
			PURead( &__pu__, (__lpPUBase__+__param__) );
			__lpSource__->LPIsSteady = __pu__.du.zzk__LPCOBJ;
		}
	}*/
	return ACRET_TRUE;
}