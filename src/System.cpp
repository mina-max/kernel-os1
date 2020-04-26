/*
 * System.cpp
 *
 *  Created on: Aug 24, 2019
 *      Author: OS1
 */

#include "System.h"
#include <dos.h>
#include <stdio.h>
#include "KSem.h"


volatile int lockFlag = 0;
int System::requested_context_change = 0;
int System::counter = 0;
Thread* System::starting = 0;
Idletred* System::idle = 0;
pInterrupt oldRoutine;
unsigned int tsp = 0;
unsigned int tss = 0;
unsigned int tbp = 0;

void System::inic() {
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(0x08);
	setvect(0x08, timer);
	setvect(0x60, oldRoutine);
#endif

	System::starting = new Thread();
	System::starting->myPCB->status = PCB::READY;
	System::counter = System::starting->myPCB->timeSlice;
	PCB::running = System::starting->myPCB;

	System::idle = new Idletred();
	System::idle->start();
}

void System::restore() {
#ifndef BCC_BLOCK_IGNORE
	setvect(0x08, oldRoutine);
#endif
	delete System::idle;
	delete System::starting;
}

void interrupt timer(...) {

	if(!System::requested_context_change) {
		System::counter--;
		tick();
		KernelSem::allSems.checkSems();
		asm int 60h;
	}

	if(System::counter == 0 || System::requested_context_change == 1) {
		if(lockFlag == 0) {
			System::requested_context_change = 0;

			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			//printf("Kontekst niti je %d\n", Thread::getRunningId());

			if(PCB::running->status == PCB::READY && PCB::running->myThread != System::idle)
				Scheduler::put((PCB*)PCB::running);

			PCB::running = Scheduler::get();

			if (PCB::running == 0) PCB::running = System::idle->myPCB;

			//printf("Promenjen kontekst niti na %d\n", Thread::getRunningId());

			System::counter = PCB::running->timeSlice;

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			asm {
				mov sp, tsp   // restore sp
				mov ss, tss
				mov bp, tbp
			}

		}
		else System::requested_context_change = 1;
	}


}



