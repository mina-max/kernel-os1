#include "Event.h"



Event::Event(IVTNo ivtNo)
{
	asm {
		pushf
		cli
	}

	myImpl = new KernelEv(ivtNo);

	asm popf
}

Event::~Event()
{
	asm {
		pushf
		cli
	}
	delete myImpl;
	asm popf
}

void Event::wait()
{
	asm {
			pushf
			cli
		}
	myImpl->wait();
	asm popf
}

void Event::signal()
{

	asm {
		pushf
		cli
	}
	myImpl->signal();
	asm popf
}
