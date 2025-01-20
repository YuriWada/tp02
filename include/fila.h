#ifndef FILA_H
#define FILA_H

#include "paciente.h"

#include <stdio.h>
#include <stdlib.h>

// Estrutura de um no da Fila
typedef struct NoFila
{
    Paciente *paciente;     // ponteiro para o Paciente
    float tempoEntrada;     // tempo em que o paciente entrou na fila
    struct NoFila *proximo; // ponteiro para o próximo nó
} NoFila;

// TAD Fila conforme especificação do TP2
typedef struct
{
    NoFila *inicio;          // ponteiro para início da fila
    NoFila *fim;             // ponteiro para o final da fila
    int tamanho;             // número de pacientes na fila
    float tempoTotalEspera;  // tempo total de espera dos pacientes
    float somaTamanhosFila;  // soma dos tamanhos da fila ao longo do tempo
    int amostrasTamanhoFila; // número de amostras para calcular o tamanho médio
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
 * Remove um paciente do início da fila
 */
Paciente *desenfileira(Fila *fila);

/**
 * Busca o nó correspondente a um paciente na fila, com base no ID.
 * Retorna o nó correspondente ou NULL, se não encontrado.
 */
NoFila *buscaNoPorPaciente(Fila *fila, int idPaciente);

/**
 * Verifica se a fila está vazia
 */
int filaVazia(Fila *fila);

/**
 * Finaliza a fila
 */
void finalizaFila(Fila *fila);

#endif /* FILA_H */
