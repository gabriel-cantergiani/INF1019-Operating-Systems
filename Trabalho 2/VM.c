#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "VM.h"

#define PageSize 0x10000
#define qtdPageFrames 256
#define tamMemFisica PageSize * qtdPageFrames
#define tamPageTable 0x10000
#define tamMemVirtual PageSize * tamPageTable

/*************** Estruturas de Dados ***************************/

typedef struct Page {

	unsigned short int Pno;	/*Número do processo a qual a página pertence*/

	unsigned short int R;		/*Bit de Referenciado*/

	unsigned short int M;		/* Bit de Modificado*/

	unsigned short int Frequency;	/* Contador do número de acessos à pagina*/

	short int Fnumber;	    /* Número da moldura de página (índice no vetor da mem Física). Se == -1, pagina não está na memoria*/

} tpPage;


typedef struct PageTable {

	unsigned short int Pno;	/* Numero do processo à qual a Tabela de Paginas pertence*/

	tpPage vtPages [ tamPageTable ] ;	/* Vetor com as páginas da tabela*/

} tpPageTable;


/******** Variáveis globais ***********/

tpPageTable * PageTable_p1;
tpPageTable * PageTable_p2;
tpPageTable * PageTable_p3;
tpPageTable * PageTable_p4;

/*********************** Funções / Procedimentos exportados pelo Módulo ******************/

/* Função criaTodasPageTables, aloca previamente em memoria compartilhada todas as PageTables a ser usadas nos 4 processos */

void createAllPageTables (void) {

	int seg1, seg2, seg3, seg4;

	printf("Vai começar a criar SHMEM\n");

	seg1 = shmget(1121, sizeof(tpPageTable), IPC_CREAT  | S_IRUSR | S_IWUSR  );
//	seg2 = shmget(2222, sizeof(tpPageTable), IPC_CREAT  | S_IRUSR | S_IWUSR  );
//	seg3 = shmget(3333, sizeof(tpPageTable), IPC_CREAT  | S_IRUSR | S_IWUSR  );
//	seg4 = shmget(4444, sizeof(tpPageTable), IPC_CREAT  | S_IRUSR | S_IWUSR  );

	if (seg1 == -1 || seg2 == -1 || seg3 == -1 || seg4 == -1){
		fprintf(stderr,"Erro na alocação de memoria compartilhada\n");
		exit(1);
	}

	printf("Criou SHMEM. Vai atribuir às Page Tables\n");

	PageTable_p1 = (tpPageTable *) shmat (seg1, 0, 0);
//	PageTable_p2 = (tpPageTable *) shmat (seg2, 0, 0);
//	PageTable_p3 = (tpPageTable *) shmat (seg3, 0, 0);
//	PageTable_p4 = (tpPageTable *) shmat (seg4, 0, 0);

	if (PageTable_p1 == (void *) -1 || PageTable_p2 == (void *) -1 || PageTable_p4 == (void *) -1 || PageTable_p4 == (void *) -1) {
		fprintf(stderr, "Erro na conexão com memoria compartilhada\n");
		exit(1);
	}

	printf("Terminou de criar todas as Page Tables.\n");

	return;
}

/* Função removeAllPageTables(), desaloca e remove memoria compartilhada */




/* Função inicializaPageTable(), inicializa uma Tabela de Paginas para o processo */

tpPageTable * initPageTable (int Pno) {

	tpPageTable * PageTable ;		/*tabela de paginas criada */

	int seg, i;	/* identificador da memoria compartilhada*/

	printf("Vai começar à atribuir Page Table à P%d\n", Pno);

	switch (Pno) {
		case 1: {
					PageTable = PageTable_p1;
					/*Inicializa tabela de paginas*/

					for(i = 0; i < tamPageTable ; i++) {
						
						PageTable->vtPages[i].Pno = 1;
						PageTable->vtPages[i].R = 0;
						PageTable->vtPages[i].M = 0;
						PageTable->vtPages[i].Frequency = 0;
						PageTable->vtPages[i].Fnumber = -1;
						
					}

					break;
				}
		case 2: {
					PageTable = PageTable_p2;
					/*Inicializa tabela de paginas*/
					
					for(i = 0; i < tamPageTable; i++) {
						PageTable->vtPages[i].Pno = 2;
						PageTable->vtPages[i].R = 0;
						PageTable->vtPages[i].M = 0;
						PageTable->vtPages[i].Frequency = 0;
						PageTable->vtPages[i].Fnumber = -1;
					}

					break;
				}
		case 3: {
					PageTable = PageTable_p3;
					
					/*Inicializa tabela de paginas*/
					
					for(i = 0; i < tamPageTable; i++) {
						PageTable->vtPages[i].Pno = 3;
						PageTable->vtPages[i].R = 0;
						PageTable->vtPages[i].M = 0;
						PageTable->vtPages[i].Frequency = 0;
						PageTable->vtPages[i].Fnumber = -1;
					}

					break;
				}
		case 4: {
					PageTable = PageTable_p4;

					/*Inicializa tabela de paginas*/
					
					for(i = 0; i < tamPageTable; i++) {
						PageTable->vtPages[i].Pno = 4;
						PageTable->vtPages[i].R = 0;
						PageTable->vtPages[i].M = 0;
						PageTable->vtPages[i].Frequency = 0;
						PageTable->vtPages[i].Fnumber = -1;
					}

					break;
				}
		default: {
					fprintf(stderr, "Numero de Processo inválido. Não foi possível criar PageTable.\n");
					exit(1);
				}

	}

	fprintf(stdout, "PageTable do Processo %d criada com sucesso!\n", Pno);
	return PageTable;

}

/* Procedimento trans() - traduz endereço virtual*/

void trans (int Pno, unsigned PageInx, unsigned offset, char rw) {

	printf("Começando a traduzir o endereço virtual. PageInx = %x e Offset = %x\n", PageInx, offset);
	
	switch(Pno) {

		/* Processo 1  -> Page Table P1 */
		case 1: {							/* PAGE FAULT*/
					if( PageTable_p1->vtPages[PageInx].Fnumber < 0 ) {
							kill( getppid(), SIGUSR1 );
					}
					printf("P%d, F-%d + %x, %c\n", Pno, PageTable_p1->vtPages[PageInx].Fnumber, offset, rw);
				}




	}


	return;
}


int createSharedIndex ( int Pno, int *Index) {

	int segId;

	switch(Pno) {

			case 1: {	segId = shmget (1500, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
						if (segId1 == -1){
							fprintf(stderr,"Erro na alocação do Index compartilhado do Processo %d\n", Pno);
							exit(1);
						}
					}
			case 2: {	segId = shmget (2500, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
						if (segId == -1){
							fprintf(stderr,"Erro na alocação do Index compartilhado do Processo %d\n", Pno);
							exit(1);
						}
					}
			case 3: {	segId = shmget (3500, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
						if (segId == -1){
							fprintf(stderr,"Erro na alocação do Index compartilhado do Processo %d\n", Pno);
							exit(1);
						}
					}
			case 4: {	segId = shmget (4500, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
						if (segId == -1){
							fprintf(stderr,"Erro na alocação do Index compartilhado do Processo %d\n", Pno);
							exit(1);
						}
					}
	}

	Index = (int *) shmat (segment1, 0, 0);

	return segId;

}


void removeSharedIndex ( int segId, int *Index) {

	shmdt(Index);
	shmctl(segId, IPC_RMID, 0);
	return;
}



int createPageTable (int Pno, tpPageTable * PageTable) {

	int seg;

	switch (Pno) {

		case 1: {
					seg = shmget( 1121, sizeof(tpPageTable), IPC_CREAT | S_IRUSR | S_IWUSR  );
					if (seg == -1){
						fprintf(stderr,"Erro na alocação da PageTable compartilhada do Processo %d\n", Pno);
						exit(1);
					}
					PageTable = (tpPageTable *) shmat (seg, 0, 0);
					if (PageTable == (void *) -1) {
						fprintf(stderr, "Erro na conexão com memoria compartilhada\n");
						exit(1);
					}
					for(i = 0; i < tamPageTable ; i++) {
						PageTable->vtPages[i].Pno = 1;
						PageTable->vtPages[i].R = 0;
						PageTable->vtPages[i].M = 0;
						PageTable->vtPages[i].Frequency = 0;
						PageTable->vtPages[i].Fnumber = -1;
					}

					PageTable_p1 = PageTable;
					break;
				}

		default: {
					fprintf(stderr, "Numero de Processo inválido. Não foi possível criar PageTable.\n");
					exit(1);
				}
	}

	printf("Page Table do Processo %d criada com sucesso\n", Pno);
	return seg;
}