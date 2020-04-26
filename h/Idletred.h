/*
 * Idletred.h
 *
 *  Created on: Aug 24, 2019
 *      Author: OS1
 */

#ifndef IDLETRED_H_
#define IDLETRED_H_
#include "Thread.h"
#include "PCB.h"

class Idletred : public Thread {
public:
	Idletred(): Thread(256, 1) { }
	virtual ~Idletred();
	virtual void run();
	void start();
};

#endif /* IDLETRED_H_ */
