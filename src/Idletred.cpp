/*
 * Idletred.cpp
 *
 *  Created on: Aug 24, 2019
 *      Author: OS1
 */

#include "Idletred.h"

void Idletred::run() {
	while(1) {}
}

void Idletred::start() {
	myPCB->status = PCB::READY;
}

Idletred::~Idletred() {
	delete myPCB;
}

