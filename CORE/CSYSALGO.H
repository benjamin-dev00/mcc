//===================================================================
//csysstmp.h
//	Head file of csysstmp.c
//		system stamps
//Zheng zhikai	95-2-6
//===================================================================

#ifndef __CSYSALGO_H
#define __CSYSALGO_H

//The kind of ID:
//		0x0000-0000
//         | |  |
//         | |  +-------the subclass of the algorithm
//         | +----------the class of the algorithm
//         +------------the general class of the algorithm
//the general class of the algorithm
#define	AC0_SYSTEM				0X01000000L
#define AC0_FLOWCONTROL			0X02000000L
#define AC0_MATH				0X03000000L
#define AC0_CONTROLALGORITHM	0X04000000L
#define AC0_DATAFILTER			0X05000000L
#define AC0_SIGNGENERATOR		0X06000000L
#define AC0_OTHERS				0X07000000L

//define system algorithms
#define IDA_NULL				0X01010000L	//Null
#define IDA_ROOT				0X01020000L	//Root

//define flow control algorithms
#define	IDA_START				0X02010000L	//Create an initial execute thread
#define	IDA_EXECUTE				0X02020000L	//Create a new execute thread
#define	IDA_GOTO				0X02030000L	//Goto
#define	IDA_CALL				0X02040000L	//Call
#define	IDA_RETURN				0X02050000L	//Return for Call
#define	IDA_LOOP				0X02060000L	//Loop
#define	IDA_BREAK				0X02070000L	//Break for Loop
#define	IDA_END					0X02080000L	//End an execute thread

/*
Executable Thread:
	An executable thread is an independent executable sequence;
	It is always organized as a tree, and it ends after all the
	nodes of the tree iterated or an "End" modu met, a thread
	can be suspended for waiting to certain condition rising.

	The following kinds of modu causes the current executable
	thread suspends:
		(1)	"Loop" modu
		(2) "Wait" modu
	When the current executable thread suspends, the system
	schedules other executable thread as the current executable
	if there are others executable thread.

Algorithms:
	The following sections give explanations for the modus
	which refers to running flow control

IDA_START:
	"Start" modu causes that the system treats its children
	modu tree as an executable thread tree; When the system
	initiates, the system creases a new executable thread for
	this tree.

	"Start" modu always sets with MTU_SKIPCHILD flag;

	"Start" modu is ONLY permitted to be created under the
	"Root" node, which is the root node of whole modu tree;

IDA_EXECUTE
	"Execute" modu has a parameter which points to a meter
	which is the root of a modu tree, whenever the "Execute"
	modu is executed, the system create a new executable thread
	for that modu tree;

	Defferent to the "Start" modu, "Execute" modu can be
	placed as any node of the modu, and it doesn't imply that
	its children tree is an executable thread tree;

IDA_GOTO
	"Goto" modu causes the change of the executing flow at run
	time, "Goto" modu has a paramter pointing to the aim modu;
	Note: "Goto" modu is not implemented now because the end
	point can't be decided.

IDA_CALL
	"Call" modu causes the change of the execute flow at run
	time, when this executing flow meets a "Return" modu, the
	executing flow will back to the next modu of the call point;
	When an "End" modu is met dring the call, the executable
	thread will terminate;

IDA_LOOP
	"Loop" modu causes the system does loop in its children tree
	if the loop condition is TRUE, when a loop ends, the system
	suspends the executable thread until the next loop, during
	the loop interval, the system change to other executable thread;

	"Loop" modu always sets with MTU_SKIPCHILD flag at all	times;

IDA_BREAK
	"Break" modu break a loop; the "Break" modu is as same as "Null"
	modu if it is not in loop tree.

IDA_END
	Whenever an "End" modu met, the current executable thread will
	terminate.

IDA_IF
	"If" modu switches executing flow, "If" modu organizes its
	THEN clause and ELSE clause in its child tree:
		"If"
		 |
		 +---- THEN clause here
		 |
		 +---- ELSE clause here

	"If" modu always sets with MTU_SKIPCHILD flag;
*/

#define _DefAlgoParam(wCmd_, wrd_, wNo_, lParam_, wNum_, id_, lpPDU_)		\
	DefProcessPIC(wCmd_, wrd_, wNo_, lParam_, TBX_MT, wNum_, (DWORD)id_, lpPDU_)

#endif 		//__CSYSALGO.H

