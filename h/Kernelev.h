#ifndef _kernelev_h_
#define _kernelev_h_
#include "Event.h"
#include "PCB.h"
#include "system.h"
#include "SCHEDULE.H"

typedef unsigned char IVTNo;

class KernelEv
{
	friend class Event;
	friend class IVTEntry;

public:

	KernelEv(IVTNo);
	~KernelEv();

	void wait();

protected:

	void signal();

private:

	PCB* owner;
	int val;
	IVTNo ivtEntry;

};

#endif

