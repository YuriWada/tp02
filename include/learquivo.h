#ifndef LEARQUIVO_H
#define LEARQUIVO_H

#include "paciente.h"

// Configurações gerais da simulação
typedef struct {
    float tempoTriagem;
    int numTriagem;
    float tempoAtendimento;
    int numAtendimento;
    float tempoMH;
    int numMH;
    float tempoTL;
    int numTL;
    float tempoEI;
    int numEI;
    float tempoIM;
    int numIM;
    int numPacientes;
} Configuracoes;

// Estrutura principal do LeitorArquivo
typedef struct {
    Configuracoes configuracoes;
    Paciente *pacientes; // Array dinâmico de pacientes
} LeitorArquivo;

/**
 * Inicializa o leitor de arquivo e carrega os dados
 */
int inicializaLeitorArquivo(LeitorArquivo *leitor, const char *nomeArquivo);

/**
 * Libera os recursos alocados pelo leitor de arquivo
 */
void liberaLeitorArquivo(LeitorArquivo *leitor);

#endif /* LEARQUIVO_H */
