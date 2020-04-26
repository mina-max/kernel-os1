#include "Ivtentry.h"

#include <dos.h>

IVTEntry* IVTEntry::IVT[256] = {0};

IVTEntry::IVTEntry(IVTNo entryNo, pInterrupt newRoutine)
{
	entry = entryNo;
	kEvent = 0;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(entry);
	setvect(entry, newRoutine);
	setvect(0x61, oldRoutine);
#endif
	IVT[entry] = this;

}

IVTEntry::~IVTEntry()
{
#ifndef BCC_BLOCK_IGNORE
	setvect(entry, oldRoutine);
#endif
	IVT[entry] = 0;
	callOldInterrupt();
}

void IVTEntry::callOldInterrupt() {
	asm int 61h;
}

void IVTEntry::signal()
{
	if(kEvent){
		kEvent->signal();
	}
}

void IVTEntry::setKEv(KernelEv * k)
{
	kEvent = k;
}
