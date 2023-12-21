/*  assert.h
	assert macro

	!!!!! Changed By Zheng zhikai from borland.c: assert.h !!!!! 	1995-11-4

*/

/* !!!!!! Zheng zhikai NOTE:
	At this section :
	 . redefines __assertfail funcction. The new function
	   does not abort application.
	 . formats source code in order for more readable, and
	 . changes code:
			- cancels old __assertfail
			- renames assert() to CORE_ASSERT()
			- cancels the '__cond' parameter:
			  change '#p' to 'NULL'
*/

#ifndef __ZASSERT_H
#define __ZASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

_DLLEXP BOOL _ZZKDECL ZzkAssertFail(LPCSTR file_, int line_);

#undef CORE_ASSERT

#ifdef NDEBUG
	#define CORE_ASSERT(p)   ((void)0)
#else
	#ifdef _CORE_EXE	//this will be defined when creation a .exe file with MSC&MFC
		#define CORE_ASSERT		ASSERT
	#else
		//use my assertion here
        //#define CORE_ASSERT(p) ((p) ? (void)0 : (void) ZzkAssertFail( __FILE__, __LINE__ ) )
		#define CORE_ASSERT(p) \
			do	\
			{	\
			if( !(p) && ZzkAssertFail( __FILE__, __LINE__ )) \
				_asm { int 3 }; \
			} while (0) 
	#endif

#endif

#ifdef  __cplusplus
}
#endif

#endif  //__ZASSERT.H

