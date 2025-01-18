#ifndef DATAHORA_H
#define DATAHORA_H

#include "paciente.h"
#include <stdio.h>

// Estrutura que representa uma data e hora
typedef struct {
    int ano;
    int mes;
    int dia;
    float hora; // Representa a hora com frações (ex: 14.5 = 14h30min)
} DataHora;

/**
 * Inicializa o TAD DataHora
 */
void inicializaDataHora(DataHora *dataHora, Paciente paciente);

/**
 * Verifica se um ano é bissexto.
 * @param ano Ano a ser verificado.
 * @return 1 se for bissexto, 0 caso contrário.
 */
int ehBissexto(int ano);

/**
 * Obtém o número de dias em um mês de um determinado ano.
 * @param mes Mês desejado.
 * @param ano Ano correspondente.
 * @return Número de dias no mês.
 */
int diasNoMes(int mes, int ano);

/**
 * Converte uma data no formato `DataHora` em horas a partir de uma data de referência.
 * @param referencia Data de referência.
 * @param entrada Data de entrada.
 * @return Horas entre as duas datas.
 */
float dataParaHoras(DataHora *referencia, DataHora entrada);

/**
 * Converte horas em uma data no formato `DataHora` a partir de uma data de referência.
 * @param horas Horas a serem convertidas.
 * @param referencia Data de referência.
 * @param resultado Ponteiro para armazenar a data resultante.
 */
void horasParaData(float horas, DataHora referencia, DataHora *resultado);

#endif /* DATAHORA_H */
