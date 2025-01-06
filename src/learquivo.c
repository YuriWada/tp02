#include "../include/learquivo.h"

int inicializaLeitorArquivo(LeitorArquivo *leitor, const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    erroAssert(arquivo != NULL, "Erro ao abrir o arquivo.");

    // Lê as configurações gerais
    int numLidos = 0;

    numLidos = fscanf(arquivo, "%f %d",
                      &leitor->configuracoes.tempoTriagem,
                      &leitor->configuracoes.numTriagem);
    erroAssert(numLidos == 2, "Erro na leitura da configuração de triagem.");

    numLidos = fscanf(arquivo, "%f %d",
                      &leitor->configuracoes.tempoAtendimento,
                      &leitor->configuracoes.numAtendimento);
    erroAssert(numLidos == 2, "Erro na leitura da configuração de atendimento.");

    numLidos = fscanf(arquivo, "%f %d",
                      &leitor->configuracoes.tempoMH,
                      &leitor->configuracoes.numMH);
    erroAssert(numLidos == 2, "Erro na leitura da configuração de medidas hospitalares.");

    numLidos = fscanf(arquivo, "%f %d",
                      &leitor->configuracoes.tempoTL,
                      &leitor->configuracoes.numTL);
    erroAssert(numLidos == 2, "Erro na leitura da configuração de testes laboratoriais.");

    numLidos = fscanf(arquivo, "%f %d",
                      &leitor->configuracoes.tempoEI,
                      &leitor->configuracoes.numEI);
    erroAssert(numLidos == 2, "Erro na leitura da configuração de exames de imagem.");

    numLidos = fscanf(arquivo, "%f %d",
                      &leitor->configuracoes.tempoIM,
                      &leitor->configuracoes.numIM);
    erroAssert(numLidos == 2, "Erro na leitura da configuração de instrumentos/medicamentos.");

    numLidos = fscanf(arquivo, "%d",
                      &leitor->configuracoes.numPacientes);
    erroAssert(numLidos == 1, "Erro na leitura do número de pacientes.");
    erroAssert(leitor->configuracoes.numPacientes > 0, "Número de pacientes inválido.");

    // Aloca memória para os pacientes
    leitor->pacientes = malloc(leitor->configuracoes.numPacientes * sizeof(Paciente));
    erroAssert(leitor->pacientes != NULL, "Erro ao alocar memória para pacientes.");

    // Lê os pacientes
    for (int i = 0; i < leitor->configuracoes.numPacientes; i++) {
        int id, alta, ano, mes, dia, urgencia, medidas, testes, exames, instrumentos;
        float hora;

        numLidos = fscanf(arquivo, "%d %d %d %d %d %f %d %d %d %d %d",
                          &id, &alta, &ano, &mes, &dia, &hora, &urgencia,
                          &medidas, &testes, &exames, &instrumentos);
        erroAssert(numLidos == 11, "Erro na leitura dos dados do paciente.");

        inicializaPaciente(&leitor->pacientes[i], id, alta, ano, mes, dia, hora, urgencia, medidas, testes, exames, instrumentos);
    }

    fclose(arquivo);
    return 1;
}

void liberaLeitorArquivo(LeitorArquivo *leitor)
{
    free(leitor->pacientes);
    leitor->pacientes = NULL;
}
