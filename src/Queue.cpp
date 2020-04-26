#include "Queue.h"
#include "System.h"
#include "SCHEDULE.H"
#include <stdio.h>
#include "KSem.h"


Queue::Queue() {
	first = last = 0;
	len = 0;
}

Queue::~Queue() {
	while (first != 0) {
		Elem* old = first;
		first = first->next;
		delete old;
	}
	len = 0;
}

void Queue::put(PCB* pcb) {						//dodajemo nit na kraj
	if (first == 0) {
		first = last = new Elem(pcb);
	}
	else {
		last->next = new Elem(pcb);
		last = last->next;
	}
	len++;
}

PCB * Queue::get() {								//uzimamo nit sa pocetka
	if (first == 0) return 0;
	Elem* curr = first;
	PCB* p = curr->pcb;
	if (first->next == 0) first = last = 0;
	else first = first->next;
	len--;
	delete curr;
	return p;
}

void Queue::timePassed(KernelSem* ks) {					//svaki put kad okine timer izvrsavace se ova metoda
	Elem *curr = first, *prev = 0;
	while (curr) {
		curr->pcb->waitTime--;						//smanjuje svakoj niti u redu vreme cekanja za 1
		if (curr->pcb->waitTime != 0) {				//ako i dalje nije nula ne radi nista, predji na sledecu nit
			prev = curr; curr = curr->next;
		}
		else {		
			ks->val++;							//ako je vreme doslo do nule znaci da treba da se oslobodi
			Elem* old = curr;
			old->pcb->timeFlag = 0;
			old->pcb->status = PCB::READY;			//stavljamo status na ready
			Scheduler::put(old->pcb);				//stavljamo u scheduler
			curr = curr->next;
			if (!prev) first = curr;
			else prev->next = curr;
			delete old;								//brisemo iz reda
			len--;
		}
	}
}

int Queue::length() {
	return len;
}


int Queue::free(int n) {					//metoda za oslobadjanje n niti iz reda
	//lock();
	if (n > len) n = len;						//ako je n vece od broja niti, n postaje broj niti
	int counter = n;
	while (counter--) {
		PCB* temp = this->get();
		temp->timeFlag = 1;							//uzimamo prvu nit iz reda (onu koja je najduze cekala)
		temp->status = PCB::READY;					//postavljamo status da je spremna
		Scheduler::put(temp);						//stavljamo u scheduler
	}
	//unlock();
	return n;										//vracamo broj deblokiranih niti
}

Thread* Queue::getThread(ID id) {
	lock();
	for(Elem* curr = first; curr != 0; curr = curr->next)
		if(curr->pcb->myThread->getId() == id) {
			unlock();
			return curr->pcb->myThread;
		}
	unlock();
	return 0;
}

void Queue::remove(PCB* p) {
	//lock();
		Elem *curr = first, *prev = 0;
		while(curr)
			if (curr->pcb != p) {
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
	//unlock();
}



/*
void Queue::write() {
	lock();
	for(Elem* curr = first; curr != 0; curr = curr->next)
		printf("%d  ", curr->pcb->myThread->getId());
	unlock();

}*/


