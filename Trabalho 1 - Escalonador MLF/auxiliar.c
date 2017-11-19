
#include "auxiliar.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Fila * criarFila (void) {

	Fila * pFila;

	pFila = (Fila *) malloc (sizeof(Fila));

	pFila->prim = NULL;
	pFila->final = NULL;
	pFila->num = 0;

}

void destruirFila (Fila *pFila) {

	Processo *p, *prox;

	p = pFila->prim;

	while(p!=NULL) {

		prox = p->prox;
		free(p);
		p = prox;

	}

	free(pFila);

}

Processo * criarProcesso (int pid, char* programa, int raj1, int raj2, int raj3, int raj4,int raj5){

	Processo *novo;
	int i;

	novo = (Processo *) malloc(sizeof(Processo));

	novo->pid = pid;
	strcpy(novo->programa, programa);
	novo->rajada[0] = raj1;
	novo->rajada[1] = raj2;
	novo->rajada[2] = raj3;
	novo->rajada[3] = raj4;
	novo->rajada[4] = raj5;
	novo->flagIO = 0;
	novo->terminado = 0;
	novo->prox = NULL;

	return novo;
}

void incluirProcesso (Fila * pFila, Processo *proc) {

	if(pFila->final==NULL || pFila->prim==NULL){
		pFila->final=proc;
		pFila->prim=proc;
	}
	else {
		pFila->final->prox = proc;
		pFila->final = proc;
	}

	pFila->num++;

}

Processo * obterProximoProcesso (Fila *pFila) {

	if(pFila->prim == NULL)
		return NULL;

	Processo *p, *segundo;

	p = pFila->prim;

	segundo = pFila->prim->prox;
	pFila->prim = segundo;
	pFila->num = pFila->num-1;
	p->prox = NULL;
	return p;
}

void excluirProcesso (Processo *proc) {

	free (proc);

}