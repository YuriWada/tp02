#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "paciente.h"
#include "fila.h"
#include "learquivo.h"
#include "datahora.h"
#include "procedimento.h"

#define QTDE_FILAS 3 // O programa simula um sistema de 6 filas

// Estrutura de um evento no escalonador
typedef struct
{
    float tempo; // Tempo do evento (em horas)
    int indiceAux;    // indiceAux do evento (1: fila triagem, 2: triagem, 3: fila atendimento, 4: atendimento etc.)
    Paciente *paciente;
} Evento;

// Estrutura principal do TAD Escalonador
typedef struct
{
    Evento *heap;                          // Array usado como Min-Heap
    int tamanho;                           // Número de eventos no heap
    int capacidade;                        // Capacidade máxima do heap
    Configuracoes *config;                 // Ponteiro para configurações do sistema
    Fila filaTr;
    Fila filasAt[QTDE_FILAS];              // Filas associadas a triagem
    Fila filasProc[QTDE_FILAS];
    Procedimento procedimentoTR;
    Procedimento procedimentoAT;
    Procedimento procedimentoMH;
    Procedimento procedimentoTL;
    Procedimento procedimentoEI;
    Procedimento procedimentoIM;
} Escalonador;

/**
 * Funcoes do min-heap
 */
void trocaEventos(Evento *a, Evento *b);
int pai(int i);
int descEsquerdo(int i);
int descDireito(int i);
void sobeHeap(Escalonador *escalonador, int indice);
void desceHeap(Escalonador *escalonador, int indice);

/**
 * Inicializa o escalonador com uma capacidade inicial
 */
void inicializaEscalonador(Escalonador *escalonador, int capacidade, Configuracoes *config);

/**
 * Insere um evento no escalonador
 * @param escalonador escalonador
 * @param tempo datahora do paciente
 * @param indiceAux indice auxiliar
 * @param paciente paciente associado ao evento escalonado
 */
void insereEvento(Escalonador *escalonador, float tempo, int indiceAux, Paciente *paciente);

/**
 * Remove e retorna o próximo evento do escalonador
 */
Evento retiraProximoEvento(Escalonador *escalonador);

/**
 * Checa se todas as filas estao vazias
 */
int checaFilas(Escalonador *escalonadora);

/**
 * Finaliza o escalonador, liberando recursos e gerando estatísticas
 */
void finalizaEscalonador(Escalonador *escalonador);

#endif /* ESCALONADOR_H */
