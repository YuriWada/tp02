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
    while (indice != 0 && &escalonador->heap[pai(indice)].tempo > &escalonador->heap[indice].tempo)
    {
        trocaEventos(&escalonador->heap[indice].tempo, &escalonador->heap[pai(indice)].tempo);
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
        trocaEventos(&escalonador->heap[indice].tempo, &escalonador->heap[menor].tempo);
        desceHeap(escalonador, menor);
    }
}

void inicializaEscalonador(Escalonador *escalonador, DataHora *dataHoraRef, int capacidade, Configuracoes *config)
{
    escalonador->dataHoraRef = dataHoraRef;
    if (capacidade < 10000)
        capacidade = 10000; // Capacidade inicial mínima

    escalonador->heap = (Evento *)malloc(capacidade * sizeof(Evento));
    erroAssert(escalonador->heap != NULL, "Erro ao alocar memória para o escalonador.");

    escalonador->tamanho = 0;
    escalonador->capacidade = capacidade;
    escalonador->config = config;

    for (int i = 0; i < 6; i++)
    {
        inicializaFila(&escalonador->filas[i]);
        escalonador->tempoUltimoServico[i] = 0.0f;
        escalonador->tempoOciosoUnidades[i] = 0.0f;
    }
}

void insereEvento(Escalonador *escalonador, float tempo, int tipo, Paciente *paciente)
{
    erroAssert(tempo >= 0, "Tempo inválido ao inserir evento.");
    erroAssert(tipo >= 1 && tipo <= 6, "Tipo de evento inválido.");

    if (escalonador->tamanho == escalonador->capacidade)
    {
        escalonador->capacidade += 10000;
        escalonador->heap = (Evento *)realloc(escalonador->heap, escalonador->capacidade * sizeof(Evento));
        erroAssert(escalonador->heap != NULL, "Erro ao realocar memória para o escalonador.");
    }

    escalonador->tamanho++;
    escalonador->heap[escalonador->tamanho-1].tempo = tempo;
    escalonador->heap[escalonador->tamanho-1].tipo = tipo;
    escalonador->heap[escalonador->tamanho-1].paciente = paciente;

    sobeHeap(escalonador, escalonador->tamanho-1);
}

Evento retiraProximoEvento(Escalonador *escalonador)
{
    erroAssert(escalonador->tamanho > 0, "Tentativa de retirar evento de um escalonador vazio.");

    // obtem o proximo evento (raiz do minheap)
    Evento raiz = escalonador->heap[0];
    escalonador->heap[0] = escalonador->heap[escalonador->tamanho-1];
    escalonador->tamanho--;
    desceHeap(escalonador, 0);

    return raiz;
}

int checaFilas(Escalonador *escalonador)
{
    for(int i = 0; i < 6; i++)
    {
        if (filaVazia(&escalonador->filas[i]) == 0) return 0;
    }
    return 1;
}

void finalizaEscalonador(Escalonador *escalonador)
{
    for (int i = 0; i < QTDE_FILAS; i++)
    {
        finalizaFila(&escalonador->filas[i]);
    }
    free(escalonador->heap);
    escalonador->heap = NULL;
    escalonador->tamanho = 0;
    escalonador->capacidade = 0;
}
