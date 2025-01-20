#include "../include/escalonador.h"
#include <stdlib.h>
#include <stdio.h>

void trocaEventos(Evento *a, Evento *b)
{
    Evento temp = *a;
    *a = *b;
    *b = temp;
}

int pai(int i) { return (i - 1) / 2; }

int descEsquerdo(int i) { return (2 * i + 1); }

int descDireito(int i) { return (2 * i + 2); }

void sobeHeap(Escalonador *escalonador, int indice)
{
    while (indice != 0 && escalonador->heap[pai(indice)].tempo > escalonador->heap[indice].tempo)
    {
        trocaEventos(&escalonador->heap[indice], &escalonador->heap[pai(indice)]);
        indice = pai(indice);
    }
}

void desceHeap(Escalonador *escalonador, int indice)
{
    int menor = indice;
    int esquerda = descEsquerdo(indice);
    int direita = descDireito(indice);

    if (esquerda < escalonador->tamanho &&
        escalonador->heap[esquerda].tempo < escalonador->heap[menor].tempo)
    {
        menor = esquerda;
    }
    if (direita < escalonador->tamanho &&
        escalonador->heap[direita].tempo < escalonador->heap[menor].tempo)
    {
        menor = direita;
    }
    if (menor != indice)
    {
        trocaEventos(&escalonador->heap[indice], &escalonador->heap[menor]);
        desceHeap(escalonador, menor);
    }
}

void inicializaEscalonador(Escalonador *escalonador, int capacidade, Configuracoes *config)
{
    if (capacidade < 10000)
        capacidade = 10000; // Capacidade inicial mínima

    // Alocar memória para o heap de eventos
    escalonador->heap = (Evento *)malloc(capacidade * sizeof(Evento));
    erroAssert(escalonador->heap != NULL, "Erro ao alocar memória para o escalonador.");

    escalonador->tamanho = 0;
    escalonador->capacidade = capacidade;
    escalonador->config = config;

    inicializaProcedimento(&escalonador->procedimentoTR, config->numTriagem, config->tempoTriagem);
    inicializaProcedimento(&escalonador->procedimentoAT, config->numAtendimento, config->tempoAtendimento);
    inicializaProcedimento(&escalonador->procedimentoMH, config->numMH, config->tempoMH);
    inicializaProcedimento(&escalonador->procedimentoTL, config->numTL, config->tempoTL);
    inicializaProcedimento(&escalonador->procedimentoEI, config->numEI, config->tempoEI);
    inicializaProcedimento(&escalonador->procedimentoIM, config->numIM, config->tempoIM);

    // Inicializar a fila de triagem
    inicializaFila(&escalonador->filaTr);

    // Inicializar as filas de atendimento e de procedimentos
    for (int i = 0; i < 3; i++)
    {
        inicializaFila(&escalonador->filasAt[i]);   // Filas de atendimento
        inicializaFila(&escalonador->filasProc[i]); // Filas de procedimentos
    }
}

void insereEvento(Escalonador *escalonador, float tempo, int indiceAux, Paciente *paciente)
{
    erroAssert(tempo >= 0, "Tempo inválido ao inserir evento.");
    erroAssert(indiceAux >= 0, "indiceAux de evento inválido.");

    if (escalonador->tamanho == escalonador->capacidade)
    {
        escalonador->capacidade += 10000;
        escalonador->heap = (Evento *)realloc(escalonador->heap, escalonador->capacidade * sizeof(Evento));
        erroAssert(escalonador->heap != NULL, "Erro ao realocar memória para o escalonador.");
    }

    // Insere o novo evento no final do heap
    escalonador->heap[escalonador->tamanho].tempo = tempo;
    escalonador->heap[escalonador->tamanho].indiceAux = indiceAux;
    escalonador->heap[escalonador->tamanho].paciente = paciente;

    // Incrementa o tamanho do heap e corrige a ordem
    escalonador->tamanho++;
    sobeHeap(escalonador, escalonador->tamanho - 1);
}

Evento retiraProximoEvento(Escalonador *escalonador)
{
    erroAssert(escalonador->tamanho > 0, "Tentativa de retirar evento de um escalonador vazio.");

    // obtem o proximo evento (raiz do minheap)
    Evento raiz = escalonador->heap[0];
    escalonador->heap[0] = escalonador->heap[escalonador->tamanho - 1];
    escalonador->tamanho--;
    desceHeap(escalonador, 0);

    return raiz;
}

int checaFilas(Escalonador *escalonador)
{
    // Verifica a fila de triagem
    if (!filaVazia(&escalonador->filaTr))
    {
        return 0;
    }

    // Verifica as filas de atendimento
    for (int i = 0; i < QTDE_FILAS; i++)
    {
        if (!filaVazia(&escalonador->filasAt[i]))
        {
            return 0;
        }
    }

    // Verifica as filas de procedimentos
    for (int i = 0; i < QTDE_FILAS; i++)
    {
        if (!filaVazia(&escalonador->filasProc[i]))
        {
            return 0;
        }
    }

    // Todas as filas estão vazias
    return 1;
}

void finalizaEscalonador(Escalonador *escalonador)
{
    erroAssert(escalonador != NULL, "Tentativa de finalizar um escalonador nulo.");

    // Libera o heap de eventos
    if (escalonador->heap != NULL)
    {
        free(escalonador->heap);
        escalonador->heap = NULL;
    }

    // Finaliza a fila de triagem
    finalizaFila(&escalonador->filaTr);

    // Finaliza as filas de atendimento e de procedimentos
    for (int i = 0; i < 3; i++) // 3 filas de atendimento e 3 filas de procedimentos
    {
        finalizaFila(&escalonador->filasAt[i]);   // Filas de atendimento
        finalizaFila(&escalonador->filasProc[i]); // Filas de procedimentos
    }

    // Finaliza os procedimentos
    finalizaProcedimento(&escalonador->procedimentoTR);
    finalizaProcedimento(&escalonador->procedimentoAT);
    finalizaProcedimento(&escalonador->procedimentoMH);
    finalizaProcedimento(&escalonador->procedimentoTL);
    finalizaProcedimento(&escalonador->procedimentoEI);
    finalizaProcedimento(&escalonador->procedimentoIM);

    // Zera os campos do escalonador
    escalonador->tamanho = 0;
    escalonador->capacidade = 0;
    escalonador->config = NULL;
}

