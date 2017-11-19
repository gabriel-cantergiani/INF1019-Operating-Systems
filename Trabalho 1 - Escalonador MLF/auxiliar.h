

typedef struct tprocesso {

	int pid;			/* id do processo*/

	char programa[16];	/* nome do programa a ser executado no processo*/

	int rajada[5];		/* vetor com duração de cada rajada (max de 5). Caso tenha menos de 5, o resto é NULL*/

	int flagIO;			/*flagIO==1 -> em IO, flagIO==0, pronto ou executando*/

	int terminado;		/*terminado=1 --> processo terminou*/

	struct tprocesso * prox;	/* próximo processo da fila*/

} Processo;


typedef struct tfila {

	Processo *prim ;	/* Proximo processo da fila a ser executado */

	Processo *final;	/* ultimo processo da fila de execução*/

	int num;

} Fila;


Fila * criarFila (void);

void destruirFila (Fila *pFila);

void incluirProcesso (Fila * pFila, Processo *proc);

Processo * criarProcesso (int pid, char* programa, int raj1, int raj2, int raj3, int raj4,int raj5);

Processo * obterProximoProcesso (Fila *pFila);

void excluirProcesso (Processo *proc);