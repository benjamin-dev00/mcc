//===================================================================
//csysstmp.h
//	Head file of csysstmp.c
//		system stamps
//Zheng zhikai	95-2-6
//===================================================================

#ifndef __CSYSSTMP_H
#define __CSYSSTMP_H

//The kind of ID:
//		0x0000-0000
//         | |  |
//         | |  +-------the subclass of the stamp
//         | +----------the class if the stamp
//         +------------the general class of the stamp
//						now:
//						01:	system
//						02: draw
//						03:	window
//						04:	trend
//the general class of the stamp

//defines system stamps id
//NULL stamp
#define IDS_NULL			0X01010100L
//ROOT stamp
#define IDS_ROOT			0X01020100L
//FACEWnd
#define IDS_FACEWND			0X03010100L


//FaceWnd() parameter order
#define FACEWND_NAME				FACE_FIRSTPARAM + 0
#define FACEWND_WNDX				FACE_FIRSTPARAM + 1
#define FACEWND_WNDY				FACE_FIRSTPARAM + 2
#define FACEWND_WNDCX				FACE_FIRSTPARAM + 3
#define FACEWND_WNDCY				FACE_FIRSTPARAM + 4
#define FACEWND_BACKCLR				FACE_FIRSTPARAM + 5 
#define FACEWND_CLIENTCX			FACE_FIRSTPARAM + 6
#define FACEWND_CLIENTCY			FACE_FIRSTPARAM + 7
#define FACEWND_STYLE				FACE_FIRSTPARAM + 8
#define FACEWND_EXSTYLE				FACE_FIRSTPARAM + 9
#define FACEWND_HWND				FACE_FIRSTPARAM + 10
#define FACEWND_STATE				FACE_FIRSTPARAM + 11
//FaceWnd() parameter type
#define FACEWND_CLIENTCX_PDT		PDT_FCCRD
#define FACEWND_CLIENTCY_PDT		PDT_FCCRD
#define FACEWND_HWND_PDT			PDT_HANDLE
#define FACEWND_NAME_PDT			PDT_LPCOBJ
#define FACEWND_STYLE_PDT			PDT_DWORD
#define FACEWND_EXSTYLE_PDT			PDT_DWORD
#define FACEWND_WNDX_PDT			PDT_int
#define FACEWND_WNDY_PDT			PDT_int
#define FACEWND_WNDCX_PDT			PDT_int
#define FACEWND_WNDCY_PDT			PDT_int
#define FACEWND_BACKCLR_PDT			PDT_COLORREF
#define FACEWND_STATE_PDT			PDT_DWORD

//define the meaning for FACEWND_STATE:
#define FW_DEACTIVATED				0x00000000L		//open and inactivated
#define FW_ACTIVATED				0x00000001L		//open and activated
#define FW_AUTOOPEN					0x80000000L		//auto open when cfs.exe initiate

/*
#define FW_OPEN						0x00000001L		//open and inactivated
#define FW_CLOSE					0x00000000L		//open and inactivated
*/

//inner function
BOOL RegisterSystemStamps(void);

#endif	//__CSYSSTMP_H



