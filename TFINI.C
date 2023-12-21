#include "tfalgo.h"
#include "tfcmdmc.h"
#include "tfmat.h"
#ifdef __ONSPEC__
#include "Mtmfun.h"
#include "Mtdllfun.h"
#endif

// debug, or __debug?
#ifdef debug
#include "tfdebug.h"
#endif

ACRET PrivInitiate(LPCOBJPRIV lpCobjPrivAll_ )
{
	GlobalInitiate( lpCobjPrivAll_ );

	ApartSR( lpCobjPrivAll_ );

	return ACRET_TRUE;
}
