/*
 * System.h
 *
 *  Created on: Aug 24, 2019
 *      Author: OS1
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_
#include "PCB.h"
#include "Idletred.h"
#include <stdio.h>


typedef void interrupt (*pInterrupt)(...);
void interrupt timer(...);

extern volatile int lockFlag;
#define lock() {lockFlag = 1;}
#define unlock() {lockFlag = 0; if(System::requested_context_change == 1) dispatch();}

class System {
public:
	static Thread* starting;
	static Idletred* idle;

	static int requested_context_change;
	static int counter;

	static void inic();
	static void restore();
};

extern void tick();

#endif /* SYSTEM_H_ */
