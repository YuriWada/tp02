#include "../include/learquivo.h"
#include "../include/escalonador.h"
#include "../include/datahora.h"

int main(int argc, char *argv[])
{
    LeitorArquivo leitor;
    Escalonador escalonador;

    // inicializacao do relogio
    float relogio = 0.0f;

    // Inicializa o leitor de arquivo
    if (!inicializaLeitorArquivo(&leitor, argv[1]))
    {
        fprintf(stderr, "Erro ao inicializar o leitor de arquivo.\n");
        return 1;
    }

    // Inicializa o escalonador
    DataHora *dataHoraRef;
    inicializaDataHora(dataHoraRef, leitor.pacientes[0]); 
    inicializaEscalonador(&escalonador, dataHoraRef, leitor.configuracoes.numPacientes, &leitor.configuracoes);

    // Insere eventos iniciais (entrada dos pacientes)
    for (int i = 0; i < leitor.configuracoes.numPacientes; i++)
    {
        DataHora paciente;
        inicializaDataHora(&paciente, leitor.pacientes[i]);
        float tempo = dataParaHoras(dataHoraRef, paciente);
        insereEvento(&escalonador, tempo, 1, &leitor.pacientes[i]);
    }

    while (escalonador.tamanho > 0 || checaFilas(&escalonador) == 1)
    {
        // Retira o próximo evento do escalonador
        Evento evento = retiraProximoEvento(&escalonador);
        relogio += evento.tempo;
        Paciente *paciente = evento.paciente;

        // Atualiza o estado do paciente
        //atualizaEstado(paciente, evento.tipo);

        int proximoEstado = evento.tipo + 1;

        switch (evento.tipo)
        {
        case 1: // Paciente na triagem
            registraTempoAtendimento(paciente, escalonador->config->tempoTriagem);
            insereEvento(escalonador, evento.tempo + escalonador->config->tempoTriagem, 2, paciente);
            break;

        case 2:
        { // Atendimento
            registraTempoAtendimento(paciente, escalonador->config->tempoAtendimento);

            if (paciente->alta)
            {
                // Calcula saída do paciente se ele recebeu alta
                calculaSaida(paciente);
                atualizaEstado(paciente, ALTA_HOSP);
            }
            else
            {
                // Decide a próxima fila baseada na prioridade do paciente
                int prioridade = paciente->urgencia;
                enfileira(&escalonador->filas[prioridade], paciente, evento.tempo);
            }
            break;
        }

        case 3: // Medidas Hospitalares
            registraTempoAtendimento(paciente, escalonador->config->tempoMH);
            insereEvento(escalonador, evento.tempo + escalonador->config->tempoMH, 4, paciente);
            break;

        case 4: // Testes Laboratoriais
            registraTempoAtendimento(paciente, escalonador->config->tempoTL);
            insereEvento(escalonador, evento.tempo + escalonador->config->tempoTL, 5, paciente);
            break;

        case 5: // Exames de Imagem
            registraTempoAtendimento(paciente, escalonador->config->tempoEI);

            if (paciente->medidasHospitalares > 0 || paciente->testesLaboratorio > 0 || paciente->examesImagem > 0)
            {
                // Retorna para próxima fila de prioridade
                enfileira(&escalonador->filas[0], paciente, evento.tempo);
            }
            else
            {
                // Calcula saída do paciente
                calculaSaida(paciente);
                atualizaEstado(paciente, ALTA_HOSP);
            }
            break;

        default:
            erroAssert(0, "Evento desconhecido no escalonador.");
            break;
        }

        // Processa filas em ordem de prioridade
        for (int i = QTDE_FILAS - 1; i >= 0; i--)
        {
            if (!filaVazia(&escalonador->filas[i]))
            {
                Paciente *proxPaciente = desenfileira(&escalonador->filas[i], evento.tempo);
                insereEvento(escalonador, evento.tempo, 2, proxPaciente);
                break; // Apenas processa uma fila por ciclo
            }
        }
    }

    // Imprime as estatísticas dos pacientes
    for (int i = 0; i < leitor.configuracoes.numPacientes; i++)
    {
        imprimePaciente(&leitor.pacientes[i]);
    }

    // Finaliza o escalonador e o leitor de arquivo
    finalizaEscalonador(&escalonador);
    liberaLeitorArquivo(&leitor);
}
