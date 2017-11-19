#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>



int main (int argc, char * argv[]) {

	int i, rajada[5];
	pid_t pidPai = getppid(), pid = getpid();
	
	for(i=1;i<argc;i++){				/* Registra rajadas passadas por argumento em vetor de inteiros*/
		rajada[i-1] = atoi(argv[i]);
	}
	
	/* Início das rajadas1 ***********************/

	/*rajada 1*/

	for( i=0 ; i< rajada[0]; i++){
		printf("%d\n", pid);
		sleep(1);
	}

	
	if(rajada[1]>0){
		kill(pidPai, SIGUSR2);			/*bloqueando para I/O */
		raise(SIGSTOP);
	}
		

	/*rajada 2*/

	for( i=0 ; i< rajada[1]; i++){
		printf("%d\n", pid);
		sleep(1);
	}

	if(rajada[2]>0){
		kill(pidPai, SIGUSR2);	/*bloqueando para I/O */
		raise(SIGSTOP);
	}
	

	/*rajada 3*/

	for( i=0 ; i< rajada[2]; i++){
		printf("%d\n", pid);
		sleep(1);
	}

	if(rajada[3]>0){
		kill(pidPai, SIGUSR2);	/*bloqueando para I/O */
		raise(SIGSTOP);
	}
	

	/*rajada 4*/

	for( i=0 ; i< rajada[3]; i++){
		printf("%d\n", pid);
		sleep(1);
	}

	if(rajada[4]>0){
		kill(pidPai, SIGUSR2);	/*bloqueando para I/O */
		raise(SIGSTOP);
	}
	

	/*rajada 5*/

	for( i=0 ; i< rajada[4]; i++){
		printf("%d\n", pid);
		sleep(1);
	}

	printf("Programa 3 finalizado!\n");
	kill(pidPai, SIGUSR1);

	return 0;

}