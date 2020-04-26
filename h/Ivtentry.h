
#ifndef _ivtentry_h_
#define _ivtentry_h_


#include "Kernelev.h"

typedef unsigned char IVTNo;

typedef void interrupt (*pInterrupt)(...);

#define PREPAREENTRY(numEntry, callOld) \
	void interrupt inter##numEntry(...); \
	IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
	void interrupt inter##numEntry(...){ \
		if(callOld == 1){ \
			newEntry##numEntry.callOldInterrupt(); \
		} \
		newEntry##numEntry.signal(); \
	}



class IVTEntry {
	friend class KernelEv;

public:

	static IVTEntry* IVT[256];

	IVTEntry(IVTNo, pInterrupt);
	~IVTEntry();
	void signal();
	void callOldInterrupt();
	void setKEv(KernelEv* k);

private:

	pInterrupt oldRoutine;
	IVTNo entry;
	KernelEv* kEvent;

};

#endif


