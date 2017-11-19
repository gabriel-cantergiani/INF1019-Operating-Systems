#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<string.h>
#include "auxiliar.h"

#define MAX 50

int main (void) {
	
	char programa[6], parametros[20];	/*strings para guardar e separar comandos lidos*/
	char* ret;						/*ponteiro para separar cada rajada dos parametros*/
	int pidEsc;						/*process ID do escalonador*/
	int i, indProc=0;				/* índices de vetires*/
	char rajadas[5][3];					/*vetor contendo as rajadas do programa a ser criado*/

	int seg1, seg2, seg3, seg4, seg5, seg6, seg7;				/*variável para guardar ID de áreas de memória compartilhada*/
	char *prog_SHM;
	char *r1, *r2, *r3, *r4, *r5;
	int *novoP;						/**/

/***********   Alocação de memória compartilhada *****************************************************************************************/

	seg1 = shmget(2232, sizeof(char *), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	seg2 = shmget(2233, sizeof(char *), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	seg3 = shmget(2234, sizeof(char *), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	seg4 = shmget(2235, sizeof(char *), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	seg5 = shmget(2236, sizeof(char *), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	seg6 = shmget(2237, sizeof(char *), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
	seg7 = shmget(2238, sizeof(int), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);


	prog_SHM = (char *) shmat(seg1, 0, 0);
	r1 = (char *) shmat(seg2, 0, 0);
	r2 = (char *) shmat(seg3, 0, 0);
	r3 = (char *) shmat(seg4, 0, 0);
	r4 = (char *) shmat(seg5, 0, 0);
	r5 = (char *) shmat(seg6, 0, 0);
	novoP = (int *) shmat(seg7, 0, 0);
	*novoP = 0;


/***************   Criação do processo filho Escalonador *********************************************************************************/


/**************	 Loop de leitura de comandos  ************************************************************************/

	printf("Comandos aceitos no formato:  exec <programa> (<rajada1>, <rajada2>,..., <rajada5>) \n\n\n");

	printf(">>> ");
	while ( scanf(" exec %s (%[^)])", programa, parametros) == 2 ){
	
		ret = strtok(parametros, "(, ");
		for(i=0;i<5;i++){
			if(ret!=NULL)
				strcpy(rajadas[i], ret);
			else
				strcpy(rajadas[i], "0");

			ret = strtok(NULL, ", ");
		}
		
		/************  Comunicação com Processo escalonador   ****************************/
		strcpy(prog_SHM, programa);
		strcpy(r1, rajadas[0]);
		strcpy(r2, rajadas[1]);
		strcpy(r3, rajadas[2]);
		strcpy(r4, rajadas[3]);
		strcpy(r5, rajadas[4]);
		
		*novoP = *novoP + 1;
		
		sleep(1);
		printf(">>> ");
	}
	
	
	*novoP = -1;

	shmdt(prog_SHM);
	shmdt(r1);
	shmdt(r2);
	shmdt(r3);
	shmdt(r4);
	shmdt(r5);
	shmdt(novoP);

	shmctl(seg1, IPC_RMID, 0);
	shmctl(seg2, IPC_RMID, 0);
	shmctl(seg3, IPC_RMID, 0);
	shmctl(seg4, IPC_RMID, 0);
	shmctl(seg5, IPC_RMID, 0);
	shmctl(seg6, IPC_RMID, 0);
	shmctl(seg7, IPC_RMID, 0);

	printf("Interpretador finalizado. \n");

	return 0;

}
