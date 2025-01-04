#include "../include/paciente.h"

void inicializaPaciente(Paciente *paciente, int id, int alta, int ano, int mes, int dia, float hora,
                        int urgencia, int medidasH, int testesL, int examesI, int instrumentosM)
{
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

    // Inicializacao das estatisticas
    // Tempo total HZ = tempoEspera + tempoAtendimento, neste caso inicializa 0
    paciente->estadoAtual = NAO_CHEGOU;
    paciente->tempoEspera = 0.0f;
    paciente->tempoAtendimento = 0.0f;

    paciente->anoSaida = 0;
    paciente->mesSaida = 0;
    paciente->diaSaida = 0;
    paciente->horaSaida = 0.0f;
}

void atualizaEstado(Paciente *paciente, EstadoPaciente novoEstado)
{
    erroAssert(paciente != NULL, "Ponteiro nulo para Paciente.");
    paciente->estadoAtual = novoEstado;
}

void registraTempoEspera(Paciente *paciente, float tempo)
{
    erroAssert(paciente != NULL, "Ponteiro nulo para Paciente.");
    paciente->tempoEspera += tempo;
}

void registraTempoAtendimento(Paciente *paciente, float tempo)
{
    erroAssert(paciente != NULL, "Ponteiro nulo para Paciente.");
    paciente->tempoAtendimento += tempo;
}

DiaMes defineDataString(Paciente *paciente)
{
    erroAssert(paciente != NULL, "Ponteiro nulo para Paciente.");

    Data data;
    inicializaData(&data, paciente->dia, paciente->mes, paciente->ano, paciente->hora);

    DiaMes diaMes;
    diaMes.diaSemana = obtemDiaSemana(calculaDiaSemana(&data));
    diaMes.nomeMes = obtemNomeMes(data.mes);
    return diaMes;
}

void calculaSaida(Paciente *paciente)
{
    erroAssert(paciente != NULL, "Ponteiro nulo para Paciente.");

    float tempoTotal = paciente->tempoAtendimento + paciente->tempoEspera;
    paciente->horaSaida = paciente->hora + tempoTotal;

    while (paciente->horaSaida >= 24.0f)
    {
        paciente->horaSaida -= 24.0f;
        paciente->diaSaida++;
    }

    // Atualiza mes e ano conforme necessário
    Data data;
    inicializaData(&data, paciente->diaSaida, paciente->mes, paciente->ano, 0.0f);
    if (!validaData(&data))
    {
        paciente->diaSaida = 1;
        paciente->mesSaida++;
        if (paciente->mesSaida > Dec)
        {
            paciente->mesSaida = Jan;
            paciente->anoSaida++;
        }
    }
}

void imprimePaciente(const Paciente *paciente)
{
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

    // Cálculo do tempo total no HZ
    float tempoTotal = paciente->tempoAtendimento + paciente->tempoEspera;

    // Impressão formatada
    printf("%d %s %s %02d %s %04d %s %s %02d %s %04d %.2f %.2f %.2f\n",
           paciente->id,
           diaMesEntrada.diaSemana, diaMesEntrada.nomeMes, dataEntrada.dia, horaEntrada, dataEntrada.ano,
           diaMesSaida.diaSemana, diaMesSaida.nomeMes, dataSaida.dia, horaSaida, dataSaida.ano,
           tempoTotal, paciente->tempoAtendimento, paciente->tempoEspera);
}