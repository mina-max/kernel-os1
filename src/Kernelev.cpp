#include "Kernelev.h"
#include "Ivtentry.h"


KernelEv::KernelEv(IVTNo ivtNo)
{
	ivtEntry = ivtNo;
	owner = (PCB*)PCB::running;
	val = 1;
	IVTEntry* ent = IVTEntry::IVT[ivtNo];
	ent->setKEv(this);
}


KernelEv::~KernelEv()
{
	signal();
	IVTEntry::IVT[ivtEntry]->kEvent = 0;
}

void KernelEv::wait()
{
	if ((PCB*)PCB::running != owner) { return; }
	if(val == 1){
		val = 0;
		PCB::running->status = PCB::BLOCKED;
		dispatch();
	} else val = 0;
}
	
void KernelEv::signal()							
{
	if(owner->status == PCB::BLOCKED) {
		val = 1;
		owner->status = PCB::READY;
		Scheduler::put(owner);
		dispatch();
	} else val = 0;
}
