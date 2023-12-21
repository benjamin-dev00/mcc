//===================================================================
//CoreErr.h
//Zheng zhikai	96-7-2
//===================================================================

#ifndef __CERROR_H
#define __CERROR_H

#define	CLEAR_LAST_ERROR        	\
	gdwLastError = ERR_No_Error

#define	SET_LAST_ERROR( __core_err_ )			\
	gdwLastError =	(__core_err_)

//define the error constants

#define	ERR_No_Error					0x00000000L
#define	ERR_Referenced_RTU				0x00000001L
#define	ERR_Referenced_TTU				0x00000002L
#define	ERR_Referenced_BTU				0x00000003L
#define	ERR_Referenced_MTU				0x00000004L
#define	ERR_Referenced_FTU				0x00000005L

#define ERR_Invalid_Entries				0x00000006L

#define ERR_RTU_No_Free_Room			0x00000010L
#define ERR_TTU_No_Free_Room			0x00000011L
#define ERR_BTU_No_Free_Room			0x00000012L
#define ERR_MTU_No_Free_Room			0x00000013L
#define ERR_FTU_No_Free_Room			0x00000014L
#define ERR_ATU_No_Free_Room			0x00000015L
#define ERR_STU_No_Free_Room			0x00000016L
#define ERR_OTU_No_Free_Room			0x00000017L

#define ERR_Invalid_RTU					0x00000020L
#define ERR_Invalid_TTU					0x00000021L
#define ERR_Invalid_BTU					0x00000022L
#define ERR_Invalid_MTU					0x00000023L
#define ERR_Invalid_FTU					0x00000024L

#define	ERR_Invalid_Min_Value_Max		0x00000030L
#define	ERR_Illegal_Tag					0x00000040L
#define	ERR_Not_Unique_Tag				0x00000050L

#define	ERR_Invalid_Core_Path           0x00000060L
#define	ERR_Global_Allocate_Failure		0x00000070L
#define	ERR_Global_Lock_Failure			0x00000080L
#define	ERR_Frag_Allocate_Failure		0x00000090L

#define ERR_Local_Allocate_Failure		0x00000090L
#define ERR_LocalInit_Failure			0x000000A0L
#define ERR_FragPool_Full				0x000000B0L
#define ERR_LocalFree_Failure			0x000000C0L

#define ERR_ReadOnly					0x000000D0L

#define ERR_Invalid_Algo				0x000000E0L
#define ERR_Invalid_Stamp				0x000000F0L
#define ERR_Invalid_Parent_Node			0x00000100L

#define ERR_Other_Corebase_In_Mem		0x00000120L
#define ERR_Action_On_System_Node		0x00000130L

#define ERR_MAX_and_MIN					0x00000141L
#define ERR_HIHI_and_HI					0x00000142L
#define ERR_HIHI_and_LO					0x00000143L
#define ERR_HIHI_and_LOLO				0x00000144L
#define ERR_HI_and_LO					0x00000145L
#define ERR_HI_and_LOLO					0x00000146L
#define ERR_LO_and_LOLO					0x00000147L
#endif	//__CERROR_H

