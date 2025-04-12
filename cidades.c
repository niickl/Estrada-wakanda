#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cidades.h"

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    Estrada *novaEstrada = (Estrada *)malloc(sizeof(Estrada));
    if (novaEstrada == NULL) {
        perror("Erro ao alocar memória para a estrutura Estrada.");
        fclose(arquivo);
        return NULL;
    }

    fscanf(arquivo, "%d", &novaEstrada->T);
    fscanf(arquivo, "%d", &novaEstrada->N);

    if (novaEstrada->T < 3 || novaEstrada->T > 1000000 ||
        novaEstrada->N < 2 || novaEstrada->N > 10000) {
        perror("Erro: T ou N fora dos limites permitidos.");
        free(novaEstrada);
        fclose(arquivo);
        return NULL;
    }

    novaEstrada->C = (Cidade *)malloc(novaEstrada->N * sizeof(Cidade));
    if (novaEstrada->C == NULL) {
        perror("Erro ao alocar memória para o vetor de cidades.");
        free(novaEstrada);
        fclose(arquivo);
        return NULL;
    }

    for (int i = 0; i < novaEstrada->N; i++) {
        fscanf(arquivo, "%d", &novaEstrada->C[i].Posicao);
        fscanf(arquivo, " %[^\n]", novaEstrada->C[i].Nome);

        if (novaEstrada->C[i].Posicao <= 0 || novaEstrada->C[i].Posicao >= novaEstrada->T) {
            perror("Erro: Posição da cidade fora dos limites permitidos.");
            free(novaEstrada->C);
            free(novaEstrada);
            fclose(arquivo);
            return NULL;
        }

        for (int j = 0; j < i; j++) {
            if (novaEstrada->C[i].Posicao == novaEstrada->C[j].Posicao) {
                perror("Erro: Duas cidades possuem a mesma posição.");
                free(novaEstrada->C);
                free(novaEstrada);
                fclose(arquivo);
                return NULL;
            }
        }
    }

    fclose(arquivo);
    return novaEstrada;
}

int compararCidades(const void *a, const void *b) {
    Cidade *cidadeA = (Cidade *)a;
    Cidade *cidadeB = (Cidade *)b;

    if (cidadeA->Posicao < cidadeB->Posicao) 
        return -1;
    else if (cidadeA->Posicao > cidadeB->Posicao) 
        return 1;
    else 
        return 0;
}

double calcularVizinhancaParaCidade(Estrada *estrada, int i) {

    if (i == 0) {
        return ((double)estrada->C[0].Posicao + estrada->C[1].Posicao) / 2.0;
    }

    else if (i == estrada->N - 1) {
        return estrada->T - (((double)estrada->C[estrada->N - 2].Posicao + estrada->C[estrada->N - 1].Posicao) / 2.0);
    }

    else {
        return ((double)estrada->C[i + 1].Posicao - estrada->C[i - 1].Posicao) / 2.0;
    }
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        perror("Erro ao inicializar getEstrada");
        return -1; 
    }

    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);

    double menorVizinhanca = calcularVizinhancaParaCidade(estrada, 0);

    for (int i = 1; i < estrada->N; i++) {
        double viz = calcularVizinhancaParaCidade(estrada, i);
        if (viz < menorVizinhanca) {
            menorVizinhanca = viz;
        }
    }

    free(estrada->C);
    free(estrada);
    return menorVizinhanca;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (estrada == NULL) {
        printf("Erro ao carregar os dados da estrada.\n");
        return NULL;
    }

    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);

    double menorVizinhanca = calcularVizinhancaParaCidade(estrada, 0);
    char *nomeCidadeMenor = estrada->C[0].Nome;

    for (int i = 1; i < estrada->N; i++) {
        double viz = calcularVizinhancaParaCidade(estrada, i);
        if (viz < menorVizinhanca) {
            menorVizinhanca = viz;
            nomeCidadeMenor = estrada->C[i].Nome;
        }
    }

    char *resultado = (char *)malloc(strlen(nomeCidadeMenor) + 1);
    if (resultado != NULL) {
        strcpy(resultado, nomeCidadeMenor);
    }

    free(estrada->C);
    free(estrada);
    return resultado;
}
