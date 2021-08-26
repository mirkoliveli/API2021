#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 4294967295

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

void MERGE(nome_punteggio *classifica, unsigned long int start, unsigned long int centro,
           unsigned long int final);

void MERGESORT(nome_punteggio *classifica, unsigned long int start, unsigned long int final);

char *Acquisisci_Stringa(char *stringa);

int Identifica_Comando(char *command);

void Acquisisci_Mactrice(unsigned  long int **matrice, unsigned int dimensione, char *stringa);

unsigned  long int
CalcoloPunteggio(unsigned  long int **matrice, unsigned  long int dimensione, nodo *grafo,
                 nodo **priority_queue);

void Inizializza_Grafo(nodo *grafo, unsigned  long int dimensione);

bool Antenato(nodo *grafo, unsigned  long int ind_analizzare, unsigned  long int ind_prev);

unsigned long int Partition(nodo **priority, unsigned  long int start, unsigned  long int final);

void Quick_Sort_Priority(nodo **priority, unsigned  long int start, unsigned  long int final);

void Bubble_Sort_Priority(nodo **priority, unsigned  long start, unsigned  long size);




//-----------------------------------------------------   MAIN    ----------------------------------------------------//

int main() {

    unsigned  long int n_node;
    unsigned  long int n_elementi_classifica;
    int command_id;
    unsigned  long int i = 0;
    unsigned  long int peggiore = 0;
    unsigned  long int contatoregrafi = 0;
    char *stringa = NULL;
    char *eptr;
    stringa = calloc(20, sizeof(char));


    /*
     * Acquisisce il primo input
     * Formato primo input: numero_nodi_del_grafo (carattere spazio) numero_elementi_della_classifica
     *
     * OLD:
     *     firstinput = Acquisisci_Stringa(firstinput);
     *     sscanf(firstinput, "%d %d%*c", &n_node, &n_elementi_classifica);
     */

    stringa = Acquisisci_Stringa(stringa);
    n_node = strtoul(stringa, &eptr, 10);

    stringa = Acquisisci_Stringa(stringa);
    n_elementi_classifica = strtoul(stringa, &eptr, 10);



    //Creo la classifica con n_elementi_classifica+1 elementi
    //L'elemento in più lo usero per aggiungere gli elementi partendo dalla coda
    nome_punteggio *classifica = (nome_punteggio *) malloc(
            (n_elementi_classifica + 1) * sizeof(nome_punteggio));

    //Inizializza_Classifica(classifica, n_elementi_classifica + 1);



    //Creo la matrice di adiacenza contenente i costi delle transizioni
    unsigned  long int **matr_costi = (unsigned  long int **) malloc(n_node * sizeof(unsigned  long int *));
    while (i < n_node) {
        matr_costi[i] = (unsigned  long int *) malloc(n_node * sizeof(unsigned  long int));
        i++;
    }


    //Alloco la memoria per il grafo. Grafo[] di nodi
    nodo *grafo = (nodo *) malloc(n_node * sizeof(nodo));

    //Alloco la priority queue
    nodo **priority_queue = (nodo **) malloc(n_node * sizeof(nodo *));


    while (1) {

        //printf("%lu\n", contatoregrafi);
        stringa = Acquisisci_Stringa(stringa);
        command_id = Identifica_Comando(stringa);
        switch (command_id) {
            case 1:
                //printf("Identificato AggiungiGrafo\n");

                Inizializza_Grafo(grafo, n_node);
                Acquisisci_Mactrice(matr_costi, n_node, stringa);



                //-----------------STAMPA PUNTEGGI DI DEBUG-----------------------//

//                printf("%lu, %lu\n", classifica[n_elementi_classifica].nome,
//                       classifica[n_elementi_classifica].punteggio);

                //----------------------------------------------------------------//



                //QUESTO IF VERIFICA CHE CI SIANO SUFFICIENTI GRAFI PER RIEMPIRE LA CLASSIFICA.
                //SE IF SODDISFATTO, CLASSIFICA NON PIENA, VA QUINDI AGGIUNTO IL NUOVO ELEMENTO IN CLASSIFICA
                if (contatoregrafi < n_elementi_classifica) {

                    classifica[contatoregrafi].nome = contatoregrafi;
                    classifica[contatoregrafi].punteggio = CalcoloPunteggio(matr_costi, n_node, grafo,
                                                                            priority_queue);
                    if (peggiore == 0) {
                        peggiore = classifica[contatoregrafi].punteggio;
                    }

                    if (peggiore < classifica[contatoregrafi].punteggio) {
                        peggiore = classifica[contatoregrafi].punteggio;
                    }

                }
                    //ENTRO IN QUESTO ELSE SE LA CLASSIFICA È PIENA
                else {

                    classifica[n_elementi_classifica].nome = contatoregrafi;
                    classifica[n_elementi_classifica].punteggio = CalcoloPunteggio(matr_costi, n_node, grafo,
                                                                                   priority_queue);
                    //QUESTA CONDIZIONE VERIFICA CHE IL MIO NUOVO PUNTEGGIO SIA MIGLIORE DEL PUNTEGGIO PEGGIORE IN CLASSIFICA
                    //SE L'IF È SODDISFATTO, ALLORA IL NUOVO ELEMENTO DEVE ESSERE AGGIUNTO ALLA CLASSIFICA
                    if (peggiore > classifica[n_elementi_classifica].punteggio) {
                        MERGESORT(classifica, 0, n_elementi_classifica);
                        peggiore = classifica[n_elementi_classifica].punteggio;
                    }
                }
                contatoregrafi++;
                break;


            case 2:
                //printf("Identificato TopK\n");
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

            case 3:
                //printf("Identificato END\n");
//                free(priority_queue);
//                free(matr_costi);
//                free(classifica);
//                free(grafo);
                return 0;


            default:
                printf("Error in 'identifica comando'\n");
                return -1;
        }
    }
}


void print_classifica(nome_punteggio *classifica, unsigned  long int n_elementi_da_stampare) {

    unsigned  long int i;

    //---------STAMPA DI DEBUG----------//

//        printf("\n\n\n\n\nLa tua classifica: \n");
//        for (i = 0; i < n_elementi_da_stampare; i++) {
//            printf("POSIZIONE %lu\n  Grafo %lu\n  Punteggio: %lu\n", i + 1, classifica[i].nome, classifica[i].punteggio);
//        }
//        printf("\n\n\n");


    //------STAMPA PER PROGETTO------//

    for (i = 0; i < n_elementi_da_stampare - 1; i++) {
        printf("%lu ", classifica[i].nome);
    }
    printf("%lu\n", classifica[i].nome);
}

void copy_classifica(nome_punteggio *dacopiare, nome_punteggio *dovecopiare, unsigned  long int startdacopiare,
                     unsigned long int startdovecopiare, unsigned  long int quanticopiarne) {
    int i;
    for (i = 0; i < quanticopiarne; i++) {
        dovecopiare[startdovecopiare].punteggio = dacopiare[startdacopiare].punteggio;
        dovecopiare[startdovecopiare].nome = dacopiare[startdacopiare].nome;
        startdacopiare++;
        startdovecopiare++;
    }
}

void MERGE(nome_punteggio *classifica, unsigned  long int start, unsigned long int centro,
           unsigned long int final) {
    unsigned long int i = start;
    unsigned long int j = centro + 1;
    unsigned long int k = 0;

    nome_punteggio *temp = (nome_punteggio *) malloc((final + 1) * sizeof(nome_punteggio));

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
    free(temp);

}

void MERGESORT(nome_punteggio *classifica, unsigned  long int start, unsigned  long int final) {
    // start: indice iniziale dell'array
    // final: indice finale dell'array
    // centro: indice intermedio dell'array
    // Divido l'array in 2 sottoarray; Se l'array ha un elemento, è già ordinato e non faccio nulla.
    if (start < final) {
        unsigned  long int centro = (start + final) / 2;
        MERGESORT(classifica, start, centro);
        MERGESORT(classifica, centro + 1, final);
        MERGE(classifica, start, centro, final);
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

void Acquisisci_Mactrice(unsigned long int **matrice, unsigned int dimensione, char *stringa) {

    unsigned long int i, j;
    char *pEnd;

    //------------------------DEBUG------------------------------//

//    for (i = 0; i < dimensione; i++) {
//        for (j = 0; j < dimensione; j++) {
//            stringa = Acquisisci_Stringa(stringa);
//            matrice[i][j] = strtoull(stringa, &pEnd , 10);
//        }
//    }
//
//
//    for (i = 0; i < dimensione; i++) {
//        for (j = 0; j < dimensione - 1; j++) {
//            printf("%lu,", matrice[i][j]);
//        }
//        printf("%lu\n", matrice[i][j]);
//    }



    //-----------------------------------------------------------//

    for (i = 0; i < dimensione; i++) {
        for (j = 0; j < dimensione; j++) {
            matrice[i][j] = strtol(Acquisisci_Stringa(stringa), &pEnd, 10);
        }
    }


}

unsigned long int
CalcoloPunteggio(unsigned long int **matrice, unsigned long int dimensione, nodo *grafo,
                 nodo **priority_queue) {

    unsigned long int i, j, k;
    unsigned long int trovati = 1;
    unsigned long int tot = 0;
    unsigned long int nuovopeso;
    bool cambiato_punteggio = false;


    priority_queue[0] = &(grafo[0]);

    for (k = 0; k < trovati; k++) {
        i = priority_queue[k]->nome;
        for (j = 1; j < dimensione; j++) {
            if (i != j && matrice[i][j] != 0 && !grafo[j].esaminato
                //&& Antenato(grafo, i, j)
                    ) {
                //if (matrice[i][j] != 0) {
                nuovopeso = matrice[i][j] + grafo[i].peso;

                if (nuovopeso < grafo[j].peso) {
                    cambiato_punteggio = true;
                    grafo[j].peso = nuovopeso;
                    grafo[j].nodo_precedente = i;

                    if (grafo[j].raggiunto == false) {
                        grafo[j].raggiunto = true;
                        priority_queue[trovati] = &(grafo[j]);
                        trovati++;
                    }
                }
                //}

            }

            if (cambiato_punteggio == true) {
                //Bubble_Sort_Priority(priority_queue, k + 1, trovati - 1);
                Quick_Sort_Priority(priority_queue, k + 1, trovati - 1);

            }

            cambiato_punteggio = false;

            grafo[i].esaminato = true;
        }
    }


    for (k = 0; k < trovati; k++) {
        tot = tot + priority_queue[k]->peso;
    }



    return tot;
}


bool Antenato(nodo *grafo, unsigned long int ind_analizzare, unsigned  long int ind_prev) {

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

void Quick_Sort_Priority(nodo **priority, unsigned long int start, unsigned  long int final) {

    unsigned long int partitioned;
    if (start < final) {
        partitioned = Partition(priority, start, final);
        Quick_Sort_Priority(priority, start, partitioned - 1);
        Quick_Sort_Priority(priority, partitioned + 1, final);
    }
}

unsigned long int Partition(nodo **priority, unsigned  long int start, unsigned  long int final) {

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

char *Acquisisci_Stringa(char *stringa) {
    char input;
    int count = 0;


    while (1)
        // Ottengo i caratteri uno ad uno fino al '\n' e li inserisco nell'array di 1024 byte
    {
        input = fgetc(stdin);
        if (input == ',' || input == ' ' || input == '\n') {
            break;
        }
        if (input == EOF) {
            return "END";
        }
        stringa[count++] = input;

    }
    stringa[count] = '\0';
    return stringa;
}

void Bubble_Sort_Priority(nodo **priority, unsigned long int start, unsigned long int size){
    unsigned long int i, j;


    for (i = start; i < size - 1; i++){
        for (j = start; j < size - 1; j++){
            if (priority[j]->peso > priority[j+1]->peso){
                nodo *tmp = priority[j+1];
                priority[j+1] = priority[j];
                priority[j] = tmp;
            }
        }
    }

//    for (i = start; i < size; i++){
//        printf("%lu\n", priority[i]->peso);
//    }
//    printf("\n");

}



//------------------------------------------------------REMINISCENZE--------------------------------------------------//