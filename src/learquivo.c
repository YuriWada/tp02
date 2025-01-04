#include "../include/learquivo.h"

int inicializaLeitorArquivo(LeitorArquivo *leitor, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    erroAssert(arquivo != NULL, "Erro ao abrir o arquivo.");

    // Lê as configurações gerais
    fscanf(arquivo, "%d %d %d %d %d %d %d %d %d %d %d %d %d",
           &leitor->configuracoes.tempoTriagem, &leitor->configuracoes.numTriagem,
           &leitor->configuracoes.tempoAtendimento, &leitor->configuracoes.numAtendimento,
           &leitor->configuracoes.tempoMH, &leitor->configuracoes.numMH,
           &leitor->configuracoes.tempoTL, &leitor->configuracoes.numTL,
           &leitor->configuracoes.tempoEI, &leitor->configuracoes.numEI,
           &leitor->configuracoes.tempoIM, &leitor->configuracoes.numIM,
           &leitor->configuracoes.numPacientes);

    // Aloca memória para os pacientes
    leitor->pacientes = malloc(leitor->configuracoes.numPacientes * sizeof(Paciente));
    erroAssert(leitor->pacientes != NULL, "Erro ao alocar memória para pacientes.");

    // Lê os pacientes
    for (int i = 0; i < leitor->configuracoes.numPacientes; i++) {
        int id, alta, ano, mes, dia, urgencia, medidas, testes, exames, instrumentos;
        float hora;
        fscanf(arquivo, "%d %d %d %d %d %f %d %d %d %d %d",
               &id, &alta, &ano, &mes, &dia, &hora, &urgencia, &medidas, &testes, &exames, &instrumentos);

        inicializaPaciente(&leitor->pacientes[i], id, alta, ano, mes, dia, hora, urgencia, medidas, testes, exames, instrumentos);
    }

    fclose(arquivo);
    return 1;
}

void liberaLeitorArquivo(LeitorArquivo *leitor) {
    free(leitor->pacientes);
    leitor->pacientes = NULL;
}
