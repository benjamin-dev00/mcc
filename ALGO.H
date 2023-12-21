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
#define IDA_CBDXREAL			0X01040100L	//CBDXReal
#define IDA_CBDXTINT			0X01050100L	//CBDXTint
#define IDA_CBDXBOOL			0X01060100L	//CBDXBool

//math group
#define IDA_ADDREALN			0X03010100L	//AddRealn
#define IDA_ADDTINTN			0X03010200L	//AddTintn

//Data Filter
#define IDA_STAIR				0X05010100L	//Stair

//sign generator
#define IDA_RANDINT				0X06010100L	//RandInt
#define IDA_RANDX				0X06010200L	//Randx
#define IDA_SINE				0X06020100L	//sine

//others
#define IDA_TIMER				0X07010100L	//Timer

