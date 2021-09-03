#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>

//#define DEBUG "input_1"
//#define DEBUG "generato.txt"
//#define DEBUG "50x50.txt"
//#define DEBUG "8x20000.txt"
#define MAX UINT64_MAX


static char *stdin_buffer;
static u_int64_t stdin_buffer_size;

typedef struct nodo nodo;

typedef struct lnodo_t {
    uint64_t peso;
    void* ptr;
    struct lnodo_t* next;
} lnodo_t;

//-------------------------------------------   LIBRERIA    ----------------------------------------------------------//

typedef struct {
    u_int64_t nome;
    u_int64_t punteggio;
} nome_punteggio;

typedef struct {
    u_int64_t nome;
    u_int64_t peso;
    u_int64_t nodo_precedente;
    bool raggiunto;
    bool esaminato;
} nodo;

void print_classifica(nome_punteggio *classifica, u_int64_t n_elementi_da_stampare);

void copy_classifica(nome_punteggio *dacopiare, nome_punteggio *dovecopiare, u_int64_t startdacopiare,
                     u_int64_t startdovecopiare, u_int64_t quanticopiarne);

void MERGE(nome_punteggio *classifica, nome_punteggio *temp, u_int64_t start, u_int64_t centro,
           u_int64_t final);

void MERGESORT(nome_punteggio *classifica, nome_punteggio *temp, u_int64_t start, u_int64_t final);

void Acquisisci_Matrice(u_int64_t **matrice, u_int64_t dimensione);

u_int64_t CalcoloPunteggio(u_int64_t **matrice, u_int64_t dimensione, nodo *grafo,
                 nodo **priority_queue);

void Inizializza_Grafo(nodo *grafo, u_int64_t dimensione);


u_int64_t Find_Next(nodo **priority, u_int64_t size);

static inline void stdin_parserow(u_int64_t size, u_int64_t *ptr);

static inline void stdin_loadrow();

static inline int stdin_getfch();

static inline void stdin_init(u_int64_t length);

void Inserimento_Ordinato(nome_punteggio *classifica, u_int64_t numelem, nome_punteggio nuovo_elem);

void Inizializza_Classifica(nome_punteggio *classifica, u_int64_t elem_class);

u_int64_t Ricerca_Binaria (nome_punteggio* classifica, nome_punteggio da_trovare, u_int64_t partenza, u_int64_t num_elem);

//-----------------------------------------------------   MAIN    ----------------------------------------------------//

int main() {

#ifdef DEBUG
    FILE *input = freopen("/home/mirko/CLionProjects/API21/open_tests/"DEBUG, "r", stdin);
#endif


    u_int64_t n_node;
    u_int64_t n_elementi_classifica;
    u_int64_t i = 0;
//    u_int64_t peggiore = MAX;
    u_int64_t contatoregrafi = 0;

    if (scanf("%lu %lu\n", &n_node, &n_elementi_classifica) != 2) {
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
    u_int64_t **matr_costi = (u_int64_t **) malloc(n_node * sizeof(u_int64_t *));
    while (i < n_node) {
        matr_costi[i] = (u_int64_t *) malloc(n_node * sizeof(u_int64_t));
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

                nome_punteggio new_p = {
                        .nome = contatoregrafi,
                        .punteggio = CalcoloPunteggio(matr_costi, n_node, grafo,
                                                      priority_queue)
                };

//                if (new_p.punteggio <= peggiore){
                    Inserimento_Ordinato(classifica, n_elementi_classifica, new_p);
//                }


                contatoregrafi++;
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


void print_classifica(nome_punteggio *classifica, u_int64_t n_elementi_da_stampare) {

    u_int64_t i;

    //---------STAMPA DI DEBUG----------//


#ifdef DEBUG
           for (i = 0; i < n_elementi_da_stampare; i++) {
               printf("%lu, %lu\n", classifica[i].nome, classifica[i].punteggio);
           }
    printf("-\n");
    int a = 1+1;
    a++;
   return;
#endif


    //------STAMPA PER PROGETTO------//

    for (i = 0; i < n_elementi_da_stampare - 1; i++) {
        printf("%lu ", classifica[i].nome);
    }
    printf("%lu\n", classifica[i].nome);
}


void copy_classifica(nome_punteggio *dacopiare, nome_punteggio *dovecopiare, u_int64_t startdacopiare,
                     u_int64_t startdovecopiare, u_int64_t quanticopiarne) {
    int i;
    for (i = 0; i < quanticopiarne; i++) {
        dovecopiare[startdovecopiare].punteggio = dacopiare[startdacopiare].punteggio;
        dovecopiare[startdovecopiare].nome = dacopiare[startdacopiare].nome;
        startdacopiare++;
        startdovecopiare++;
    }
}
//
//
//void MERGE(nome_punteggio *classifica, nome_punteggio *temp, u_int64_t start, u_int64_t centro,
//           u_int64_t final) {
//    u_int64_t i = start;
//    u_int64_t j = centro + 1;
//    u_int64_t k = 0;
//
//
//    while (i <= centro && j <= final) {
//        if (classifica[i].punteggio <= classifica[j].punteggio) {
//            temp[k].punteggio = classifica[i].punteggio;
//            temp[k].nome = classifica[i].nome;
//            i++;
//        } else {
//            temp[k].nome = classifica[j].nome;
//            temp[k].punteggio = classifica[j].punteggio;
//            j++;
//        }
//        k++;
//    }
//    if (i > centro) {
//        while (j <= final) {
//            temp[k].nome = classifica[j].nome;
//            temp[k].punteggio = classifica[j].punteggio;
//            j++;
//            k++;
//        }
//    } else {
//        while (i <= centro) {
//            temp[k].punteggio = classifica[i].punteggio;
//            temp[k].nome = classifica[i].nome;
//            i++;
//            k++;
//        }
//    }
//    copy_classifica(temp, classifica, 0, start, k);
//
//}
//
//
//void MERGESORT(nome_punteggio *classifica, nome_punteggio *temp, u_int64_t start, u_int64_t final) {
//    // start: indice iniziale dell'array
//    // final: indice finale dell'array
//    // centro: indice intermedio dell'array
//    // Divido l'array in 2 sottoarray; Se l'array ha un elemento, è già ordinato e non faccio nulla.
//    if (start < final) {
//        u_int64_t centro = (start + final) / 2;
//        MERGESORT(classifica, temp, start, centro);
//        MERGESORT(classifica, temp, centro + 1, final);
//        MERGE(classifica, temp, start, centro, final);
//    }
//}


void Acquisisci_Matrice(u_int64_t **matrice, u_int64_t dimensione) {

    u_int64_t i;
    for (i = 0; i < dimensione; i++) {
        stdin_loadrow();
        stdin_parserow(dimensione, matrice[i]);
    }
}


u_int64_t CalcoloPunteggio(u_int64_t **matrice, u_int64_t dimensione, nodo *grafo,
                 nodo **priority_queue) {

    u_int64_t i, j, k;
    u_int64_t trovati = 1;
    u_int64_t tot = 0;
    u_int64_t nuovo_peso;

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
            if (i != j && matrice[i][j] != 0 && !grafo[j].esaminato){
//            if (matrice[i][j] != 0
//                    ) {
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

void Inizializza_Grafo(nodo *grafo, u_int64_t dimensione) {

    u_int64_t index;

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


u_int64_t Find_Next(nodo **priority, u_int64_t size) {

    u_int64_t n;
    u_int64_t next = priority[size]->nome;
    u_int64_t migliore = MAX;
    u_int64_t peso_da_cercare;

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


static inline void stdin_parserow(u_int64_t size, u_int64_t *ptr) {
    char *tmp_ptr = stdin_buffer;
    for (u_int64_t i = 0; i < size; i++) {
        u_int64_t toReturn = 0;
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


static inline void stdin_init(u_int64_t length) {
    stdin_buffer_size = length * 10 + length + 10;
    stdin_buffer = malloc(stdin_buffer_size);
}


void Inserimento_Ordinato_2(lnodo_t* list, lnodo_t* data){
    while (list->peso < data->peso){
        if (list == NULL){
            return;
        }
        list = list->next;
    }
}

void Inserimento_Ordinato(nome_punteggio *classifica, u_int64_t numelem, nome_punteggio nuovo_elem) {


    if (nuovo_elem.punteggio >= classifica[numelem - 1].punteggio){
        // skip
        return;
    }

    u_int64_t index = Ricerca_Binaria(classifica, nuovo_elem, 0, numelem - 1);

    assert(index <= numelem);

    if (index < numelem){
        memmove(&(classifica[index + 1]), &(classifica[index]), (numelem - index) * sizeof(nome_punteggio));
    }

    classifica[index].nome = nuovo_elem.nome;
    classifica[index].punteggio = nuovo_elem.punteggio;

//    print_classifica(classifica, numelem + 1);
}


u_int64_t Ricerca_Binaria (nome_punteggio* classifica, nome_punteggio da_trovare, u_int64_t partenza, u_int64_t num_elem){


    u_int64_t mezzo = partenza + (num_elem - partenza)/2;

    if (da_trovare.punteggio < classifica[partenza].punteggio){
        return partenza;
    }

    if (classifica[mezzo].punteggio == da_trovare.punteggio){
        return mezzo + 1;
    }

    if (da_trovare.punteggio > classifica[mezzo].punteggio){
        return Ricerca_Binaria(classifica, da_trovare, mezzo + 1, num_elem);
    }

    return Ricerca_Binaria(classifica, da_trovare, partenza, mezzo - 1);

}


void Inizializza_Classifica(nome_punteggio *classifica, u_int64_t elem_class) {

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

//    print_classifica(classifica, elem_class);

}

