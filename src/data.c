#include "../include/data.h"

// Array que corresponde às chaves do enum "meses"
const char* nomeMeses[] = {
    "", "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

int inicializaData(Data *data, int dia, meses mes, int ano, float hora)
{
    // Verificacao de datas de entrada
    erroAssert(data != NULL, "Ponteiro nulo para Data.");
    erroAssert(dia > 0 && dia <= 31, "Data invalida!");
    erroAssert(mes >= Jan && mes <= Dec, "Data invalida!");
    erroAssert(hora >= 0.0f && hora < 24.0f, "Data invalida!");

    data->dia = dia;
    data->mes = mes;
    data->ano = ano;
    data->hora = hora;

    return validaData(data);
}

int validaData(const Data *data)
{
    erroAssert(data != NULL, "Ponteiro nulo para Data.");

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
    return data->dia <= diasPorMes[data->mes];
}

int calculaDiaSemana(const Data *data)
{
    erroAssert(data != NULL, "Ponteiro nulo para Data.");

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

const char *obtemDiaSemana(int diaSemana)
{
    const char *dias[] = {
            "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };

    // Retorna o dia da semana abreviado
    return dias[diaSemana];
}

const char *obtemNomeMes(meses mes)
{
    erroAssert(mes >= Jan && mes <= Dec, "Mes invalido.");
    // validacao feita na inicializacao de Data
    return nomeMeses[mes];
}

const char *transformaHora(const Data *data)
{
    erroAssert(data != NULL, "Ponteiro nulo para Data.");
    
    int hora = (int)data->hora;                                        // obtem a parte inteira
    int minutos = (int)((data->hora - hora) * 60);                     // converte parte fracionaria em minutos
    int segundos = (int)((((data->hora - hora) * 60) - minutos) * 60); // parte restante convertida para segundos

    // Buffer estatico para armazenar o resultado
    static char buffer[9];

    // formatando a string no formato hh:mm:ss
    sprintf(buffer, "%02d:%02d:%02d", hora, minutos, segundos);
    return buffer;
}