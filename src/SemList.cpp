/*
 * SemList.cpp
 *
 *  Created on: Aug 25, 2019
 *      Author: OS1
 */

#include "SemList.h"
#include "KSem.h"

SemList::SemList() {
	first = last = 0;
	len = 0;
}

SemList::~SemList() {
	while (first != 0) {
		Elem *old = first;
		first = first->next;
		delete old;
	}
	len = 0;
}

void SemList::put(KernelSem* s) {
	if (first == 0) {
		first = last = new Elem(s);
	}
	else {
		last->next = new Elem(s);
		last = last->next;
	}
	len++;
}

KernelSem* SemList::get() {

	if (first == 0) return 0;
	Elem* curr = first;
	KernelSem* s = curr->ks;
	if (first->next == 0) first = last = 0;
	else first = first->next;
	len--;
	delete curr;
	return s;
}

int SemList::length() {
	return len;
}

void SemList::remove(KernelSem* s) {
	Elem *curr = first, *prev = 0;
	while(curr)
		if (curr->ks != s) {
			prev = curr; curr = curr->next;
		}
		else {
			Elem* old = curr;
			curr = curr->next;
			if (!prev) first = curr;
			else prev->next = curr;
			delete old;
			len--;
		}
}

void SemList::checkSems()
{
	if(first == 0) return;
	for (Elem* curr = first; curr != 0; curr = curr->next) {
		curr->ks->timeQueue.timePassed(curr->ks);
	}
}
