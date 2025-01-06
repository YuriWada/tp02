#include "../include/paciente.h"

/**
 * Inicializa os dados do paciente com os valores fornecidos.
 */
void inicializaPaciente(Paciente *paciente, int id, int alta, int ano, int mes, int dia, float hora,
                        int urgencia, int medidasH, int testesL, int examesI, int instrumentosM) {
    // Atribui valores básicos
    paciente->id = id;
    paciente->alta = alta;
    paciente->ano = ano;
    paciente->mes = mes;
    paciente->dia = dia;
    paciente->hora = hora;
    paciente->urgencia = urgencia;
    paciente->medidasHospitalares = medidasH;
    paciente->testesLaboratorio = testesL;
    paciente->examesImagem = examesI;
    paciente->instrumentosMedicamentos = instrumentosM;

    // Inicializa estatísticas
    paciente->estadoAtual = NAO_CHEGOU;
    paciente->tempoEspera = 0.0f;
    paciente->tempoAtendimento = 0.0f;

    // Inicializa data de saída
    paciente->anoSaida = 0;
    paciente->mesSaida = 0;
    paciente->diaSaida = 0;
    paciente->horaSaida = 0.0f;
}

/**
 * Atualiza o estado atual do paciente.
 */
void atualizaEstado(Paciente *paciente, EstadoPaciente novoEstado) {
    erroAssert(paciente != NULL, "Ponteiro nulo para Paciente.");
    paciente->estadoAtual = novoEstado;
}

/**
 * Registra o tempo de espera total do paciente.
 */
void registraTempoEspera(Paciente *paciente, float tempo) {
    erroAssert(paciente != NULL, "Ponteiro nulo para Paciente.");
    paciente->tempoEspera += tempo;
}

/**
 * Registra o tempo total de atendimento do paciente.
 */
void registraTempoAtendimento(Paciente *paciente, float tempo) {
    erroAssert(paciente != NULL, "Ponteiro nulo para Paciente.");
    paciente->tempoAtendimento += tempo;
}

/**
 * Transforma a data do paciente no formato Dia da Semana e Nome do Mês.
 */
DiaMes defineDataString(const Paciente *paciente) {
    erroAssert(paciente != NULL, "Ponteiro nulo para Paciente.");

    Data data;
    inicializaData(&data, paciente->dia, paciente->mes, paciente->ano, paciente->hora);

    DiaMes diaMes;
    diaMes.diaSemana = obtemDiaSemana(calculaDiaSemana(&data));
    diaMes.nomeMes = obtemNomeMes(data.mes);
    return diaMes;
}

/**
 * Calcula a data e horário de saída do paciente com base no tempo total de permanência no hospital.
 */
void calculaSaida(Paciente *paciente) {
    erroAssert(paciente != NULL, "Ponteiro nulo para Paciente.");

    // Calcula o tempo total no hospital
    float tempoTotal = paciente->tempoAtendimento + paciente->tempoEspera;

    Data dataSaida;
    inicializaData(&dataSaida, paciente->dia, paciente->mes, paciente->ano, paciente->hora);

    // Adiciona o tempo total ao horário da entrada
    dataSaida.hora += tempoTotal;

    // Ajusta o horário e a data se ultrapassar limites
    while (dataSaida.hora >= 24.0f) {
        dataSaida.hora -= 24.0f;
        dataSaida.dia++;
        if (!validaData(&dataSaida)) {
            dataSaida.dia = 1;
            dataSaida.mes++;
            if (dataSaida.mes > Dec) {
                dataSaida.mes = Jan;
                dataSaida.ano++;
            }
        }
    }

    // Atualiza os campos de saída no paciente
    paciente->diaSaida = dataSaida.dia;
    paciente->mesSaida = dataSaida.mes;
    paciente->anoSaida = dataSaida.ano;
    paciente->horaSaida = dataSaida.hora;
}

/**
 * Imprime as informações do paciente no formato especificado pelo TP.
 */
void imprimePaciente(const Paciente *paciente) {
    erroAssert(paciente != NULL, "Ponteiro nulo para Paciente.");

    // Data de entrada
    Data dataEntrada;
    inicializaData(&dataEntrada, paciente->dia, paciente->mes, paciente->ano, paciente->hora);
    DiaMes diaMesEntrada = defineDataString(paciente);
    const char *horaEntrada = transformaHora(&dataEntrada);

    // Data de alta
    Data dataSaida;
    inicializaData(&dataSaida, paciente->diaSaida, paciente->mesSaida, paciente->anoSaida, paciente->horaSaida);
    DiaMes diaMesSaida;
    diaMesSaida.diaSemana = obtemDiaSemana(calculaDiaSemana(&dataSaida));
    diaMesSaida.nomeMes = obtemNomeMes(dataSaida.mes);
    const char *horaSaida = transformaHora(&dataSaida);

    // Tempo total no hospital
    float tempoTotal = paciente->tempoAtendimento + paciente->tempoEspera;

    // Imprime os dados formatados
    printf("%d %s %s %02d %s %04d %s %s %02d %s %04d %.2f %.2f %.2f\n",
           paciente->id,
           diaMesEntrada.diaSemana, diaMesEntrada.nomeMes, dataEntrada.dia, horaEntrada, dataEntrada.ano,
           diaMesSaida.diaSemana, diaMesSaida.nomeMes, dataSaida.dia, horaSaida, dataSaida.ano,
           tempoTotal, paciente->tempoAtendimento, paciente->tempoEspera);
}
