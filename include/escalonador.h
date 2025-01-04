#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "paciente.h"
#include "fila.h"
#include "learquivo.h"

// Estrutura de um evento no escalonador
typedef struct
{
    float tempo;        // Tempo do evento (em horas)
    int tipo;           // Tipo do evento (1: triagem, 2: atendimento, etc.)
    Paciente *paciente; // Ponteiro para o paciente associado
} Evento;

// Estrutura principal do TAD Escalonador
typedef struct
{
    Evento *heap;          // Array usado como Min-Heap
    int tamanho;           // Número de eventos no heap
    int capacidade;        // Capacidade máxima do heap
    Configuracoes *config; // Ponteiro para configurações do sistema
    Fila filas[5];         // Filas associadas a cada tipo de procedimento
} Escalonador;

/**
 * Inicializa o escalonador com uma capacidade inicial
 */
void inicializaEscalonador(Escalonador *escalonador, int capacidade, Configuracoes *config);

/**
 * Insere um evento no escalonador
 */
void insereEvento(Escalonador *escalonador, float tempo, int tipo, Paciente *paciente);

/**
 * Remove e retorna o próximo evento do escalonador
 */
Evento retiraProximoEvento(Escalonador *escalonador);

/**
 * Processa eventos no escalonador
 */
void processaEventos(Escalonador *escalonador);

/**
 * Finaliza o escalonador, liberando recursos e gerando estatísticas
 */
void finalizaEscalonador(Escalonador *escalonador);

#endif /* ESCALONADOR_H */
