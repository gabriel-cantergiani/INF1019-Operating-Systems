#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>

#include "auxiliar.h"

/******************************************************************** Declaração de variáveis globais ao módulo ***************************************************************************************************/

static int seg1, seg2, seg3, seg4, seg5, seg6, seg7, *novoP;	/*variáveis para guardar ID de áreas de memória compartilhada*/
static char *prog_SHM, *r1, *r2, *r3, *r4, *r5;				/* variáveis para guardar ponteiro para areas de memoria compartilhada */

static pid_t pid[50]; 										/* variável para guardar IDs dos processos criados */
static int indFila=0, indProcesso=0, numProcessos=0, filaCorrente=0;
static int QUANTUM[3] = {1, 2, 4}, termina=0;
Processo *pCorrente;								
Fila * fila[3];


/* Funções auxiliares e de tratamento de sinais *****************************************************************************/

void trata_novo_processo (void); 	//Função de criação de novo processo.

void trata_sinal_IO (int sinal);	 // Função de bloqueio de processo por I/O. Ativada com sinal SIGUSR2

void elimina_processo (int sinal);	 // Função que elimina processo que terminou execução

void termina_tudo(void);		// Função que termina programa


/*************************************************************************************************************

	FUNÇÃO MAIN

**************************************************************************************************************/

int main (void) {
	
	
/**********   Criação das filas de prioridade *********************************/

	if(fila[0]==NULL)
		fila[0] = criarFila();
	fila[1] = criarFila();
	fila[2] = criarFila();

/***********   Alocação de memória compartilhada ********************************/
	
	seg1 = shmget(2232, sizeof(char *), IPC_CREAT | S_IRUSR | S_IWUSR);
	seg2 = shmget(2233, sizeof(char *), IPC_CREAT | S_IRUSR | S_IWUSR);
	seg3 = shmget(2234, sizeof(char *), IPC_CREAT | S_IRUSR | S_IWUSR);
	seg4 = shmget(2235, sizeof(char *), IPC_CREAT | S_IRUSR | S_IWUSR);
	seg5 = shmget(2236, sizeof(char *), IPC_CREAT | S_IRUSR | S_IWUSR);
	seg6 = shmget(2237, sizeof(char *), IPC_CREAT | S_IRUSR | S_IWUSR);
	seg7 = shmget(2238, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);

	prog_SHM = (char *) shmat(seg1, 0, 0);
	r1 = (char *) shmat(seg2, 0, 0);
	r2 = (char *) shmat(seg3, 0, 0);
	r3 = (char *) shmat(seg4, 0, 0);
	r4 = (char *) shmat(seg5, 0, 0);
	r5 = (char *) shmat(seg6, 0, 0);
	novoP = (int *) shmat(seg7, 0, 0);
	*novoP = 0;

/* ********** Define tratamento de sinais ***************************************/

	signal(SIGUSR2, trata_sinal_IO);
	signal(SIGUSR1, elimina_processo);



/* *******************************************************************************/
	printf("Escalonador em execução...\n");
	
	while(termina==0) {	
		/* executa infinitamente, até receber sinal de termino*/
		if((*novoP)>0){ 
//			printf("Achou novo processo\n");
			trata_novo_processo();
		}	
		/*testa se número de processos é maior que 0*/
		if (numProcessos>0 && ((fila[filaCorrente]->num) > 0)) {
			/*pega o primeiro da fila 1*/
			pCorrente = obterProximoProcesso (fila[filaCorrente]);

			/*executa o processo*/

			kill(pCorrente->pid, SIGCONT);
			/*espera o quantum de tempo da fila*/
			sleep(QUANTUM[filaCorrente]);
			/* para o processo */
			if(pCorrente!=NULL){

				kill(pCorrente->pid, SIGSTOP);
			}
	
			/*checa se processo terminou antes do tempo  */
			if(pCorrente!=NULL && pCorrente->terminado==1)
			{								/*se sim, elimina processo*/	
					excluirProcesso(pCorrente);
					numProcessos--;
			}
			else if (pCorrente!=NULL)							/*se não, (verifica flag de processo em IO)*/
			{
				
				if(pCorrente->flagIO==1){

					pCorrente->flagIO=0;	
					/* coloca processo no fim da fila de cima (ou no fim da mesma fila, se tiver na Fila 1)*/
					if(filaCorrente==0 || filaCorrente==1){

						incluirProcesso(fila[0], pCorrente);
					}
					else if(filaCorrente==2){

						incluirProcesso(fila[1], pCorrente);
					}
																										
				}																						
				else if(pCorrente->flagIO==0) {

							/* coloca processo no fim da fila de baixo*/

					if(filaCorrente==1 || filaCorrente==2){
		
						incluirProcesso(fila[2], pCorrente);
					}
					else if (filaCorrente==0){

						incluirProcesso(fila[1], pCorrente);
					}

				}																				
								
			}
															

		}
			/* se uma fila de maior prioridade esta vazia, troca a fila */

		if (fila[0]->num>0)
			filaCorrente = 0;
		else if(fila[1]->num>0)
			filaCorrente = 1;
		else
			filaCorrente = 2;


		if((*novoP)==-1)		/*Testa se interpretador mandou parar*/
			termina_tudo();

		
	}

/***************** Desconecta e libera área de memória compartilhada *****************************/

	shmdt(prog_SHM);
	shmdt(r1);
	shmdt(r2);
	shmdt(r3);
	shmdt(r4);
	shmdt(r5);

	shmctl(seg1, IPC_RMID, 0);
	shmctl(seg2, IPC_RMID, 0);
	shmctl(seg3, IPC_RMID, 0);
	shmctl(seg4, IPC_RMID, 0);
	shmctl(seg5, IPC_RMID, 0);
	shmctl(seg6, IPC_RMID, 0);

	printf("Escalonador terminado...");

	return 0;

}

/******************************************

	Função trata_novo_processo

******************************************/


void trata_novo_processo (void) {

	Processo *p;
	char *argv[7];
	char  path[] = "progs/";

	if ( (pid[indProcesso] = fork())== 0 ) {	/* filho */

			argv[0] = (char *) malloc(sizeof(prog_SHM));	strcpy(argv[0], path);		strcat(argv[0], prog_SHM);  
			argv[1] = (char *) malloc(sizeof(r1));			strcpy(argv[1], r1);
			argv[2] = (char *) malloc(sizeof(r2));			strcpy(argv[2], r2);
			argv[3] = (char *) malloc(sizeof(r3));			strcpy(argv[3], r3);
			argv[4] = (char *) malloc(sizeof(r4));			strcpy(argv[4], r4);
			argv[5] = (char *) malloc(sizeof(r5));			strcpy(argv[5], r5);
			argv[6] = NULL;

			execv(argv[0], argv);
	} 
	else{	/* pai */ 
	kill(pid[indProcesso], SIGSTOP);

	p  = criarProcesso (pid[indProcesso], prog_SHM, *r1, *r2, *r3, *r4, *r5);	
	indProcesso++;

	if(fila[0]==NULL)
		fila[0] = criarFila();
	incluirProcesso(fila[0], p);
	filaCorrente=0;
	numProcessos++;
	*novoP = *novoP - 1;

	}
	
}

/******************************************

	Função trata_sinal_IO

******************************************/


void trata_sinal_IO (int sinal) {

	/* liga flag (do processo) dizendo que processo está em IO */
	kill(pCorrente->pid, SIGSTOP);
	sleep(3);
	pCorrente->flagIO = 1;
	

}

/******************************************

	Função elimina_processo

******************************************/


void elimina_processo (int sinal) {

	/*Mata processo*/
	printf("Eliminando processo...\n");
	kill(pCorrente->pid, SIGKILL);
	excluirProcesso(pCorrente);
	pCorrente = NULL;
	/*diminui numero de processos em 1*/
	numProcessos--;
	

}

/******************************************

	Função termina_tudo

******************************************/


void termina_tudo (void) {


	destruirFila(fila[0]);
	destruirFila(fila[1]);
	destruirFila(fila[2]);
	termina=1;

	/*esvazia filas, poe numProcessos=0 e termina programa*/
}
