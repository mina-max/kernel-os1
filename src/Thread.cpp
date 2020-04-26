/*
 * Thread.cpp
 *
 *  Created on: Aug 24, 2019
 *      Author: OS1
 */

#include "Thread.h"
#include "SCHEDULE.H"
#include "System.h"

int Thread::ID = 0;

Thread::Thread(StackSize stackSize, Time timeSlice) {
	id = ++ID;
	myPCB = new PCB(stackSize, timeSlice, this);
	myPCB->status = PCB::NEW;
}

Thread::~Thread() {
	waitToComplete();
	delete myPCB;
}

void Thread::start(){
	myPCB->status = PCB::READY;
	Scheduler::put(myPCB);
}

void Thread::waitToComplete() {
	lock();

	if (this->myPCB->status == PCB::DEAD || this->myPCB->status == PCB::NEW) { unlock(); return; }

	if(this->myPCB == (PCB*)PCB::running) {unlock(); return; }

	if(this->myPCB == System::starting->myPCB) {unlock(); return;}

	if(this->myPCB == System::idle->myPCB) {unlock(); return;}

	PCB::running->status = PCB::BLOCKED;

	//printf("Nit %d se blokira\n", PCB::running->myThread->getId());

	this->myPCB->waitQueue.put((PCB*)PCB::running);

	dispatch();

	unlock();

}

int Thread::getId() {
	return id;
}

int Thread::getRunningId() {
	return PCB::running->myThread->getId();
}

Thread* getThreadById(ID id) {
	return PCB::allPCBs.getThread(id);
}

void dispatch() {
	asm cli;
	System::requested_context_change = 1;
	timer();
	asm sti;
}

