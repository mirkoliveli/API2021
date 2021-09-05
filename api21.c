//Li Veli Mirko


//------------------------------------------- LIBRERIE  -------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


//---------------------------------------------  TEST ---------------------------------------------//

//#define DEBUG "input_1"
//#define DEBUG "input_2"
//#define DEBUG "input_3"
//#define DEBUG "input_4"
//#define DEBUG "input_5"
//#define DEBUG "input_6"
//#define DEBUG "class.txt"
//#define DEBUG "medio.txt"
#define DEBUG "punteggi.txt"
//#define DEBUG "class_final.txt"
//#define STAMPA


//-------------------------------------------  GLOBALI  -------------------------------------------//

#define TXT 10
#define MAX 4294967295

static char *stdin_buffer;
static int stdin_buffer_size;
static char *stdout_buffer;
static int stdout_buffer_size;


//-------------------------------------------   DEFINIZIONI    -------------------------------------------//

typedef struct {
    u_int64_t nome;
    u_int64_t punteggio;
    char txt[TXT];
    bool init;
    unsigned char txt_len;
} nome_punteggio;

typedef struct {
    u_int64_t nome;
    u_int64_t peso;
    bool raggiunto;
    bool esaminato;
} nodo;

void Stampa_Classifica(nome_punteggio *classifica, u_int64_t n_elementi_da_stampare);

void copy_classifica(nome_punteggio *dacopiare, nome_punteggio *dovecopiare, u_int64_t startdacopiare,
                     u_int64_t startdovecopiare, u_int64_t quanticopiarne);

void MERGE(nome_punteggio *classifica, nome_punteggio *temp, u_int64_t start, u_int64_t centro,
           u_int64_t final);

void MERGESORT(nome_punteggio *classifica, nome_punteggio *temp, u_int64_t start, u_int64_t final);

void Acquisisci_Matrice(u_int64_t **matrice, int dimensione);

u_int64_t CalcoloPunteggio(u_int64_t **matrice, u_int64_t dimensione, nodo *grafo,
                           nodo **priority_queue);

void Inizializza_Grafo(nodo *grafo, u_int64_t dimensione);

u_int64_t Find_Next(nodo **priority, u_int64_t size);

static inline void stdin_parserow(int size, u_int64_t *ptr);

static inline void stdin_loadrow();

static inline int stdin_getfch();

static inline void stdin_init(int length_graph, int lenght_charts);

u_int64_t
Ricerca_Binaria(nome_punteggio *classifica, nome_punteggio da_trovare, u_int64_t partenza, u_int64_t num_elem);

void Inserimento_In_Testa(nome_punteggio *classifica, nome_punteggio nuovo_elem, u_int64_t num_elem);

void Inserimento_Ordinato(nome_punteggio *classifica, u_int64_t numelem, nome_punteggio nuovo_elem);

unsigned char convert(u_int64_t data, char *buff);



//-----------------------------------------------------   MAIN    ----------------------------------------------------//

int main() {

#ifdef DEBUG
    FILE *input = freopen("/home/mirko/CLionProjects/API21/open_tests/"DEBUG, "r", stdin);
#endif


    int n_node;
    u_int64_t n_elementi_classifica;
    u_int64_t i = 0;
    u_int64_t migliore = MAX;
    u_int64_t peggiore = MAX;
    u_int64_t contatoregrafi = 0;


    //Prendo il numero di nodi e il numero di elementi della classifica
    if (scanf("%d %lu\n", &n_node, &n_elementi_classifica) != 2) {
        return -1;
    }

    stdin_init(n_node, n_elementi_classifica);
//    printf("%lu %lu\n", n_node, n_elementi_classifica);

    //Creo la classifica con n_elementi_classifica+1 elementi
    //L'elemento in più lo usero per aggiungere gli elementi partendo dalla coda
    nome_punteggio *classifica = (nome_punteggio *) malloc(
            (n_elementi_classifica + 1) * sizeof(nome_punteggio));

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

        switch (cmd) {
            case 'A':
//                printf("Identificato AggiungiGrafo\n");

                Inizializza_Grafo(grafo, n_node);
                Acquisisci_Matrice(matr_costi, n_node);

                if (contatoregrafi == n_elementi_classifica) {
                    MERGESORT(classifica, temp, 0, n_elementi_classifica - 1);
                }

                //QUESTO IF VERIFICA CHE CI SIANO SUFFICIENTI GRAFI PER RIEMPIRE LA CLASSIFICA.
                //SE IF SODDISFATTO, CLASSIFICA NON PIENA, VA QUINDI AGGIUNTO IL NUOVO ELEMENTO IN CLASSIFICA
                if (contatoregrafi < n_elementi_classifica) {


                    classifica[contatoregrafi].nome = contatoregrafi;
                    classifica[contatoregrafi].txt[0] = '\0';
                    classifica[contatoregrafi].init = false;
                    classifica[contatoregrafi].punteggio = CalcoloPunteggio(matr_costi, n_node, grafo,
                                                                            priority_queue);


                    if (peggiore < classifica[contatoregrafi].punteggio) {
                        peggiore = classifica[contatoregrafi].punteggio;
                    }
                    if (classifica[contatoregrafi].punteggio < migliore) {
                        migliore = classifica[contatoregrafi].punteggio;
                    }

                }
                    //ENTRO IN QUESTO ELSE SE LA CLASSIFICA È PIENA
                else {


                    classifica[n_elementi_classifica].init = false;
                    classifica[n_elementi_classifica].txt[0] = '\0';
                    classifica[n_elementi_classifica].nome = contatoregrafi;
                    classifica[n_elementi_classifica].punteggio = CalcoloPunteggio(matr_costi, n_node, grafo,
                                                                                   priority_queue);

                    //QUESTA CONDIZIONE VERIFICA CHE IL MIO NUOVO PUNTEGGIO SIA MIGLIORE DEL PUNTEGGIO PEGGIORE IN CLASSIFICA
                    //SE L'IF È SODDISFATTO, ALLORA IL NUOVO ELEMENTO DEVE ESSERE AGGIUNTO ALLA CLASSIFICA
                    if (classifica[n_elementi_classifica].punteggio < migliore) {
                        //Inserimento in testa
                        Inserimento_In_Testa(classifica, classifica[n_elementi_classifica], n_elementi_classifica);
                        migliore = classifica[0].punteggio;
                    } else {
                        if (peggiore > classifica[n_elementi_classifica].punteggio) {
                            Inserimento_Ordinato(classifica, n_elementi_classifica, classifica[n_elementi_classifica]);
                            peggiore = classifica[n_elementi_classifica].punteggio;
                        }
                    }
                }

                contatoregrafi++;
                break;


            case 'T':
//                printf("Identificato TopK\n");
                if (contatoregrafi > 0) {
                    if (contatoregrafi < n_elementi_classifica) {
                        Stampa_Classifica(classifica, contatoregrafi);
                    } else {
                        Stampa_Classifica(classifica, n_elementi_classifica);
                    }
                } else {
                    printf("\n");
                }
                break;

            case EOF:
//                printf("Identificato END\n");

                free(priority_queue);
                free(stdout_buffer);
                free(stdin_buffer);
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


void Stampa_Classifica(nome_punteggio *classifica, u_int64_t n_elementi_da_stampare) {

    u_int64_t i;


    //---------STAMPA DI DEBUG----------//
#ifdef STAMPA
    for (i = 0; i < n_elementi_da_stampare; i++) {
               printf("%lu, %lu\n", classifica[i].nome, classifica[i].punteggio);
           }
    printf("\n");

    return;
#endif

    //------STAMPA PER PROGETTO------//


    stdout_buffer[0] = '\0';
    char *curr = stdout_buffer;

    for (i = 0; i < n_elementi_da_stampare; i++) {

        if (classifica[i].init == false) {
            classifica[i].init = true;
            classifica[i].txt_len = convert(classifica[i].nome, classifica[i].txt);
        }

        memcpy(curr, classifica[i].txt, classifica[i].txt_len);
        curr += classifica[i].txt_len;
        *curr = ((i == n_elementi_da_stampare - 1) ? '\n' : ' ');
        curr++;
    }

    *curr = '\0';
    fputs(stdout_buffer, stdout);

}


unsigned char convert(u_int64_t data, char *buff) {

    int i = 2;
    buff[TXT - 1] = '\0';

    if (data == 0) {
        buff[TXT - i] = '0';
        i++;
    } else {
        while (data > 0) {
            char temp = data % 10;
            buff[TXT - i] = temp + 48;
            data /= 10;
            i++;
        }
    }

    memmove(buff, &(buff[TXT - i + 1]), i - 1);
    return (i - 2);

}


void copy_classifica(nome_punteggio *dacopiare, nome_punteggio *dovecopiare, u_int64_t startdacopiare,
                     u_int64_t startdovecopiare, u_int64_t quanticopiarne) {

    memcpy(&(dovecopiare[startdovecopiare]), &(dacopiare[startdacopiare]), quanticopiarne * sizeof(nome_punteggio));

}


void MERGE(nome_punteggio *classifica, nome_punteggio *temp, u_int64_t start, u_int64_t centro,
           u_int64_t final) {

    u_int64_t i = start;
    u_int64_t j = centro + 1;
    u_int64_t k = 0;

    size_t stazza = sizeof(nome_punteggio);

    while (i <= centro && j <= final) {
        if (classifica[i].punteggio <= classifica[j].punteggio) {

            memcpy(&(temp[k]), &(classifica[i]), stazza);

            i++;
        } else {

            memcpy(&(temp[k]), &(classifica[j]), stazza);

            j++;
        }
        k++;
    }
    if (i > centro) {
        while (j <= final) {

            memcpy(&(temp[k]), &(classifica[j]), stazza);

            j++;
            k++;
        }
    } else {
        while (i <= centro) {

            memcpy(&(temp[k]), &(classifica[i]), stazza);
            i++;
            k++;
        }
    }
    copy_classifica(temp, classifica, 0, start, k);

}


void MERGESORT(nome_punteggio *classifica, nome_punteggio *temp, u_int64_t start, u_int64_t final) {
    // start: indice iniziale dell'array
    // final: indice finale dell'array
    // centro: indice intermedio dell'array
    // Divido l'array in 2 sottoarray; Se l'array ha un elemento, è già ordinato e non faccio nulla.
    if (start < final) {
        u_int64_t centro = (start + final) / 2;
        MERGESORT(classifica, temp, start, centro);
        MERGESORT(classifica, temp, centro + 1, final);
        MERGE(classifica, temp, start, centro, final);
    }
}


void Acquisisci_Matrice(u_int64_t **matrice, int dimensione) {

    int i;
    for (i = 0; i < dimensione; i++) {
        stdin_loadrow();
        stdin_parserow(dimensione, matrice[i]);
    }
}


/*
    CALCOLO_PUNTEGGIO
    // i = nodo di partenza
    // j = nodo di arrivo
    // k = numero nodi analizzati
 */
u_int64_t CalcoloPunteggio(u_int64_t **matrice, u_int64_t dimensione, nodo *grafo,
                           nodo **priority_queue) {

    u_int64_t i, j, k;
    u_int64_t trovati = 1;
    u_int64_t tot = 0;
    u_int64_t nuovo_peso;
    priority_queue[0] = &(grafo[0]);

    for (k = 0; k < trovati; k++) {

        i = Find_Next(priority_queue, trovati - 1);

        for (j = 1; j < dimensione; j++) {
//            if (i != j && matrice[i][j] != 0) {
            if (matrice[i][j] != 0) {
                nuovo_peso = matrice[i][j] + grafo[i].peso;

                if (!grafo[j].raggiunto) {
                    grafo[j].raggiunto = true;
                    grafo[j].peso = nuovo_peso;
                    priority_queue[trovati] = &(grafo[j]);
                    trovati++;
                }

                if (nuovo_peso < grafo[j].peso) {
                    grafo[j].peso = nuovo_peso;
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
    grafo[0].raggiunto = true;
    grafo[0].esaminato = false;

    for (index = 1; index < dimensione; index++) {
        grafo[index].nome = index;
        grafo[index].raggiunto = false;
        grafo[index].esaminato = false;
    }

}


u_int64_t Find_Next(nodo **priority, u_int64_t size) {

    //La funzione ritorna l'indice del nodo non analizzato col minor peso

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


static inline void stdin_parserow(int size, u_int64_t *ptr) {

    char *tmp_ptr = stdin_buffer;
    for (int i = 0; i < size; i++) {
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


static inline void stdin_init(int length_graph, int length_charts) {

    stdin_buffer_size = length_graph * 10 + length_graph + 10;
    stdin_buffer = malloc(stdin_buffer_size);

    stdout_buffer_size = (length_charts + 1) * 10 + length_charts + 10;
    stdout_buffer = malloc(stdout_buffer_size);
}


void Inserimento_In_Testa(nome_punteggio *classifica, nome_punteggio nuovo_elem, u_int64_t num_elem) {


    memmove(&(classifica[1]), &(classifica[0]), (num_elem) * sizeof(nome_punteggio));
    memcpy(&(classifica[0]), &(nuovo_elem), sizeof(nome_punteggio));

}


void Inserimento_Ordinato(nome_punteggio *classifica, u_int64_t numelem, nome_punteggio nuovo_elem) {

    if (nuovo_elem.punteggio >= classifica[numelem - 1].punteggio) {
        // skip
        return;
    }

    u_int64_t index = Ricerca_Binaria(classifica, nuovo_elem, 0, numelem - 1);

    assert(index <= numelem);

    if (index < numelem) {
        memmove(&(classifica[index + 1]), &(classifica[index]), (numelem - index) * sizeof(nome_punteggio));
    }

    memcpy(&(classifica[index]), &(nuovo_elem), sizeof(nome_punteggio));

//    classifica[index].nome = nuovo_elem.nome;
//    classifica[index].punteggio = nuovo_elem.punteggio;

}


u_int64_t
Ricerca_Binaria(nome_punteggio *classifica, nome_punteggio da_trovare, u_int64_t partenza, u_int64_t num_elem) {


    u_int64_t mezzo = partenza + ((num_elem - partenza) >> 1);

    if (da_trovare.punteggio < classifica[partenza].punteggio) {
        return partenza;
    }

    if (classifica[mezzo].punteggio == da_trovare.punteggio) {
        return (mezzo + 1);
    }

    if (da_trovare.punteggio > classifica[mezzo].punteggio) {
        return Ricerca_Binaria(classifica, da_trovare, mezzo + 1, num_elem);
    }

    return Ricerca_Binaria(classifica, da_trovare, partenza, mezzo - 1);

}