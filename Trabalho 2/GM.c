#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "VM.h"


int main (void) {

	pid_t pid1, pid2, pid3, pid4;	/* pid dos processos filho*/
	int MemIsFull = 0;				/* Memoria fisica esta cheia ou não*/

	int * PageIndex1, *PageIndex2, *PageIndex3, *PageIndex4, segId1, segId2, segId3, segId4;

	tppPageTable PageTable_p1;

	/* Cria tabelas e indices compartilhados */

	createAllPageTables();

	PageTable_p1 = initPageTable(1);

	// segId1 = createSharedIndex (1, PageIndex1);
	// segId2 = createSharedIndex (2, PageIndex2);
	// segId3 = createSharedIndex (3, PageIndex3);
	// segId4 = createSharedIndex (4, PageIndex4);

	/* Cria processos filho*/

	if ( (pid1 = fork()) == 0 ) {
			printf("Processo 1 criado.\n");				/* Cria Processo 1*/
			execv("P1", NULL);
	}





}