#ifndef FILA_H
#define FILA_H

#include "paciente.h"

#include <stdio.h>
#include <stdlib.h>

// Estrutura de um no da Fila
typedef struct NoFila
{
    Paciente *paciente;     // ponteiro para o Paciente
    float tempoEntrada;     // tempo em que o paciente entrou na filha
    struct NoFila *proximo; // ponteiro para o proximo no
} NoFila;

// TAD Fila conforme especificacao do TP2
typedef struct
{
    NoFila *inicio;          // ponteiro para inicio da fila
    NoFila *fim;             // ponteiro para o final da fila
    int tamanho;             // numero de pacientes na fila
    float tempoTotalEspera;  // tempo total de espera dos pacientes
    float somaTamanhosFila;  // soma dos tamanhos da fila ao longo do tempo
    int amostrasTamanhoFila; // numero de amostras para calcular o tamanho medio
} Fila;

/**
 * Inicializa uma fila vazia
 */
void inicializaFila(Fila *fila);

/**
 * Adiciona um paciente ao final da fila
 */
void enfileira(Fila *fila, Paciente *paciente, float tempoAtual);

/**
 * Remove um paciente do inicio da fila
 */
Paciente *desenfileira(Fila *fila, float tempoATual);

/**
 * Verifica se a fila esta vazia
 */
int filaVazia(Fila *fila);

/**
 * Finaliza a fila
 */
void finalizaFila(Fila *fila);

#endif /* FILA_H */