/**********  Estruturas de Dados **********************/

typedef struct Page * tppPage;

typedef struct PageTable * tppPageTable;



/************    Funções / Procedimentos  ******************/


void createAllPageTables (void);


tppPageTable initPageTable (int Pno);


void trans (int Pno, unsigned PageInx, unsigned offset, char rw);


int createSharedIndex ( int Pno, int *Index);


void removeSharedIndex ( int segId, int *Index);