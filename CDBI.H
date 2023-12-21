//===================================================================
//CDBI.H
//	head file for CDBI.C
//Zheng zhikai	96-10-20
//===================================================================

#ifndef __CDBI_H
#define __CDBI_H

//this function can be exported
_DLLEXP BOOL _ZZKDECL TestLogging( WORD wLoopTimes_ );

//non-exportable functions
BOOL InitialDBI(void);
BOOL FreeDBI(void);
BOOL LogCore( int iMode_ );
void GetDBIError(void);

/////////////////////////////////
//the following data struct is the cache for history data log
//
//data organising in cache:
//	1.	the first unit is cache head struct,
//	2.	then an array stores the data items:	CACHECELL	_cc[the number of data item];
typedef struct _tagCACHE_CELL {
	long	time;	// time: in seconds, elapsed since 00:00:00 GMT, January 1, 1970
	float	val;	//logging value at this time
}CACHECELL, FAR* LPCACHECELL;

typedef struct _tagCACHE_HEAD {
	long        max_cc;			//the max number of CACHECELL
	long		num_cc;			//the number of CACHECELL
	BOOL		pack;			//whether pack data
	CORETIME	begin_time;		//backup of BegingDateTime
	CORETIME	end_time;		//backup of EndDateTime
	WORD		table;			//backup of table
	WRD			wrd;			//backup the WRD of data source
	CACHECELL	cc;				//CACHECELL array
}CACHEHEAD, FAR* LPCACHEHEAD;

#define	CACHE_MAX_ITEMS			(800)

_DLLEXP LPCACHEHEAD _ZZKDECL AllocateHisCache( int max_item_ );
_DLLEXP LPCACHEHEAD _ZZKDECL QueryHisData( LPCACHEHEAD lpch_,
										   WORD table_,
										   WORD wrd_,
										   CORETIME begin_time_,
										   CORETIME end_time_ );

#endif	//__CDBI_H
