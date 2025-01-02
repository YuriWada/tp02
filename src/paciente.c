#include "../include/paciente.h"

void inicializaPaciente(Paciente *paciente, int id, int alta, int ano, int mes, int dia, int hora, 
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
    paciente->tempoEspera = 0.0;
    paciente->tempoAtendimento = 0.0;
}

void atualizaEstado(Paciente *paciente, EstadoPaciente novoEstado)
{
    paciente->estadoAtual = novoEstado;
}

void registraTempoEspera(Paciente *paciente, float tempo)
{
    paciente->tempoEspera += tempo;
}

void registraTempoAtendimento(Paciente *paciente, float tempo)
{
    paciente->tempoAtendimento += tempo;
}

void imprimePaciente(const Paciente *paciente)
{
    // implementa imprimePaciente
}