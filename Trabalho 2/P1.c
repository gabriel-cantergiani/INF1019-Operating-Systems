#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "VM.h"

#define S 16
#define PageSize 0x10000 


int main (void)  {

	FILE * log;
	unsigned address;	/*endereço lido do arquivo log */
	unsigned pageInx;		/*indice de pagina         */
	unsigned offset;	/*offset dentro da pagina */
	char rw;					/*byte de Escrita/Leitura */
	int seg;					/*id de memória compartilhada*/
	tppPageTable PageTable;

	PageTable = initPageTable (1);

	log = fopen ("logs/compilador.log", "r");

	printf("Processo 1 iniciando leitura do arquivo compilador.log\n");

	while ( fscanf (log, "%x %c", &address, &rw) != EOF ) {

		pageInx = address >> S ;
		offset = address & 0x0000FFFF ;

		trans (1, pageInx, offset, rw);
	}

	printf("Processo 1 finalizou leitura do arquivo compilador.log\n");

	return 0;

}
