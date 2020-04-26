/*
 * PCB.h
 *
 *  Created on: Aug 24, 2019
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_
#include "Thread.h"
#include "SCHEDULE.H"
#include "Queue.h"

class PCB {
public:
	unsigned* stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	Time timeSlice;
	Thread* myThread;
	enum state {NEW, READY, BLOCKED, DEAD};
	volatile state status;
	Time waitTime;
	int timeFlag;
	static PCB* running;

	static Queue allPCBs;

	Queue waitQueue;


	PCB(StackSize, Time, Thread*);
	virtual ~PCB();
	static void wrapper();
};

#endif /* PCB_H_ */
