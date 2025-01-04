#include "../include/escalonador.h"
#include <stdlib.h>
#include <stdio.h>

void trocaEventos(Evento *a, Evento *b)
{
    Evento temp = *a;
    *a = *b;
    *b = temp;
}

void sobeHeap(Escalonador *escalonador, int indice)
{
    while (indice > 0)
    {
        int pai = (indice - 1) / 2;
        if (escalonador->heap[indice].tempo >= escalonador->heap[pai].tempo)
        {
            break;
        }
        trocaEventos(&escalonador->heap[indice], &escalonador->heap[pai]);
        indice = pai;
    }
}

void desceHeap(Escalonador *escalonador, int indice)
{
    int menor = indice;
    int esquerda = 2 * indice + 1;
    int direita = 2 * indice + 2;

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
    escalonador->heap = (Evento *)malloc(capacidade * sizeof(Evento));
    erroAssert(escalonador->heap != NULL, "Erro ao alocar memória para o escalonador.");
    escalonador->tamanho = 0;
    escalonador->capacidade = capacidade;
    escalonador->config = config;

    for (int i = 0; i < 5; i++)
    {
        inicializaFila(&escalonador->filas[i]);
    }
}

void insereEvento(Escalonador *escalonador, float tempo, int tipo, Paciente *paciente)
{
    if (escalonador->tamanho == escalonador->capacidade)
    {
        escalonador->capacidade *= 2;
        escalonador->heap = (Evento *)realloc(escalonador->heap, escalonador->capacidade * sizeof(Evento));
        erroAssert(escalonador->heap != NULL, "Erro ao realocar memória para o escalonador.");
    }

    escalonador->heap[escalonador->tamanho].tempo = tempo;
    escalonador->heap[escalonador->tamanho].tipo = tipo;
    escalonador->heap[escalonador->tamanho].paciente = paciente;

    sobeHeap(escalonador, escalonador->tamanho);
    escalonador->tamanho++;
}

Evento retiraProximoEvento(Escalonador *escalonador)
{
    erroAssert(escalonador->tamanho > 0, "Tentativa de retirar evento de um escalonador vazio.");

    Evento proximo = escalonador->heap[0];
    escalonador->heap[0] = escalonador->heap[--escalonador->tamanho];
    desceHeap(escalonador, 0);

    return proximo;
}

void processaEventos(Escalonador *escalonador)
{
    while (escalonador->tamanho > 0)
    {
        Evento evento = retiraProximoEvento(escalonador);
        Paciente *paciente = evento.paciente;

        // Atualiza o estado do paciente
        atualizaEstado(paciente, evento.tipo);

        switch (evento.tipo)
        {
        case 1: // Triagem
            registraTempoAtendimento(paciente, escalonador->config->tempoTriagem);
            insereEvento(escalonador, evento.tempo + escalonador->config->tempoTriagem, 2, paciente);
            break;

        case 2: // Atendimento
            registraTempoAtendimento(paciente, escalonador->config->tempoAtendimento);

            // Adiciona eventos para os procedimentos subsequentes
            if (paciente->medidasHospitalares > 0)
            {
                insereEvento(escalonador, evento.tempo + escalonador->config->tempoAtendimento, 3, paciente);
            }
            else if (paciente->testesLaboratorio > 0)
            {
                insereEvento(escalonador, evento.tempo + escalonador->config->tempoAtendimento, 4, paciente);
            }
            else if (paciente->examesImagem > 0)
            {
                insereEvento(escalonador, evento.tempo + escalonador->config->tempoAtendimento, 5, paciente);
            }
            else
            {
                // Paciente liberado após atendimento
                calculaSaida(paciente);
                paciente->estadoAtual = ALTA_HOSP;
            }
            break;

        case 3: // Medidas Hospitalares
            registraTempoAtendimento(paciente, escalonador->config->tempoMH);
            insereEvento(escalonador, evento.tempo + escalonador->config->tempoMH, 4, paciente);
            break;

        case 4: // Testes Laboratoriais
            registraTempoAtendimento(paciente, escalonador->config->tempoTL);
            insereEvento(escalonador, evento.tempo + escalonador->config->tempoTL, 5, paciente);
            break;

        case 5: // Exames de Imagem
            registraTempoAtendimento(paciente, escalonador->config->tempoEI);

            // Paciente liberado após último procedimento
            calculaSaida(paciente);
            paciente->estadoAtual = ALTA_HOSP;
            break;
        }
    }
}

void finalizaEscalonador(Escalonador *escalonador)
{
    for (int i = 0; i < 5; i++)
    {
        finalizaFila(&escalonador->filas[i]);
    }
    free(escalonador->heap);
    escalonador->heap = NULL;
    escalonador->tamanho = 0;
    escalonador->capacidade = 0;
}
