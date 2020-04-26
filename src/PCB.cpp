/*
 * PCB.cpp
 *
 *  Created on: Aug 24, 2019
 *      Author: OS1
 */

#include "PCB.h"
#include <dos.h>
#include <stdio.h>

PCB* PCB::running = 0;
Queue PCB::allPCBs;

PCB::PCB(StackSize size, Time t, Thread* thread) {
	timeFlag = 1;
	waitTime = 0;
	size /= sizeof(unsigned);
	stack = new unsigned[size];
	timeSlice = t;
	myThread = thread;
	PCB::allPCBs.put(this);

	stack[size - 3] = 0x200;

#ifndef BCC_BLOCK_IGNORE
	stack[size - 4] = FP_SEG(PCB::wrapper);
	stack[size - 5] = FP_OFF(PCB::wrapper);

	ss = FP_SEG(stack + size - 14);
	sp = FP_OFF(stack + size - 14);
	bp = sp;
#endif

	stack[size - 14] = bp;

}

PCB::~PCB() {
	allPCBs.remove(this);
	delete stack;
}

void PCB::wrapper() {
	PCB::running->myThread->run();

	PCB::running->status = PCB::DEAD;

	while(PCB::running->waitQueue.length() != 0) {
		PCB* temp = PCB::running->waitQueue.get();
		temp->status = PCB::READY;
		Scheduler::put(temp);
	}

	dispatch();
}

