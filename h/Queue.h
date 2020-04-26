#ifndef LIST_H_
#define LIST_H_


class Thread;
class PCB;
class KernelSem;

typedef int ID;

class Queue {
public:
	struct Elem {
		PCB *pcb;
		Elem *next;

		Elem(PCB * p) {
			pcb = p;
			next = 0;
		}
	};

	Elem *first, *last;

	Queue();
	~Queue();
	void put(PCB *p);
	PCB* get();
	int length();
	void remove(PCB *p);
	int free(int n);
	Thread* getThread(ID id);
	void timePassed(KernelSem*);

	int len;

};

#endif
