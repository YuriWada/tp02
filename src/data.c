#include "../include/data.h"

int inicializaData(Data *data, int dia, meses mes, int ano)
{
    // Verificacao de datas de entrada
    erroAssert(dia>0, "Data invalida!");
    erroAssert(dia<=31, "Data invalida!");
    erroAssert(mes>0, "Data invalida!");
    erroAssert(mes<=12, "Data invalida!");

    data->dia = dia;
    data->mes = mes;
    data->ano = ano;

    return validaData(data);
}

int validaData(const Data *data)
{
    // Distribui os dias max de cada mes
    // OBS: inicializa com 0
    int diasPorMes[] = {
        0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    // Calcula ano bissexto
    if ((data->ano % 4 == 0 && data->ano % 100 != 0) || (data->ano % 400 == 0))
    {
        diasPorMes[Feb] = 29; // Ano bissexto, fev tem 29 dias
    }

    // valida o dia com base no enum
    // o mes nao pode ter mais dias que o diaPorMes
    if (data->dia < 1 || data->dia > diasPorMes[data->mes]) return 0;

    return 1;
}

int calculaDiaSemana(const Data *data)
{
    int dia = data->dia;
    int mes = data->mes;
    int ano = data->ano;

    if (mes == Jan || mes == Feb)
    {
        mes += 12;
        ano -= 1;
    }

    int K = ano % 100; // recebe os ultimos 2 digitos do ano
    int J = ano / 100; // recebe os primeiros 2 digitos do ano

    // Aplica algoritmo de Zeller
    int h = (dia + (13 * (mes + 1)) / 5 + K + (K / 4) + (J / 4) - 2 * J) % 7;

    // retorna 0 para domingo, 1 segunda, 2 terca, etc.
    return (h + 6) % 7;
}

const char* obtemDiaSemana(int diaSemana)
{
    const char* dias[] =
    {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };

    // Retorna o dia da semana abreviado 
    return dias[diaSemana];
}

const char* obtemNomeMes(meses mes) {
    // validacao feita na inicializacao de Data
    return nomeMeses[mes];
}