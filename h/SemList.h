/*
 * SemList.h
 *
 *  Created on: Aug 25, 2019
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_

class KernelSem;

class SemList {
public:
	struct Elem {
			KernelSem* ks;
			Elem *next;

			Elem(KernelSem* s) {
				ks = s;
				next = 0;
			}
		};
		Elem *first, *last;
		int len;

		SemList();
		~SemList();

		void put(KernelSem *s);
		KernelSem* get();
		int length();
		void remove(KernelSem *s);
		void checkSems();
};

#endif /* SEMLIST_H_ */
