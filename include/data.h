#ifndef DATA_H
#define DATA_H

#include "msgassert.h"

#include <stdio.h>
#include <stdlib.h>

// Enum com os valores correspondentes aos meses
typedef enum
{
    Jan = 1, Feb, Mar, Apr, May, Jun,
    Jul, Aug, Sep, Oct, Nov, Dec
} meses;

// Array que corresponde às chaves do enum "meses"
extern const char* nomeMeses[];

// TAD que representa as datas
typedef struct
{
    int dia;
    meses mes; // meses do ano com base no enum "meses"
    int ano;
    float hora; // armazena a hora de entrada/saida
} Data;

/**
 * Inicializa as variáveis de data e verifica a validade 
 * das datas de entrada.
 * Retorna 1 para sucesso ou 0 para falha.
 */
int inicializaData(Data *data, int dia, meses mes, int ano, float hora);

/**
 * Valida a data conforme dia, mês e ano (bissexto).
 */
int validaData(const Data *data);

/**
 * Calcula o dia da semana usando Algoritmo de Zeller.
 */
int calculaDiaSemana(const Data *data);

/**
 * Retorna o dia da semana em formato abreviado.
 */
const char* obtemDiaSemana(int diaSemana);

/**
 * Retorna o nome do mês a partir do enum.
 */
const char* obtemNomeMes(meses mes);

/**
 * Transforma a hora para o formato hh:mm:ss.
 */
const char* transformaHora(const Data *data);

#endif /* DATA_H */
