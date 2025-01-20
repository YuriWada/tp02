#ifndef PROCEDIMENTO_H
#define PROCEDIMENTO_H

#include "datahora.h" // Supondo que datahora gerencia tempos

typedef struct
{
    int id;                   // Identificador único da unidade
    int ocupado;              // 1 se ocupado, 0 se livre
    float tempoUltimoServico; // Tempo (em horas) até o qual a unidade está ocupada
    float tempoOcioso;        // Tempo total que a unidade ficou ociosa
} Unidade;

typedef struct
{
    Unidade *unidades;        // Array de unidades para este procedimento
    int numUnidades;          // Número total de unidades
    float tempoMedioExecucao; // Tempo médio de execução do serviço (em horas)
} Procedimento;

/**
 * Inicializa um procedimento com as unidades especificadas.
 * @param procedimento Ponteiro para o procedimento a ser inicializado.
 * @param numUnidades Número de unidades do procedimento.
 * @param tempoMedioExecucao Tempo médio de execução do serviço (em horas).
 */
void inicializaProcedimento(Procedimento *procedimento, int numUnidades, float tempoMedioExecucao);

/**
 * Libera os recursos alocados pelo procedimento.
 * @param procedimento Ponteiro para o procedimento a ser finalizado.
 */
void finalizaProcedimento(Procedimento *procedimento);

/**
 * Encontra uma unidade disponível no procedimento.
 * @param procedimento Ponteiro para o procedimento.
 * @param tempoAtual Tempo atual da simulação (em horas).
 * @return O índice da unidade disponível ou -1 se nenhuma unidade estiver disponível.
 */
int encontraUnidadeDisponivel(Procedimento *procedimento, float tempoAtual);

/**
 * Aloca uma unidade para um serviço.
 * @param procedimento Ponteiro para o procedimento.
 * @param indice Índice da unidade a ser alocada.
 * @param tempoAtual Tempo atual da simulação (em horas).
 * @param duracaoServico Duração do serviço (em horas).
 */
void alocaUnidade(Procedimento *procedimento, int indice, float tempoAtual, float duracaoServico);

/**
 * Libera uma unidade após o término do serviço.
 * @param procedimento Ponteiro para o procedimento.
 * @param indice Índice da unidade a ser liberada.
 * @param tempoAtual Tempo atual da simulação (em horas).
 */
void liberaUnidade(Procedimento *procedimento, int indice, float tempoAtual);

#endif /* PROCEDIMENTO_H */
