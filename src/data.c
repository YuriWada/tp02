#include "../include/data.h"

// Definição única de nomeMeses
const char* nomeMeses[] = {
    "", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

int inicializaData(Data *data, int dia, meses mes, int ano, float hora)
{
    erroAssert(data != NULL, "Ponteiro nulo para Data.");
    erroAssert(hora >= 0.0f && hora < 24.0f, "Hora inválida.");
    erroAssert(mes >= Jan && mes <= Dec, "Mês inválido.");

    data->dia = dia;
    data->mes = mes;
    data->ano = ano;
    data->hora = hora;

    return validaData(data); // Retorna 1 se a data for válida, 0 caso contrário
}

int validaData(const Data *data)
{
    erroAssert(data != NULL, "Ponteiro nulo para Data.");

    // Dias por mês (não considerando anos bissextos ainda)
    int diasPorMes[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Considera ano bissexto
    if ((data->ano % 4 == 0 && data->ano % 100 != 0) || (data->ano % 400 == 0)) {
        diasPorMes[Feb] = 29;
    }

    // Validação do dia
    if (data->dia < 1 || data->dia > diasPorMes[data->mes]) return 0;

    return 1; // Data válida
}

int calculaDiaSemana(const Data *data)
{
    erroAssert(data != NULL, "Ponteiro nulo para Data.");

    int dia = data->dia;
    int mes = data->mes;
    int ano = data->ano;

    if (mes == Jan || mes == Feb) {
        mes += 12;
        ano -= 1;
    }

    int K = ano % 100; // Últimos 2 dígitos do ano
    int J = ano / 100; // Primeiros 2 dígitos do ano

    // Algoritmo de Zeller para calcular o dia da semana
    int h = (dia + (13 * (mes + 1)) / 5 + K + (K / 4) + (J / 4) - 2 * J) % 7;

    // Ajuste para valores positivos e formato desejado (0 = domingo)
    return (h + 5) % 7; 
}

const char *obtemDiaSemana(int diaSemana)
{
    const char *dias[] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };

    return dias[diaSemana];
}

const char *obtemNomeMes(meses mes)
{
    erroAssert(mes >= Jan && mes <= Dec, "Mês inválido.");
    return nomeMeses[mes];
}

const char *transformaHora(const Data *data)
{
    erroAssert(data != NULL, "Ponteiro nulo para Data.");

    int hora = (int)data->hora;                                        // Parte inteira da hora
    int minutos = (int)((data->hora - hora) * 60);                     // Parte fracionária para minutos
    int segundos = (int)((((data->hora - hora) * 60) - minutos) * 60); // Resto para segundos

    // Buffer estático para armazenar a string formatada
    static char buffer[9];
    sprintf(buffer, "%02d:%02d:%02d", hora, minutos, segundos);

    return buffer;
}
