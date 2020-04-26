#include "System.h"
#include <stdio.h>

extern int userMain(int argc, char* argv[]);

int main(int argc, char** argv) {
	System::inic();
	userMain(argc,argv);
	System::restore();
}
