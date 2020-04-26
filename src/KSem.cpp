/*
 * KSem.cpp
 *
 *  Created on: Aug 25, 2019
 *      Author: OS1
 */

#include "KSem.h"

SemList KernelSem::allSems;


KernelSem::KernelSem(int init) {
	val = init;
	allSems.put(this);
}

KernelSem::~KernelSem() {
	valQueue.free(valQueue.length());
	timeQueue.free(timeQueue.length());
	allSems.remove(this);
}


int KernelSem::wait(Time maxTimeToWait)
{
	lock();
	if(--val < 0) {
		PCB::running->status = PCB::BLOCKED;
		if (maxTimeToWait > 0) {						//ako je vreme vece od nule
			PCB::running->waitTime = maxTimeToWait;			//dodeli joj vreme cekanja
			timeQueue.put((PCB*)PCB::running);				//ubaci je u red niti koje cekaju zbog wait(n)
		}
		if (maxTimeToWait == 0) {
			valQueue.put((PCB*)PCB::running);			//i stavlja u red onih koje cekaju da se promeni value semafora
		}
		unlock();
		dispatch();
		lock();
		int ret=PCB::running->timeFlag;
		PCB::running->timeFlag=1;
		unlock();
		return ret;
	}
	unlock();
	return 1;
}


int KernelSem::signal(int n) {
	lock();
	if(n < 0) {
			unlock();
			return n;
		}
	if(val <= 0) {
		if (n > 0) {											//ako je n vece od 0
			val += n;												//uvecaj vrednost semafora za n
			int freed = valQueue.free(n);							//oslobadjamo niti iz reda onih koje cekaju zbog val i vracamo broj oslobodjenih
			int left = n - freed;									//broj preostalih niti za oslobadjanje
			freed += timeQueue.free(left);							//oslobodi niti iz reda onih koje cekaju zbog vremena
			unlock();
			return freed;											//vrati ukupan broj oslobodjenih niti
		}
		if (n == 0) {
			val++;
			if (valQueue.length() == 0) timeQueue.free(1);
			else valQueue.free(1);
			unlock();
			return 1;
		}
	}
	if(n == 0) val++;
	else val += n;
	unlock();
	return 0;
}


int KernelSem::value() {
	return val;
}
