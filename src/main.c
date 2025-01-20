#include "../include/learquivo.h"
#include "../include/escalonador.h"
#include "../include/datahora.h"
#include "../include/procedimento.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    LeitorArquivo leitor;
    Escalonador escalonador;

    // Inicializa o leitor de arquivo
    if (!inicializaLeitorArquivo(&leitor, argv[1]))
    {
        fprintf(stderr, "Erro ao inicializar o leitor de arquivo.\n");
        return 1;
    }

    // Cria data-hora referencia
    DataHora dataHoraRef;
    inicializaDataHora(&dataHoraRef, leitor.pacientes[0]);

    // Inicializa o escalonador
    inicializaEscalonador(&escalonador, leitor.configuracoes.numPacientes, &leitor.configuracoes);

    // inicializacao do relogio
    float tempoAtual = 0.0f;

    // Insere eventos iniciais (entrada dos pacientes)
    for (int i = 0; i < leitor.configuracoes.numPacientes; i++)
    {
        DataHora paciente;
        inicializaDataHora(&paciente, leitor.pacientes[i]);
        float tempo = dataParaHoras(&dataHoraRef, paciente);
        insereEvento(&escalonador, tempo, 0, &leitor.pacientes[i]);
    }

    while (escalonador.tamanho > 0 || checaFilas(&escalonador) == 1)
    {
        // Retira o próximo evento do escalonador
        Evento evento = retiraProximoEvento(&escalonador);
        tempoAtual += evento.tempo;
        Paciente *paciente = evento.paciente;

        if (paciente->estadoAtual == NAO_CHEGOU)
        {
            enfileira(&escalonador.filaTr, paciente, tempoAtual);
            paciente->tempoNaFila = tempoAtual;
            paciente->estadoAtual++;
        }

        // Processa a fila de triagem se houver unidade disponivel para atendimento
        if (encontraUnidadeDisponivel(&escalonador.procedimentoTR, tempoAtual) != -1)
        {
            int indiceUnidadeDisponivel = encontraUnidadeDisponivel(&escalonador.procedimentoTR, tempoAtual);
            float tempoOcioso = tempoAtual - escalonador.procedimentoTR.unidades[indiceUnidadeDisponivel].tempoUltimoServico;
            escalonador.procedimentoTR.unidades[indiceUnidadeDisponivel].tempoOcioso += tempoOcioso;

            // Remove paciente da fila
            Paciente *pacienteTr = desenfileira(&escalonador.filaTr);
            pacienteTr->estadoAtual++; // Atualiza estado do paciente para Sendo Triado (2)

            // Registra o tempo de espera do paciente
            DataHora dataHoraPaciente;
            inicializaDataHora(&dataHoraPaciente, *pacienteTr);
            float tempoEspera = tempoAtual - dataParaHoras(&dataHoraRef, dataHoraPaciente);
            registraTempoEspera(pacienteTr, tempoEspera);

            escalonador.procedimentoTR.unidades[indiceUnidadeDisponivel].tempoUltimoServico = tempoAtual + escalonador.config->tempoTriagem;
            registraTempoAtendimento(pacienteTr, escalonador.config->tempoTriagem);

            float tempoEvento = tempoAtual + escalonador.config->tempoTriagem;
            insereEvento(&escalonador, tempoEvento, indiceUnidadeDisponivel, pacienteTr);

            // Coloca o paciente na fila adequada
            enfileira(&escalonador.filasAt[pacienteTr->urgencia], pacienteTr, tempoAtual);
            paciente->tempoNaFila = tempoAtual;
            pacienteTr->estadoAtual++; // Atualiza estado do paciente para Na fila de atendimento (3)
        }

        for (int i = 2; i >= 0; i--)
        {
            switch (paciente->estadoAtual)
            {
            case 4:
                if (!filaVazia(&escalonador.filasAt[i]) && encontraUnidadeDisponivel(&escalonador.procedimentoAT, tempoAtual) != -1)
                {
                    int indiceUnidadeDisponivel = encontraUnidadeDisponivel(&escalonador.procedimentoAT, tempoAtual);
                    float tempoOcioso = tempoAtual - escalonador.procedimentoAT.unidades[indiceUnidadeDisponivel].tempoUltimoServico;
                    escalonador.procedimentoAT.unidades[indiceUnidadeDisponivel].tempoOcioso += tempoOcioso;

                    Paciente *pacienteAt = desenfileira(&escalonador.filasAt[i]);

                    float tempoEspera = tempoAtual - pacienteAt->tempoNaFila;
                    registraTempoEspera(pacienteAt, tempoEspera);

                    escalonador.procedimentoAT.unidades[indiceUnidadeDisponivel].tempoUltimoServico = tempoAtual + escalonador.config->tempoAtendimento;
                    registraTempoAtendimento(pacienteAt, escalonador.config->tempoAtendimento);

                    float tempoEvento = tempoAtual + escalonador.config->tempoAtendimento;
                    insereEvento(&escalonador, tempoEvento, indiceUnidadeDisponivel, pacienteAt);
                }
                break;

            case 6:
                if (!filaVazia(&escalonador.filasProc[i]) && encontraUnidadeDisponivel(&escalonador.procedimentoMH, tempoAtual) != -1)
                {
                    int indiceUnidadeDisponivel = encontraUnidadeDisponivel(&escalonador.procedimentoMH, tempoAtual);
                    float tempoOcioso = tempoAtual - escalonador.procedimentoMH.unidades[indiceUnidadeDisponivel].tempoUltimoServico;
                    escalonador.procedimentoMH.unidades[indiceUnidadeDisponivel].tempoOcioso += tempoOcioso;

                    Paciente *pacienteMH = desenfileira(&escalonador.filasProc[i]);

                    float tempoEspera = tempoAtual - pacienteMH->tempoNaFila;
                    registraTempoEspera(pacienteMH, tempoEspera);

                    escalonador.procedimentoMH.unidades[indiceUnidadeDisponivel].tempoUltimoServico = tempoAtual + escalonador.config->tempoMH;
                    registraTempoAtendimento(pacienteMH, escalonador.config->tempoMH);

                    float tempoEvento = tempoAtual + escalonador.config->tempoAtendimento;
                    insereEvento(&escalonador, tempoEvento, indiceUnidadeDisponivel, pacienteMH);
                }
                break;

            case 8:
                if (!filaVazia(&escalonador.filasProc[i]) && encontraUnidadeDisponivel(&escalonador.procedimentoTL, tempoAtual) != -1)
                {
                    int indiceUnidadeDisponivel = encontraUnidadeDisponivel(&escalonador.procedimentoTL, tempoAtual);
                    float tempoOcioso = tempoAtual - escalonador.procedimentoTL.unidades[indiceUnidadeDisponivel].tempoUltimoServico;
                    escalonador.procedimentoTL.unidades[indiceUnidadeDisponivel].tempoOcioso += tempoOcioso;

                    Paciente *pacienteTL = desenfileira(&escalonador.filasProc[i]);

                    float tempoEspera = tempoAtual - pacienteTL->tempoNaFila;
                    registraTempoEspera(pacienteTL, tempoEspera);

                    escalonador.procedimentoTL.unidades[indiceUnidadeDisponivel].tempoUltimoServico = tempoAtual + escalonador.config->tempoTL;
                    registraTempoAtendimento(pacienteTL, escalonador.config->tempoTL);

                    float tempoEvento = tempoAtual + escalonador.config->tempoAtendimento;
                    insereEvento(&escalonador, tempoEvento, indiceUnidadeDisponivel, pacienteTL);
                }
                break;

            case 10:
                if (!filaVazia(&escalonador.filasProc[i]) && encontraUnidadeDisponivel(&escalonador.procedimentoEI, tempoAtual) != -1)
                {
                    int indiceUnidadeDisponivel = encontraUnidadeDisponivel(&escalonador.procedimentoEI, tempoAtual);
                    float tempoOcioso = tempoAtual - escalonador.procedimentoEI.unidades[indiceUnidadeDisponivel].tempoUltimoServico;
                    escalonador.procedimentoEI.unidades[indiceUnidadeDisponivel].tempoOcioso += tempoOcioso;

                    Paciente *pacienteEI = desenfileira(&escalonador.filasProc[i]);

                    float tempoEspera = tempoAtual - pacienteEI->tempoNaFila;
                    registraTempoEspera(pacienteEI, tempoEspera);

                    escalonador.procedimentoEI.unidades[indiceUnidadeDisponivel].tempoUltimoServico = tempoAtual + escalonador.config->tempoEI;
                    registraTempoAtendimento(pacienteEI, escalonador.config->tempoEI);

                    float tempoEvento = tempoAtual + escalonador.config->tempoAtendimento;
                    insereEvento(&escalonador, tempoEvento, indiceUnidadeDisponivel, pacienteEI);
                }
                break;

            case 12:
                if (!filaVazia(&escalonador.filasProc[i]) && encontraUnidadeDisponivel(&escalonador.procedimentoIM, tempoAtual) != -1)
                {
                    int indiceUnidadeDisponivel = encontraUnidadeDisponivel(&escalonador.procedimentoIM, tempoAtual);
                    float tempoOcioso = tempoAtual - escalonador.procedimentoIM.unidades[indiceUnidadeDisponivel].tempoUltimoServico;
                    escalonador.procedimentoIM.unidades[indiceUnidadeDisponivel].tempoOcioso += tempoOcioso;

                    Paciente *pacienteIM = desenfileira(&escalonador.filasProc[i]);

                    float tempoEspera = tempoAtual - pacienteIM->tempoNaFila;
                    registraTempoEspera(pacienteIM, tempoEspera);

                    escalonador.procedimentoIM.unidades[indiceUnidadeDisponivel].tempoUltimoServico = tempoAtual + escalonador.config->tempoIM;
                    registraTempoAtendimento(pacienteIM, escalonador.config->tempoIM);

                    float tempoEvento = tempoAtual + escalonador.config->tempoAtendimento;
                    insereEvento(&escalonador, tempoEvento, indiceUnidadeDisponivel, pacienteIM);
                }
                break;

            default:
                break;
            }
        }

        proximoEstado(paciente);

        // Verifica se o paciente recebeu alta
        if (paciente->estadoAtual == ALTA_HOSP)
            break;
        else
        {
            switch (paciente->estadoAtual)
            {
            case 3:
                enfileira(&escalonador.filasAt[paciente->urgencia], paciente, tempoAtual);
                paciente->tempoNaFila = tempoAtual;
                paciente->estadoAtual++;
                break;

            default:
                enfileira(&escalonador.filasProc[paciente->urgencia], paciente, tempoAtual);
                paciente->tempoNaFila = tempoAtual;
                paciente->estadoAtual++;
                break;
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
