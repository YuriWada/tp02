#ifndef LEARQUIVO_H
#define LEARQUIVO_H

#include "paciente.h"

// Configurações gerais da simulação
typedef struct {
    int tempoTriagem;
    int numTriagem;
    int tempoAtendimento;
    int numAtendimento;
    int tempoMH;
    int numMH;
    int tempoTL;
    int numTL;
    int tempoEI;
    int numEI;
    int tempoIM;
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
