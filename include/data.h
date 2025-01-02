/**
 * TAD auxiliar para retornar o dia da semana e mes em string
 */
#ifndef DATA_H
#define DATA_H

#include "msgassert.h"

#include <stdio.h>
#include <stdlib.h>

// Enum para retornar os valores correspondentes aos meses
typedef enum
{
    jan = 1, feb, mar, apr, may, jun,
    jul, aug, sep, oct, nov, dec
} meses;

// TAD que representa as datas
typedef struct
{
    int dia;
    meses mes; // meses do ano com base no enum "meses"
    int ano;
} Data;

/**
 * Inicializa as variaveis de data e verifica a validade 
 * das datas de entrada
 */
int inicializaData(Data *data, int dia, meses mes, int ano);

/**
 * Valida a data conforme dia, mes e ano (bissexto)
 */
int validaData(const Data *Data);

/**
 * Calcula o dia da semana usando Algoritmo de Zeller
 */
int calculaDiaSemana(const Data *data);

/**
 * Retorna o dia da semana em char
 * Os dias estao abreviados em ingles
 */
const char* getDiaSemana(int diaSemana);

#endif /* DATA_H */