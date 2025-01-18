#include "../include/datahora.h"

void inicializaDataHora(DataHora *dataHora, Paciente paciente)
{
    dataHora->dia = paciente.dia;
    dataHora->mes = paciente.mes;
    dataHora->ano = paciente.ano;
    dataHora->hora = paciente.hora;
}

/**
 * Verifica se um ano é bissexto.
 */
int ehBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

/**
 * Obtém o número de dias em um mês de um determinado ano.
 */
int diasNoMes(int mes, int ano) {
    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && ehBissexto(ano)) {
        return 29; // Fevereiro em ano bissexto
    }
    return diasPorMes[mes - 1];
}

/**
 * Converte uma data no formato `DataHora` em horas a partir de uma data de referência.
 */
float dataParaHoras(DataHora *referencia, DataHora entrada) {
    float horasTotais = 0.0;

    // Conta os anos entre a data de referência e a data de entrada
    for (int anoAtual = referencia->ano; anoAtual < entrada.ano; anoAtual++) {
        horasTotais += ehBissexto(anoAtual) ? 366 * 24 : 365 * 24;
    }

    // Conta os meses do ano de entrada
    for (int mesAtual = 1; mesAtual < entrada.mes; mesAtual++) {
        horasTotais += diasNoMes(mesAtual, entrada.ano) * 24;
    }

    // Conta os dias do mês de entrada
    horasTotais += (entrada.dia - 1) * 24;

    // Soma as horas do dia de entrada
    horasTotais += entrada.hora;

    // Remove o tempo da data de referência
    for (int mesAtual = 1; mesAtual < referencia->mes; mesAtual++) {
        horasTotais -= diasNoMes(mesAtual, referencia->ano) * 24;
    }
    horasTotais -= (referencia->dia - 1) * 24;
    horasTotais -= referencia->hora;

    return horasTotais;
}

/**
 * Converte horas em uma data no formato `DataHora` a partir de uma data de referência.
 */
void horasParaData(float horas, DataHora referencia, DataHora *resultado) {
    *resultado = referencia;

    // Adiciona as horas iniciais
    resultado->hora += horas;

    // Ajusta dias e meses conforme necessário
    while (resultado->hora >= 24) {
        resultado->hora -= 24;
        resultado->dia++;
        if (resultado->dia > diasNoMes(resultado->mes, resultado->ano)) {
            resultado->dia = 1;
            resultado->mes++;
            if (resultado->mes > 12) {
                resultado->mes = 1;
                resultado->ano++;
            }
        }
    }

    // Ajusta para trás caso as horas sejam negativas
    while (resultado->hora < 0) {
        resultado->hora += 24;
        resultado->dia--;
        if (resultado->dia < 1) {
            resultado->mes--;
            if (resultado->mes < 1) {
                resultado->mes = 12;
                resultado->ano--;
            }
            resultado->dia = diasNoMes(resultado->mes, resultado->ano);
        }
    }
}
