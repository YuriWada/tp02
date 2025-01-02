
/**
 * Implementacao de TAD auxiliar para leitura de arquivo
 */
#ifndef LEARQUIVO_H
#define LEARQUIVO_H

#include "msgassert.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Configuracoes gerais do Sistema de Escalonamento Hospitalar
 */
typedef struct
{
    float _tempoTriagem;
    int _triagem;
    float _tempoAtendimento;
    int _atendimento;
    float _tempoMH;
    int _MH; // numero de Medidas Hospitalares (MH)
    float _tempoTL;
    int _TL; // numero de Testes Laboratoriais (TL)
    float _tempoEI;
    int _EI; // numero de Exames de Imagem (EI)
    float _tempoIM;
    int _IM; // numero de Instrumentos/Medicamentos (IM)
    int _QuantidadePacientes;
} Config;

typedef struct
{
    Config cfg;
    // Paciente pc;
} LeArquivo;

int inicializaLeArquivo(LeArquivo *leitor, const char *nomeArquivo);
void liberaLeArquivo(LeArquivo *leitor);
void imprimeDados(LeArquivo *leitor);

#endif /* LEARQUIVO_H */