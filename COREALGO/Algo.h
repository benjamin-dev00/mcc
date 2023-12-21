//===================================================================
//algo.h
//Zheng zhikai	95-4-22
//===================================================================

//The kind of ID:
//		0x0000-0000
//         | |  |
//         | |  +-------the subclass of the algorithm
//         | +----------the class of the algorithm
//         +------------the general class of the algorithm
//the general class of the algorithm
/*
#define	AC0_SYSTEM				0X01000000L
#define AC0_FLOWCONTROL			0X02000000L
#define AC0_MATH				0X03000000L
#define AC0_CONTROLALGORITHM	0X04000000L
#define AC0_DATAFILTER			0X05000000L
#define AC0_SIGNGENERATOR		0X06000000L
#define AC0_OTHERS				0X07000000L
*/

//system group
#define IDA_CLOSEWND			0X01010100L	//CloseWnd
#define IDA_SHOWWND				0X01020100L	//ShowWnd
#define IDA_REPLACEWND			0X01030100L	//ReplaceWnd
#define IDA_EXCHANGER			0X01040100L	//Exchange Real
#define IDA_EXCHANGEN			0X01050100L	//Exchange Tint
#define IDA_EXCHANGEB			0X01060100L	//Exchange Bool

//Arithmetic Operation
#define IDA_ADDR				0X03010100L	//AddR
#define IDA_ADDN				0X03010200L	//AddN
#define IDA_MINUSR				0X03020100L	//MinusR
#define IDA_MINUSN				0X03020201L	//MinusN
#define IDA_MULTIR				0X03030100L	//MultiR
#define IDA_DIVIDER				0X03040100L	//DivideR 
#define IDA_ORB					0X03050300L	//OrB
#define IDA_ANDB				0X03060300L	//AndB

//Data Filter
#define IDA_STAIR				0X05010100L	//Stair

//sign generator
#define IDA_RANDN  				0X06010100L	//RandN  
#define IDA_RANDR				0X06010200L	//RandR
#define IDA_SIN 				0X06020100L	//sin

//others
#define IDA_TIMER				0X07010100L	//Timer

