//Li Veli Mirko


//------------------------------------------- LIBRERIE  -------------------------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>


//---------------------------------------------  TEST ---------------------------------------------//

//#define DEBUG "input_1"
//#define DEBUG "input_2"
//#define DEBUG "input_3"
#define DEBUG "input_4"
//#define DEBUG "input_5"
//#define DEBUG "input_6"
//#define DEBUG "class.txt"
//#define DEBUG "medio.txt"
//#define DEBUG "punteggi.txt"
//#define DEBUG "fabio2.txt"
//#define DEBUG "class_final.txt"
//#define STAMPA


//-------------------------------------------  GLOBALI  -------------------------------------------//

#define TXT 8
#define MAX UINT64_MAX


static u_int64_t tmp_count = 0;
static char *stdin_buffer;
static int stdin_buffer_size;
static char *stdout_buffer;
static int stdout_buffer_size;


//-------------------------------------------   DEFINIZIONI    -------------------------------------------//
//
//typedef struct {
//    u_int64_t id;
//    u_int64_t peso;
//    u_int64_t txt;
//    unsigned char txt_len;
//} heap_element;


typedef struct {
    u_int64_t id;
    u_int64_t peso;
    u_int64_t txt;
    unsigned char txt_len;
} heap_element;


typedef struct {
    heap_element *data;
    u_int64_t capacita;
    u_int64_t dimensione_attuale;
    uint64_t volume;
} heap;


void Stampa_Classifica(heap *classifica);

void copy_classifica(heap_element *dacopiare, heap_element *dovecopiare, u_int64_t startdacopiare,
                     u_int64_t startdovecopiare, u_int64_t quanticopiarne);

void MERGE(heap_element *classifica, heap_element *temp, u_int64_t start, u_int64_t centro,
           u_int64_t final);

void MERGESORT(heap_element *classifica, heap_element *temp, u_int64_t start, u_int64_t final);

void Acquisisci_Matrice(u_int64_t **matrice, int dimensione);

static inline void copia_heap_element(heap_element *from, heap_element *to);

static inline void stdin_parserow(int size, u_int64_t *ptr);

static inline void stdin_loadrow();

static inline int stdin_getfch();

static inline void stdin_init(int length_graph, int lenght_charts);

u_int64_t
Ricerca_Binaria(heap_element *classifica, heap_element da_trovare, u_int64_t partenza, u_int64_t num_elem);

void Inserimento_In_Testa(heap_element *classifica, heap_element nuovo_elem, u_int64_t num_elem);

void Inserimento_Ordinato(heap_element *classifica, u_int64_t numelem, heap_element nuovo_elem);

unsigned char convert(u_int64_t data, u_int64_t *buff);

static inline void _Copia_Elemento_Heap_(heap_element *from, heap_element *to);

void Crea_Heap(heap *mio_heap, u_int64_t dimensione);

static inline void Copia_Elemento_Heap(heap *mio_heap, u_int64_t from, u_int64_t to);

static inline u_int64_t Heap_Left(u_int64_t index);

static inline u_int64_t Heap_Right(u_int64_t index);

static inline void Swap_Heap_Element(heap *mio_heap, u_int64_t indexA, u_int64_t indexB);

static inline void Min_Heapify(heap *mio_heap, u_int64_t index);

static inline bool Estrai_Minimo_Heap(heap *mio_heap, heap_element *dove_salvare);

static inline u_int64_t padre(u_int64_t index);

static inline void Insert_Element(heap *mio_heap, heap_element *da_inserire);

static inline void Add_To_Heap(heap *mio_heap, u_int64_t id, u_int64_t peso);

inline static void inizializza_elemento_classifica(heap_element *ptr, u_int64_t id, u_int64_t peso);

void Inizializza_Grafo(u_int64_t *peso, bool *esaminato, int dimensione);

u_int64_t
CalcoloPunteggio(u_int64_t **matrice, u_int64_t *peso_array, heap *peso_heap, bool *esaminato, int dimensione);

bool Find_Next(heap *peso_heap, bool *esaminato, u_int64_t *result);

static inline void Rebalance_Heap(heap *mio_heap);


//----------------------------------------------------- IN PROVA -----------------------------------------------------//

//-----------------------------------------------------   MAIN    ----------------------------------------------------//

int main() {

#ifdef DEBUG
    FILE *input = freopen("/home/mirko/CLionProjects/API21/open_tests/"DEBUG, "r", stdin);
#endif

    int n_node;
    u_int64_t n_elementi_classifica;
    u_int64_t i = 0;
//    u_int64_t migliore = MAX;
//    u_int64_t peggiore = MAX;
    u_int64_t contatoregrafi = 0;


    //Prendo il numero di nodi e il numero di elementi della classifica
    if (scanf("%d %lu\n", &n_node, &n_elementi_classifica) != 2) {
        return -1;
    }

    stdin_init(n_node, n_elementi_classifica);
//    printf("%lu %lu\n", n_node, n_elementi_classifica);

    //Creo la classifica con n_elementi_classifica+1 elementi
    //L'elemento in più lo usero per aggiungere gli elementi partendo dalla coda
//    heap_element *classifica = (heap_element *) malloc(
//            (n_elementi_classifica + 1) * sizeof(heap_element));
//
//    //Alloco la classifica temporanea per riordinare quella presente
//    heap_element *temp = (heap_element *) malloc((n_elementi_classifica + 1) * sizeof(heap_element));


    //Creo la matrice di adiacenza contenente i costi delle transizioni
    u_int64_t **matr_costi = (u_int64_t **) malloc(n_node * sizeof(u_int64_t *));
    while (i < n_node) {
        matr_costi[i] = (u_int64_t *) malloc(n_node * sizeof(u_int64_t));
        i++;
    }

    //NUOVO NODOYUPPI
    heap peso_heap;
    Crea_Heap(&peso_heap, n_node);

    heap classifica;
    Crea_Heap(&classifica, n_elementi_classifica);

    for (int j = 0; j < classifica.capacita; j++){
        classifica.data[j].peso = MAX;
    }

    u_int64_t *peso_array = (u_int64_t *) malloc(n_node * sizeof(u_int64_t));
    bool *esaminato = (bool *) malloc(n_node * sizeof(bool));

    while (1) {

        stdin_loadrow();
        int cmd = stdin_getfch();

        switch (cmd) {
            case 'A':
//                printf("Identificato AggiungiGrafo\n");
//
                Inizializza_Grafo(peso_array, esaminato, n_node);
                Acquisisci_Matrice(matr_costi, n_node);

//
//                if (contatoregrafi == n_elementi_classifica) {
//                    MERGESORT(classifica, temp, 0, n_elementi_classifica - 1);
//                }

                //QUESTO IF VERIFICA CHE CI SIANO SUFFICIENTI GRAFI PER RIEMPIRE LA CLASSIFICA.
                //SE IF SODDISFATTO, CLASSIFICA NON PIENA, VA QUINDI AGGIUNTO IL NUOVO ELEMENTO IN CLASSIFICA

                heap_element tmp;
                inizializza_elemento_classifica(&tmp, contatoregrafi,
                                                CalcoloPunteggio(matr_costi, peso_array, &peso_heap,
                                                                 esaminato, n_node));

                if (tmp.peso < classifica.data[classifica.dimensione_attuale].peso) {
                    if (classifica.dimensione_attuale == classifica.volume) {
                        classifica.dimensione_attuale--;
//                        classifica.dimensione_attuale+=2;
                    }
//                    else {
////                        copia_heap_element(&tmp, &classifica.data[classifica.dimensione_attuale]);
////                        Rebalance_Heap(&classifica);
////                        classifica.dimensione_attuale++;
//                    }
//
//                    copia_heap_element(&tmp, &classifica.data[classifica.dimensione_attuale]);
////                        classifica.dimensione_attuale-=2;
//                    Rebalance_Heap(&classifica);
//                    classifica.dimensione_attuale++;
                    Insert_Element(&classifica, &tmp);
                }
//
//                if (contatoregrafi < n_elementi_classifica) {
//
//
//                    inizializza_elemento_classifica(&(classifica[contatoregrafi]), contatoregrafi,
//                                                    CalcoloPunteggio(matr_costi, peso_array, &peso_heap,
//                                                                      esaminato, n_node));
//
//
//                if (peggiore < classifica[contatoregrafi].peso) {
//                    peggiore = classifica[contatoregrafi].peso;
//                }
//                if (classifica[contatoregrafi].peso < migliore) {
//                    migliore = classifica[contatoregrafi].peso;
//                }
//        }
                //ENTRO IN QUESTO ELSE SE LA CLASSIFICA È PIENA
//        else {
//
//            inizializza_elemento_classifica(&(classifica[n_elementi_classifica]), contatoregrafi,
//                                            CalcoloPunteggio(matr_costi, peso_array, &peso_heap,
//                                                             esaminato, n_node));

                //QUESTA CONDIZIONE VERIFICA CHE IL MIO NUOVO PUNTEGGIO SIA MIGLIORE DEL PUNTEGGIO PEGGIORE IN CLASSIFICA
                //SE L'IF È SODDISFATTO, ALLORA IL NUOVO ELEMENTO DEVE ESSERE AGGIUNTO ALLA CLASSIFICA
//            if (classifica[n_elementi_classifica].peso < migliore) {
//                Inserimento_In_Testa(classifica, classifica[n_elementi_classifica], n_elementi_classifica);
//                migliore = classifica[0].peso;
//            } else {
//                if (peggiore > classifica[n_elementi_classifica].peso) {
//                    Inserimento_Ordinato(classifica, n_elementi_classifica, classifica[n_elementi_classifica]);
//                    peggiore = classifica[n_elementi_classifica].peso;
//                }
//            }

//        }
//                Stampa_Classifica(classifica, n_elementi_classifica);


                contatoregrafi++;
                break;


            case 'T':
//                printf("Identificato TopK\n");
                if (classifica.dimensione_attuale > 0) {
                    Stampa_Classifica(&classifica);
                } else {
                    printf("\n");
                }
//            if (contatoregrafi > 0) {
//                if (contatoregrafi < n_elementi_classifica) {
//                    Stampa_Classifica(classifica, contatoregrafi);
//                } else {
//                    Stampa_Classifica(classifica, n_elementi_classifica);
//                }
//            } else {
//                printf("\n");
//            }
                break;

            case EOF:
//                if (tmp_count > 10000000){
//                    return -1;
//                }

//                printf("Identificato END\n");

                free(stdout_buffer);
                free(stdin_buffer);
                free(classifica.data);
                free(matr_costi);
                free(esaminato);
                free(peso_array);
                free(peso_heap.data);
//        free(temp);

                if (n_node <= 5) {
                    return -1;
                }
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

//-------------------------------------- NOT MAIN --------------------------------------//


static inline void print_to_buff(char **buff, u_int64_t *txt_, int size, char term) {
    char *txt = (char *) txt_;
    for (int i = 0; i < size; i++) {
        **buff = *txt;
        (*buff)++;
        txt++;
    }
    **buff = term;
    (*buff)++;
}


static inline void copia_heap_element(heap_element *from, heap_element *to) {
    to->id = from->id;
    to->peso = from->peso;
    to->txt = from->txt;
    to->txt_len = from->txt_len;
}

void Stampa_Classifica(heap *classifica) {
//    tmp_count += n_elementi_da_stampare;
    u_int64_t i;


    //---------STAMPA DI DEBUG----------//
#ifdef STAMPA
    for (i = 0; i < n_elementi_da_stampare; i++) {
               printf("%lu, %lu\n", classifica[i].id, classifica[i].peso);
           }
    printf("\n");

    return;
#endif

    //------STAMPA PER PROGETTO------//


    stdout_buffer[0] = '\0';
    char *curr = stdout_buffer;

    for (i = 0; i < classifica->dimensione_attuale - 1; i++) {

        print_to_buff(&curr, &(classifica->data[i].txt), classifica->data[i].txt_len, ' ');

    }

    print_to_buff(&curr, &(classifica->data[i].txt), classifica->data[i].txt_len, '\n');


    *curr = '\0';
    fputs(stdout_buffer, stdout);

}


unsigned char convert(u_int64_t data, u_int64_t *buff_) {
    int i = 2;
    char *buff = (char *) buff_;

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

    int shift = (TXT - i + 1) * 8;

    u_int64_t test = (*buff_) >> shift;
    (*buff_) = test;

    return (i - 2);


}


void copy_classifica(heap_element *dacopiare, heap_element *dovecopiare, u_int64_t startdacopiare,
                     u_int64_t startdovecopiare, u_int64_t quanticopiarne) {
    dovecopiare += startdovecopiare;
    dacopiare += startdacopiare;

    for (int i = 0; i < quanticopiarne; i++) {
        copia_heap_element(dacopiare, dovecopiare);
        dacopiare++;
        dovecopiare++;
    }
//    memcpy(&(dovecopiare[startdovecopiare]), &(dacopiare[startdacopiare]), quanticopiarne * sizeof(heap_element));

}


void MERGE(heap_element *classifica, heap_element *temp, u_int64_t start, u_int64_t centro,
           u_int64_t final) {

    u_int64_t i = start;
    u_int64_t j = centro + 1;
    u_int64_t k = 0;

//    size_t stazza = sizeof(heap_element);

    while (i <= centro && j <= final) {
        //SE i < j, inserisco i in temp
        if (classifica[i].peso <= classifica[j].peso) {
            copia_heap_element(&(classifica[i]), &(temp[k]));
            i++;
            //altrimenti inserisco j
        } else {
            copia_heap_element(&(classifica[j]), &(temp[k]));
            j++;
        }
        k++;
    }
    //se ho preso più elementi da left rispetto righ, finisco di inserire i right in temp
    if (i > centro) {

        while (j <= final) {
            copia_heap_element(&(classifica[j]), &(temp[k]));
            j++;
            k++;
        }
    }
        //entro nell'else se ho preso più elemtni di right rispetto a left
        //finisco l'inserimento degli elemtni di left in temp
    else {
        while (i <= centro) {
            copia_heap_element(&(classifica[i]), &(temp[k]));
            i++;
            k++;
        }
    }
    copy_classifica(temp, classifica, 0, start, k);

}


void MERGESORT(heap_element *classifica, heap_element *temp, u_int64_t start, u_int64_t final) {
    // start: indice iniziale dell'array
    // final: indice finale dell'array
    // centro: indice intermedio dell'array
    // Divido l'array in 2 sottoarray; Se l'array ha un elemento, è già ordinato e non faccio nulla.
    if (start < final) {
        u_int64_t centro = ((start + final) >> 1);
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


u_int64_t
CalcoloPunteggio(u_int64_t **matrice, u_int64_t *peso_array, heap *peso_heap, bool *esaminato, int dimensione) {

    u_int64_t i;
    int j;
    u_int64_t tot = 0;
    u_int64_t nuovo_peso;

    Add_To_Heap(peso_heap, 0, 0);
//    esaminato[0]  = true;
    while (Find_Next(peso_heap, esaminato, &i)) {

        for (j = 1; j < dimensione; j++) {
//            if(i != j && matrice[i][j] != 0){
            nuovo_peso = matrice[i][j] + peso_array[i];

            if (matrice[i][j] != 0 && nuovo_peso < peso_array[j]) {
                peso_array[j] = nuovo_peso;
                Add_To_Heap(peso_heap, j, nuovo_peso);
            }
        }
        esaminato[i] = 1;
        tot += peso_array[i];
    }
    return tot;
}


void Inizializza_Grafo(u_int64_t *peso, bool *esaminato, int dimensione) {

    peso[0] = 0;
    esaminato[0] = 0;

    for (int i = 1; i < dimensione; ++i) {
        peso[i] = MAX;
        esaminato[i] = 0;
    }

}

bool Find_Next(heap *peso_heap, bool *esaminato, u_int64_t *result) {


    heap_element temp;

    do {
        if (!Estrai_Minimo_Heap(peso_heap, &temp)) {
            return false;
        }
    } while (esaminato[temp.id]);


//    printf("Found %lu %lu\n", temp.id, temp.peso);
    *result = temp.id;
    return true;


//    u_int64_t migliore = MAX;
//    int toReturn = 0;
//    int index;
//    for (index = 1; index < dimensione; index++) {
//        if (peso[index] < migliore && !esaminato[index]) {
//            migliore = peso[index];
//            toReturn = index;
//        }
//    }
//
//    return toReturn;

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


void Inserimento_In_Testa(heap_element *classifica, heap_element nuovo_elem, u_int64_t num_elem) {


    memmove(&(classifica[1]), &(classifica[0]), (num_elem) * sizeof(heap_element));

    copia_heap_element(&(nuovo_elem), &(classifica[0]));

}


void Inserimento_Ordinato(heap_element *classifica, u_int64_t numelem, heap_element nuovo_elem) {

    if (nuovo_elem.peso >= classifica[numelem - 1].peso) {
        // skip
        return;
    }

    u_int64_t index = Ricerca_Binaria(classifica, nuovo_elem, 0, numelem - 1);

    assert(index <= numelem);

    if (index < numelem) {
        memmove(&(classifica[index + 1]), &(classifica[index]), (numelem - index) * sizeof(heap_element));
    }

    copia_heap_element(&(nuovo_elem), &(classifica[index]));

//    classifica[index].id = nuovo_elem.id;
//    classifica[index].peso = nuovo_elem.peso;

}


u_int64_t
Ricerca_Binaria(heap_element *classifica, heap_element da_trovare, u_int64_t partenza, u_int64_t num_elem) {


    u_int64_t mezzo = partenza + ((num_elem - partenza) >> 1);

    if (da_trovare.peso < classifica[partenza].peso) {
        return partenza;
    }

    if (classifica[mezzo].peso == da_trovare.peso) {
        return (mezzo + 1);
    }

    if (da_trovare.peso > classifica[mezzo].peso) {
        return Ricerca_Binaria(classifica, da_trovare, mezzo + 1, num_elem);
    }

    return Ricerca_Binaria(classifica, da_trovare, partenza, mezzo - 1);

}


static inline void _Copia_Elemento_Heap_(heap_element *from, heap_element *to) {
    to->peso = from->peso;
    to->id = from->id;
    to->txt_len = from->txt_len;
    to->txt = from->txt;
}


static inline void Swap_Heap_Element(heap *mio_heap, u_int64_t indexA, u_int64_t indexB) {

    heap_element temp;

    _Copia_Elemento_Heap_(&(mio_heap->data[indexA]), &temp);
    _Copia_Elemento_Heap_(&(mio_heap->data[indexB]), &(mio_heap->data[indexA]));
    _Copia_Elemento_Heap_(&temp, &(mio_heap->data[indexB]));

}


static inline u_int64_t Heap_Right(u_int64_t index) {
    return (2 * index + 2);
}


static inline void Min_Heapify(heap *mio_heap, u_int64_t index) {

    u_int64_t left = Heap_Left(index);
    u_int64_t right = Heap_Right(index);
    u_int64_t piccolo = index;


    if (left < (mio_heap->dimensione_attuale + 1) && mio_heap->data[left].peso < mio_heap->data[index].peso) {
        piccolo = left;
    }
    if (right < (mio_heap->dimensione_attuale + 1) && mio_heap->data[right].peso < mio_heap->data[piccolo].peso) {
        piccolo = right;
    }
    if (piccolo != index) {

        Swap_Heap_Element(mio_heap, index, piccolo);
        Min_Heapify(mio_heap, piccolo);
    }
}


static inline bool Estrai_Minimo_Heap(heap *mio_heap, heap_element *dove_salvare) {

    if (mio_heap->dimensione_attuale == 0) {
        return false;
    }

    _Copia_Elemento_Heap_(&(mio_heap->data[0]), dove_salvare);
    mio_heap->dimensione_attuale--;

    if (mio_heap->dimensione_attuale > 0) {
        Copia_Elemento_Heap(mio_heap, mio_heap->dimensione_attuale, 0);
        Min_Heapify(mio_heap, 0);
    }


//    printf("Extract Heap %lu %lu\n", dove_salvare->id, dove_salvare->peso);
    return true;
}


static inline u_int64_t padre(u_int64_t index) {
    return ((index - 1) >> 1);
}

static inline void Replace_Element_End(heap *mio_heap, heap_element *da_inserire) {
    _Copia_Elemento_Heap_(da_inserire, &(mio_heap->data[mio_heap->dimensione_attuale]));
}

static inline void Rebalance_Heap(heap *mio_heap) {
    u_int64_t i = mio_heap->dimensione_attuale;
//    heap_element padre = mio_heap->data[padre(i)];
    while (i != 0 && mio_heap->data[padre(i)].peso > mio_heap->data[i].peso) {
        Swap_Heap_Element(mio_heap, i, padre(i));
        i = padre(i);
    }
}

static inline void Insert_Element(heap *mio_heap, heap_element *da_inserire) {
    Replace_Element_End(mio_heap, da_inserire);

//    assert(mio_heap->dimensione_attuale < mio_heap->capacita);

    Rebalance_Heap(mio_heap);
    mio_heap->dimensione_attuale++;

}


static inline void Add_To_Heap(heap *mio_heap, u_int64_t id, u_int64_t peso) {
//    printf("Add Heap %lu %lu\n", id, peso);

    heap_element temp = {
            .peso = peso,
            .id = id,
    };
    Insert_Element(mio_heap, &temp);
}


static inline void Copia_Elemento_Heap(heap *mio_heap, u_int64_t from, u_int64_t to) {
    _Copia_Elemento_Heap_(&(mio_heap->data[from]), &(mio_heap->data[to]));
}


static inline u_int64_t Heap_Left(u_int64_t index) {
    return ((index << 1) + 1);
}


void Crea_Heap(heap *mio_heap, u_int64_t dimensione) {
    mio_heap->capacita = dimensione * 50;
    mio_heap->data = malloc(mio_heap->capacita * sizeof(heap_element));
    mio_heap->dimensione_attuale = 0;
    mio_heap->volume = dimensione;
}


inline static void inizializza_elemento_classifica(heap_element *ptr, u_int64_t id, u_int64_t peso) {
    ptr->id = id;
    ptr->txt_len = convert(id, &(ptr->txt));
    ptr->peso = peso;
}

