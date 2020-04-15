int createPageTable (int Pno, tpPageTable * PageTable) {

	int seg, i;

	switch (Pno) {		/* Cria shared memory de acordo com processo em questao*/

		case 1: {
					seg = shmget( 1000, sizeof(tpPageTable), IPC_CREAT | S_IRUSR | S_IWUSR  );	/* aloca memoria compartilhada*/
					PageTable = (tpPageTable *) shmat (seg, 0, 0);								/* conectada Page table com a memoria alocada*/
					PageTable_p1 = PageTable;													/* Atualiza ponteiro local */
					break;
				}
		case 2: {
					seg = shmget( 2000, sizeof(tpPageTable), IPC_CREAT | S_IRUSR | S_IWUSR  );
					PageTable = (tpPageTable *) shmat (seg, 0, 0);
					PageTable_p2 = PageTable;
					break;
				}
		case 3: {
					seg = shmget( 3000, sizeof(tpPageTable), IPC_CREAT | S_IRUSR | S_IWUSR  );
					PageTable = (tpPageTable *) shmat (seg, 0, 0);
					PageTable_p3 = PageTable;
					break;
				}
		case 4: {
					seg = shmget( 4000, sizeof(tpPageTable), IPC_CREAT | S_IRUSR | S_IWUSR  );
					PageTable = (tpPageTable *) shmat (seg, 0, 0);
					PageTable_p4 = PageTable;
					break;
				}

		default: {
					fprintf(stderr, "Numero de Processo inválido. Não foi possível criar PageTable.\n");
					exit(1);
				}
	}

	if (seg == -1){
			fprintf(stderr,"Erro na alocação da PageTable compartilhada do Processo %d\n", Pno);
			exit(1);
	}

	if (PageTable == (void *) -1) {
		fprintf(stderr, "Erro na conexão com memoria compartilhada\n");
		exit(1);
	}

	/* Inicializa todas as paginas da PageTable*/
	for(i = 0; i < tamPageTable ; i++) {
		PageTable->vtPages[i].Pno = Pno;
		PageTable->vtPages[i].R = 0;
		PageTable->vtPages[i].M = 0;
		PageTable->vtPages[i].Frequency = 0;
		PageTable->vtPages[i].Fnumber = -1;
	}

	printf("Page Table do Processo %d criada com sucesso\n", Pno);
	return seg;
}


void removePageTable (int segId, tpPageTable * PageTable) {

	shmdt(PageTable);

	shmctl(segId, IPC_RMID, 0);

	return;
}