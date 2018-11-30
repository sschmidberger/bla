#include "personen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
	if (argc < 2){
		fputs ("usage: programm <personenfile>\n", stderr);
	    exit (1);
	}
	PersonList *pl = newPersonList ();
	readPersonList(pl, argv[1]);
	getMiddleChild(pl);

	return 0;
}
