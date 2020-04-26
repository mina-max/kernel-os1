#ifndef _event_h_
#define _event_h_
#include "Kernelev.h"
#include "Ivtentry.h"


typedef unsigned char IVTNo;


class Event
{
	friend class IVTEntry;
public:

	Event(IVTNo ivtNo);
	~Event();
	void wait();

protected:

	friend class KernelEv;
	void signal(); // can call KernelEv

private:

	KernelEv* myImpl;

};
#endif
