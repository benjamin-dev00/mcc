#ifndef _CDAOI_COMPILE_
#define _CDAOI_COMPILE_

#define DLLEXP __declspec(dllexport)

#include <sqlext.h>
#include <odbcinst.h>
#include <afxdao.h>

typedef struct _tag_SQL_RESULT_REAL{
	SDWORD              cbdt;
	long                dt;
	float               val;
	SDWORD              cbval;
}SQL_RESULT_REAL;

typedef struct _tag_SQL_RESULT_TINT{
	long                dt;
	SDWORD              cbdt;
	long                val;
	SDWORD              cbval;
}SQL_RESULT_TINT;

typedef struct _tag_SQL_RESULT_BOOL{
	long                dt;
	SDWORD              cbdt;
	BOOL                val;
	SDWORD              cbval;
}SQL_RESULT_BOOL;

class CDaoI
{
public:
	CString m_DatabaseName; 
	CDaoDatabase * m_pDB;
	CDaoRecordset * m_pRS;

public:
	CDaoI(LPCSTR lpszDBName_);
	CDaoI(void);
	BOOL Initialize(LPCSTR lpszDBName_);
	static BOOL CreateDatabase(LPCSTR lpszDBName);

	virtual ~CDaoI();
	
	////////////////////////////////////////////////
	//   Initialization and uninitialization of 
	// DaoDatabase has been done in CDaoI and ~CDaoI
	// so there is no neseccery to use InitialDBI_DAO
	// and FreeDBI_DAO function.
	////////////////////////////////////////////////
	BOOL InitialDBI_DAO(void);

	BOOL FreeDBI_DAO(void);
	////////////////////////////////////////////////


	BOOL LogCore_DAO( int iMode_ );
	BOOL LogCore_TEST( void);
	
	LPCACHEHEAD QueryHisData_DAO( 
								LPCACHEHEAD lpch_,
								WORD table_,
								WORD wrd_,
								CORETIME begin_time_,
								CORETIME end_time_ );
private:
	long _near QueryResultRows( LPCSTR lpszTableName_,
								WORD wrd_,
								CORETIME begin_time_,
								CORETIME end_time_ );							
	//Real data operation
	LPCACHEHEAD _near QueryDataReal( 
								LPCACHEHEAD lpch_,
								WORD wrd_,
								CORETIME begin_time_,
								CORETIME end_time_ );

	int _QueryDataBeforeReal(	SQL_RESULT_REAL FAR * _lpset,
								WRD wrd_,
								CORETIME begin_time_ );

	int _QueryDataAfterReal(	SQL_RESULT_REAL FAR * _lpset,
								WRD wrd_,
								CORETIME begin_time_ );

	void _FetachDataReal(		LPCACHEHEAD lpch_ ,
								WORD wrd_,
								CORETIME begin_time_,
								CORETIME end_time_ );

	BOOL _CommitDataReal(		LPCACHEHEAD lpch_, 
								LPCACHECELL lp_, 
								int option_ );

	void _PackDataReal(			LPCACHECELL lpcc_, 
								long _dt , 
								float _val );

	void _FetachDataPackReal(	LPCACHEHEAD lpch_ ,
								WORD wrd_,
								CORETIME begin_time_,
								CORETIME end_time_ );
	//Tint data operation
	LPCACHEHEAD _near QueryDataTint( 
								LPCACHEHEAD lpch_,
								WORD wrd_,
								CORETIME begin_time_,
								CORETIME end_time_ );


};

extern "C"
{
	DLLEXP BOOL CreateDatabase_DAO( LPCSTR szDBName_ );
	DLLEXP BOOL InitialDBI_DAO( LPCSTR lpszDBName_ );
	DLLEXP BOOL FreeDBI_DAO(void);
	DLLEXP BOOL LogCore_DAO( int iMode_ );
	DLLEXP LPCACHEHEAD QueryHisData_DAO( LPCACHEHEAD lpch_, WORD table_, WORD wrd_, long begin_time_, long end_time_ );
}

#endif	//_CDAOI_COMPILE_
