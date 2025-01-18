#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "paciente.h"
#include "fila.h"
#include "learquivo.h"
#include "datahora.h"

#define QTDE_FILAS 6 // O programa simula um sistema de 6 filas

// Estrutura de um evento no escalonador
typedef struct
{
    float tempo; // Tempo do evento (em horas)
    int tipo;    // Tipo do evento (1: triagem, 2: atendimento, etc.)
    Paciente *paciente;
} Evento;

// Estrutura principal do TAD Escalonador
typedef struct
{
    DataHora *dataHoraRef;
    Evento *heap;                          // Array usado como Min-Heap
    int tamanho;                           // Número de eventos no heap
    int capacidade;                        // Capacidade máxima do heap
    Configuracoes *config;                 // Ponteiro para configurações do sistema
    Fila filas[QTDE_FILAS];                // Filas associadas a cada tipo de procedimento
    float tempoUltimoServico[QTDE_FILAS];  // Tempo do último serviço em cada unidade
    float tempoOciosoUnidades[QTDE_FILAS]; // Tempo ocioso acumulado por unidade
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
void inicializaEscalonador(Escalonador *escalonador, DataHora *dataHoraRef, int capacidade, Configuracoes *config);

/**
 * Insere um evento no escalonador
 */
void insereEvento(Escalonador *escalonador, float tempo, int tipo, Paciente *paciente);

/**
 * Remove e retorna o próximo evento do escalonador
 */
Evento retiraProximoEvento(Escalonador *escalonador);

/**
 * Checa se todas as filas estao vazias
 */
int checaFilas(Escalonador *escalonador);

/**
 * Finaliza o escalonador, liberando recursos e gerando estatísticas
 */
void finalizaEscalonador(Escalonador *escalonador);

#endif /* ESCALONADOR_H */
