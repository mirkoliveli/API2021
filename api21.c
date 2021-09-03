#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

//#define DEBUG "input_2"
//#define DEBUG "generato.txt"
//#define DEBUG "50x50.txt"
#define DEBUG "8x20000.txt"
#define MAX 4294967295


static char *stdin_buffer;
static int stdin_buffer_size;



//-------------------------------------------   LIBRERIA    ----------------------------------------------------------//

typedef struct {
    unsigned long int nome;
    unsigned long int punteggio;
} nome_punteggio;

typedef struct {
    unsigned long int nome;
    unsigned long int peso;
    unsigned long int nodo_precedente;
    bool raggiunto;
    bool esaminato;
} nodo;

void print_classifica(nome_punteggio *classifica, unsigned long int n_elementi_da_stampare);

void copy_classifica(nome_punteggio *dacopiare, nome_punteggio *dovecopiare, unsigned long int startdacopiare,
                     unsigned long int startdovecopiare, unsigned long int quanticopiarne);

void MERGE(nome_punteggio *classifica, nome_punteggio *temp, unsigned long int start, unsigned long int centro,
           unsigned long int final);

void MERGESORT(nome_punteggio *classifica, nome_punteggio *temp, unsigned long int start, unsigned long int final);

void Acquisisci_Matrice(unsigned long int **matrice, int dimensione);

unsigned long int
CalcoloPunteggio(unsigned long int **matrice, unsigned long int dimensione, nodo *grafo,
                 nodo **priority_queue);

void Inizializza_Grafo(nodo *grafo, unsigned long int dimensione);


unsigned long int Find_Next(nodo **priority, unsigned long int size);

static inline void stdin_parserow(int size, unsigned long int *ptr);

static inline void stdin_loadrow();

static inline int stdin_getfch();

static inline void stdin_init(int length);

void Inserimento_Ordinato(nome_punteggio *classifica, unsigned long int numelem, nome_punteggio nuovo_elem);

void Inizializza_Classifica(nome_punteggio *classifica, unsigned long int elem_class);

//-----------------------------------------------------   MAIN    ----------------------------------------------------//

int main() {

#ifdef DEBUG
    FILE *input = freopen("/home/mirko/CLionProjects/API21/open_tests/"DEBUG, "r", stdin);
#endif


    int n_node;
    unsigned long int n_elementi_classifica;
    unsigned long int i = 0;
    unsigned long int peggiore = 0;
    unsigned long int contatoregrafi = 0;
    bool piena = false;

    if (scanf("%d %lu\n", &n_node, &n_elementi_classifica) != 2) {
        return -1;
    }

    stdin_init(n_node);
//    printf("%lu %lu\n", n_node, n_elementi_classifica);

    //Creo la classifica con n_elementi_classifica+1 elementi
    //L'elemento in più lo usero per aggiungere gli elementi partendo dalla coda
    nome_punteggio *classifica = (nome_punteggio *) malloc(
            (n_elementi_classifica + 1) * sizeof(nome_punteggio));

    Inizializza_Classifica(classifica, n_elementi_classifica + 1);

    //Alloco la classifica temporanea per riordinare quella presente
    nome_punteggio *temp = (nome_punteggio *) malloc((n_elementi_classifica + 1) * sizeof(nome_punteggio));


    //Creo la matrice di adiacenza contenente i costi delle transizioni
    unsigned long int **matr_costi = (unsigned long int **) malloc(n_node * sizeof(unsigned long int *));
    while (i < n_node) {
        matr_costi[i] = (unsigned long int *) malloc(n_node * sizeof(unsigned long int));
        i++;
    }


    //Alloco la memoria per il grafo. Grafo[] di nodi
    nodo *grafo = (nodo *) malloc(n_node * sizeof(nodo));

    //Alloco la priority queue
    nodo **priority_queue = (nodo **) malloc(n_node * sizeof(nodo *));


    while (1) {
        stdin_loadrow();
        int cmd = stdin_getfch();
//        printf("Command %c\n", cmd);
        switch (cmd) {
            case 'A':
//                printf("Identificato AggiungiGrafo\n");

                Inizializza_Grafo(grafo, n_node);
                Acquisisci_Matrice(matr_costi, n_node);


                //----------------DA VALUTARE QUANDO CLASSIFICA NON PIENA!!!!!!----------------//
                classifica[n_elementi_classifica].nome = contatoregrafi;
                classifica[n_elementi_classifica].punteggio = CalcoloPunteggio(matr_costi, n_node, grafo,
                                                                               priority_queue);

//                if (peggiore == 0) {
//                    peggiore = classifica[n_elementi_classifica].punteggio;
////                    Inserimento_Ordinato(classifica, n_elementi_classifica, classifica[n_elementi_classifica]);
//
//                }
                //QUESTA CONDIZIONE VERIFICA CHE IL MIO NUOVO PUNTEGGIO SIA MIGLIORE DEL PUNTEGGIO PEGGIORE IN CLASSIFICA
                //SE L'IF È SODDISFATTO, ALLORA IL NUOVO ELEMENTO DEVE ESSERE AGGIUNTO ALLA CLASSIFICA
                if (peggiore > classifica[n_elementi_classifica].punteggio || !piena) {
                    Inserimento_Ordinato(classifica, n_elementi_classifica, classifica[n_elementi_classifica]);
                    peggiore = classifica[n_elementi_classifica - 1].punteggio;
                }

                contatoregrafi++;
                if (contatoregrafi == n_elementi_classifica) {
                    piena = true;
                }
                break;


            case 'T':
//                printf("Identificato TopK\n");
                if (contatoregrafi > 0) {
                    if (contatoregrafi < n_elementi_classifica) {
                        print_classifica(classifica, contatoregrafi);
                    } else {
                        print_classifica(classifica, n_elementi_classifica);
                    }
                } else {
                    printf("\n");
                }
                break;

            case EOF:
//                printf("Identificato END\n");
                free(priority_queue);
                free(matr_costi);
                free(classifica);
                free(grafo);
                free(temp);

#ifdef DEBUG
                fclose(input);
#endif
                return 0;


            default:
                printf("Error in 'identifica comando @ %lu (%d)'\n", contatoregrafi, cmd);
                return -1;
        }
    }
}


void print_classifica(nome_punteggio *classifica, unsigned long int n_elementi_da_stampare) {

    unsigned long int i;

    //---------STAMPA DI DEBUG----------//
    /*
           for (i = 0; i < n_elementi_da_stampare; i++) {
               printf("%lu, %lu\n", classifica[i].nome, classifica[i].punteggio);
           }

   */


    //------STAMPA PER PROGETTO------//

    for (i = 0; i < n_elementi_da_stampare - 1; i++) {
        printf("%lu ", classifica[i].nome);
    }
    printf("%lu\n", classifica[i].nome);
}


void copy_classifica(nome_punteggio *dacopiare, nome_punteggio *dovecopiare, unsigned long int startdacopiare,
                     unsigned long int startdovecopiare, unsigned long int quanticopiarne) {
    int i;
    for (i = 0; i < quanticopiarne; i++) {
        dovecopiare[startdovecopiare].punteggio = dacopiare[startdacopiare].punteggio;
        dovecopiare[startdovecopiare].nome = dacopiare[startdacopiare].nome;
        startdacopiare++;
        startdovecopiare++;
    }
}


void MERGE(nome_punteggio *classifica, nome_punteggio *temp, unsigned long int start, unsigned long int centro,
           unsigned long int final) {
    unsigned long int i = start;
    unsigned long int j = centro + 1;
    unsigned long int k = 0;


    while (i <= centro && j <= final) {
        if (classifica[i].punteggio <= classifica[j].punteggio) {
            temp[k].punteggio = classifica[i].punteggio;
            temp[k].nome = classifica[i].nome;
            i++;
        } else {
            temp[k].nome = classifica[j].nome;
            temp[k].punteggio = classifica[j].punteggio;
            j++;
        }
        k++;
    }
    if (i > centro) {
        while (j <= final) {
            temp[k].nome = classifica[j].nome;
            temp[k].punteggio = classifica[j].punteggio;
            j++;
            k++;
        }
    } else {
        while (i <= centro) {
            temp[k].punteggio = classifica[i].punteggio;
            temp[k].nome = classifica[i].nome;
            i++;
            k++;
        }
    }
    copy_classifica(temp, classifica, 0, start, k);

}


void MERGESORT(nome_punteggio *classifica, nome_punteggio *temp, unsigned long int start, unsigned long int final) {
    // start: indice iniziale dell'array
    // final: indice finale dell'array
    // centro: indice intermedio dell'array
    // Divido l'array in 2 sottoarray; Se l'array ha un elemento, è già ordinato e non faccio nulla.
    if (start < final) {
        unsigned long int centro = (start + final) / 2;
        MERGESORT(classifica, temp, start, centro);
        MERGESORT(classifica, temp, centro + 1, final);
        MERGE(classifica, temp, start, centro, final);
    }
}


void Acquisisci_Matrice(unsigned long int **matrice, int dimensione) {

    int i;
    for (i = 0; i < dimensione; i++) {
        stdin_loadrow();
        stdin_parserow(dimensione, matrice[i]);
    }
}


unsigned long int
CalcoloPunteggio(unsigned long int **matrice, unsigned long int dimensione, nodo *grafo,
                 nodo **priority_queue) {

    unsigned long int i, j, k;
    unsigned long int trovati = 1;
    unsigned long int tot = 0;
    unsigned long int nuovo_peso;

    /*
     *
     * i = nodo di partenza
     * j = nodo di arrivo
     * k = numero di nodi analizzati
     *
     */

    priority_queue[0] = &(grafo[0]);

    for (k = 0; k < trovati; k++) {


        /*
         *
         * SE INSERTION SORT
         *
         * i = priority_queue[k]->nome
         */

        i = Find_Next(priority_queue, trovati - 1);

        for (j = 1; j < dimensione; j++) {
//            if (i != j && matrice[i][j] != 0 && !grafo[j].esaminato &&
            if (matrice[i][j] != 0
                    ) {
                nuovo_peso = matrice[i][j] + grafo[i].peso;

                if (nuovo_peso < grafo[j].peso) {
                    //cambiato_punteggio = true;
                    grafo[j].peso = nuovo_peso;
                    grafo[j].nodo_precedente = i;

                    if (grafo[j].raggiunto == false) {
                        grafo[j].raggiunto = true;
                        priority_queue[trovati] = &(grafo[j]);
                        trovati++;
                    }
                }
            }
        }
        grafo[i].esaminato = true;
        tot = tot + grafo[i].peso;
    }

    return tot;
}

void Inizializza_Grafo(nodo *grafo, unsigned long int dimensione) {

    unsigned long int index;

    grafo[0].nome = 0;
    grafo[0].peso = 0;
    grafo[0].nodo_precedente = 0;
    grafo[0].raggiunto = true;
    grafo[0].esaminato = false;

    for (index = 1; index < dimensione; index++) {
        grafo[index].nome = index;
        grafo[index].peso = MAX;
        grafo[index].nodo_precedente = MAX;
        grafo[index].raggiunto = false;
        grafo[index].esaminato = false;
    }

}


unsigned long int Find_Next(nodo **priority, unsigned long int size) {

    unsigned long int n;
    unsigned long int next = priority[size]->nome;
    unsigned long int migliore = MAX;
    unsigned long int peso_da_cercare;

    for (n = 1; n <= size; n++) {
        if (!priority[n]->esaminato) {
            peso_da_cercare = priority[n]->peso;
            if (peso_da_cercare < migliore) {
                migliore = peso_da_cercare;
                next = priority[n]->nome;
            }
        }
    }

    return next;
}


static inline void stdin_parserow(int size, unsigned long int *ptr) {
    char *tmp_ptr = stdin_buffer;
    for (int i = 0; i < size; i++) {
        unsigned long int toReturn = 0;
        while (*tmp_ptr != ',' && *tmp_ptr != '\n') {
            toReturn *= 10;
            toReturn += (*tmp_ptr) - 48;
            tmp_ptr++;
        }

        tmp_ptr++;
        *ptr = toReturn;
        ptr++;
    }
}


static inline void stdin_loadrow() {
    if (fgets(stdin_buffer, stdin_buffer_size, stdin) == NULL && feof(stdin)) {}
}


static inline int stdin_getfch() {
    if (feof(stdin)) {
        return EOF;
    } else {
        return stdin_buffer[0];
    }
}


static inline void stdin_init(int length) {
    stdin_buffer_size = length * 10 + length + 10;
    stdin_buffer = malloc(stdin_buffer_size);
}


void Inserimento_Ordinato(nome_punteggio *classifica, unsigned long int numelem, nome_punteggio nuovo_elem) {

    int index = 0;

    while (classifica[index].punteggio < nuovo_elem.punteggio) {
        index++;
    }

    if (index != numelem){
        int gigi = numelem - index - 1;
        assert(gigi > 0);
        memmove(&(classifica[index + 1]), &(classifica[index]), (gigi) * sizeof(nome_punteggio));
    }
    classifica[index].nome = nuovo_elem.nome;
    classifica[index].punteggio = nuovo_elem.punteggio;
}


void Inizializza_Classifica(nome_punteggio *classifica, unsigned long int elem_class) {

    int index = 0;
//    nome_punteggio tmp;
//    tmp.nome = MAX;
//    tmp.punteggio = MAX;

    while (index < elem_class) {
//        memcpy(&(classifica[index]), &tmp, sizeof(nome_punteggio));
        classifica[index].nome = MAX;
        classifica[index].punteggio = MAX;
        index++;
    }
}


//------------------------------------------------------REMINISCENZE--------------------------------------------------//

/*
bool Antenato(nodo *grafo, unsigned long int ind_analizzare, unsigned long int ind_prev);

unsigned long int Partition(nodo **priority, unsigned long int start, unsigned long int final);

void Quick_Sort_Priority(nodo **priority, unsigned long int start, unsigned long int final);

void Bubble_Sort_Priority(nodo **priority, unsigned long start, unsigned long size);

 int Identifica_Comando(char *command);

void Insertion_Sort(nodo **priority, unsigned long int start, unsigned long int size);

unsigned long int Funzione_Buffa (nodo **priority, unsigned long int i, unsigned long int j);
*/

/*
 * void Bubble_Sort_Priority(nodo **priority, unsigned long int start, unsigned long int size) {
    unsigned long int i, j;


    for (i = start; i < size - 1; i++) {
        for (j = start; j < size - 1; j++) {
            if (priority[j]->peso > priority[j + 1]->peso) {
                nodo *tmp = priority[j + 1];
                priority[j + 1] = priority[j];
                priority[j] = tmp;
            }
        }
    }

}


void Insertion_Sort(nodo **priority, unsigned long int start, unsigned long int size) {
    unsigned long int i, j, k;

    for (i = start; i < size; i++) {
        k = priority[i]->peso;
        j = i - 1;

        while (j >= start && priority[j]->peso > k) {
            nodo *tmp = priority[j + 1];
            priority[j + 1] = priority[j];
            priority[j] = tmp;
            j = j - 1;
        }
        priority[j + 1]->peso = k;
    }
}

int Identifica_Comando(char *command) {

    if (strcmp(command, "TopK") == 0) {
        //printf("TopK\n");
        return 2;
    }
    if (strcmp(command, "AggiungiGrafo") == 0) {
        //printf("AggiungiGrafo\n");
        return 1;
    }
    if (strcmp(command, "END") == 0) {
        //printf("END\n");
        return 3;
    }
    printf("Identificato Niente\n");
    return -1;
}

void Quick_Sort_Priority(nodo **priority, unsigned long int start, unsigned long int final) {

    unsigned long int partitioned;
    if (start < final) {
        partitioned = Partition(priority, start, final);
        Quick_Sort_Priority(priority, start, partitioned - 1);
        Quick_Sort_Priority(priority, partitioned + 1, final);
    }
}


unsigned long int Partition(nodo **priority, unsigned long int start, unsigned long int final) {

    unsigned long int el = priority[final]->peso;
    unsigned long int i = start - 1;
    unsigned long int j;
    for (j = start; j <= final - 1; j++) {
        if (priority[j]->peso <= el) {
            i++;
            nodo *tmp = priority[i];
            priority[i] = priority[j];
            priority[j] = tmp;
        }
    }
    nodo *tmp = priority[i + 1];
    priority[i + 1] = priority[final];
    priority[final] = tmp;

    return (i + 1);

}

bool Antenato(nodo *grafo, unsigned long int ind_analizzare, unsigned long int ind_prev) {

    bool ritorno = true;

    if (ind_analizzare == 0) {
        return true;
    }
    if (grafo[ind_analizzare].nodo_precedente != ind_prev) {
        ritorno = Antenato(grafo, grafo[ind_analizzare].nodo_precedente, ind_prev);
    }
    if (grafo[ind_analizzare].nodo_precedente == ind_prev) {
        return false;
    }
    return ritorno;
}


char *split_base;
char *split_init;
char *split_str;

const int SPLIT_SIZE = 512*1024;
const char *SPLIT_DELIMITER = " ,\n";
char *SPLIT_INIT_STRING = "\0";

void initAcquisisciStringa() {
    split_base = malloc(SPLIT_SIZE + 1);
    *split_base = '\0';
    split_init = split_base - 1;
    split_str = split_base - 1;
}

static inline char *_Acquisisci_Stringa() {
    split_init = split_str + 1;
    split_str = split_init;
    while (*split_str != ',' && *split_str != ' ' && *split_str != '\n') {
//        printf("->WHILE: [%s] [%s] [%s]\n", split_base, split_init, split_str);
        if (*split_str == '\0') {
            int nchar = split_str - split_init;
            memmove(split_base, split_init, nchar);
            split_init = split_base;
            split_str = split_base + nchar;
            //printf("REALLOC1 %d\n", (SPLIT_SIZE - nchar));
            int nread = fread(split_str, 1, SPLIT_SIZE - nchar, stdin);
            //printf("REALLOC2 %d\n", nread);
            if (nread == 0 && feof(stdin)) {
                return "END";
            }
            *(split_str + 1 + nread) = '\0';
        } else {
            split_str++;
        }
    }

    *split_str = '\0';
//    printf("RETURN %s\n", split_init);
    return split_init;
//        // Ottengo i caratteri uno ad uno fino al '\n' e li inserisco nell'array
//    {
//        input = getc_unlocked(stdin);
//        if (input == ',' || input == ' ' || input == '\n') {
//            break;
//        }
//        if (input == EOF) {
//            return "END";
//        }
//        stringa[count++] = input;
//
//    }
//    stringa[count] = '\0';
//    return stringa;
//
//
//
//
//    char* toReturn = strtok(stringa, SPLIT_DELIMITER);
//
//    if (toReturn != NULL){
//        return toReturn;
//    }
//    else {
//        int r = fread(new_str, 1, SPLIT_SIZE, stdin);
//        new_str[r + 1] = '\0';
//
//        if (r == 0){
//            if (feof(stdin)){
//                return "END";
//            }
//        }
//
//        char* tmp = new_str;
//        new_str = current_str;
//        current_str = tmp;
//        tmp_str = current_str;
//        return _Acquisisci_Stringa(tmp_str);
//
//    }


//
//    char input;
//    int count = 0;
//
//    while (1)
//        // Ottengo i caratteri uno ad uno fino al '\n' e li inserisco nell'array
//    {
//        input = getc_unlocked(stdin);
//        if (input == ',' || input == ' ' || input == '\n') {
//            break;
//        }
//        if (input == EOF) {
//            return "END";
//        }
//        stringa[count++] = input;
//
//    }
//    stringa[count] = '\0';
//    return stringa;


}

static inline char *Acquisisci_Stringa() {
    return _Acquisisci_Stringa();
};

*/


