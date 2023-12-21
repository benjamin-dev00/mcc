//////////////////////////////////////////////////////////
//Mtconst.h												//
//S.D.Chen 97-10-23										//
//////////////////////////////////////////////////////////
#define TRUE   1
#define FALSE  0
#define GOODDATA 1
#define BADDATA	 2
#define RUN		TRUE
#define STOP	FALSE
#define AUTO	TRUE
#define MANU	FALSE
#define ON     0x02	//000000*0
#define OFF    0x00 //      |___1:set bit1 of status
					//          0:clear bit0 of status

#define STRINGLEN    8  //lengh of string of char-member
#define MINISAMPTIME 0.1