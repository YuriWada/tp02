/**
 * Implementacao do TAD Paciente
 */
#ifndef PACIENTE_H
#define PACIENTE_H

#include "msgassert.h"
#include "data.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * TAD com os estados do paciente
 */
typedef enum
{
    NAO_CHEGOU,
    FILA_TRIAGEM,
    SENDO_TRIADO,
    FILA_ATENDIMENTO,
    SENDO_ATENDIDO,
    FILA_MH, // Na fila de Medidas Hospitalares
    REALIZANDO_MH,
    FILA_TL, // Na fila de Testes de Laboratorio
    REALIZANDO_TL,
    FILA_EI, // Na fila de Exames de Imagem
    REALIZANDO_EI,
    FILA_IM, // Na fila de Instrumentos/Medicamentos
    REALIZANDO_IM,
    ALTA_HOSP
} EstadoPaciente;

// TAD que representa o paciente e suas estatisticas
typedef struct
{
    int id;
    int alta;
    int ano, mes, dia;
    float hora;
    int urgencia;
    int medidasHospitalares;
    int testesLaboratorio;
    int examesImagem;
    int instrumentosMedicamentos;

    // Variaveis para obter estatistica paciente
    EstadoPaciente estadoAtual;
    float tempoEspera; // calcula o tempo total nas filas
    float tempoAtendimento; // calcula o tempo total em que o paciente foi atendido

    int anoSaida, mesSaida, diaSaida;
    float horaSaida;
} Paciente;


// TAD auxiliar para transformacao da data de entrada
typedef struct
{
    const char* diaSemana;
    const char* nomeMes;
} DiaMes;

// Inicializa as variaveis do paciente
void inicializaPaciente(Paciente *paciente, int id, int alta, int ano, int mes, int dia, float hora, 
                        int urgencia, int medidasH, int testesL, int examesI, int instrumentosM);

/**
 * Atualiza o estado atual do paciente a cada novo evento
 */
void atualizaEstado(Paciente *paciente, EstadoPaciente novoEstado);

/**
 * Registra o tempo de espera do paciente em cada fila
 */
void registraTempoEspera(Paciente *paciente, float tempo);

/**
 * Registra tempo de atendimento do paciente em cada atendimento
 */
void registraTempoAtendimento(Paciente *paciente, float tempo);

/**
 * Transforma a data de entrada e saida do paciente no formato especificado pelo TP
 */
DiaMes defineDataString(Paciente *paciente);

/**
 * Registra o novo horario de saida do paciente, com base no tempo que permaneceu no HZ
 */
void calculaSaida(Paciente *paciente);

/**
 * Imprime as informacoes do paciente conforme output especificado pelo TP
 */
void imprimePaciente(const Paciente *paciente);

#endif /* PACIENTE_H */