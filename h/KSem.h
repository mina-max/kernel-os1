/*
 * KSem.h
 *
 *  Created on: Aug 25, 2019
 *      Author: OS1
 */

#ifndef KSEM_H_
#define KSEM_H_
#include "Queue.h"
#include "SemList.h"
#include "System.h"

typedef unsigned int Time;

class KernelSem {
public:
	int val;
	Queue timeQueue;
	Queue valQueue;

	KernelSem(int init = 1);
	virtual ~KernelSem();
	virtual int wait (Time maxTimeToWait);
	virtual int signal(int n=0);
	int value();

	static SemList allSems;

};

#endif /* KSEM_H_ */
