/*
 * Semaphor.cpp
 *
 *  Created on: Aug 25, 2019
 *      Author: OS1
 */

#include "Semaphor.h"

Semaphore::Semaphore(int init) {
	myImpl = new KernelSem(init);
}

Semaphore::~Semaphore() {
	delete myImpl;
}

int Semaphore::wait(Time t) {
	return myImpl->wait(t);
}

int Semaphore::signal(int n) {
	return myImpl->signal(n);
}

int Semaphore::val() const {
	return myImpl->value();
}
