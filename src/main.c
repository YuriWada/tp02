#include "../include/learquivo.h"
#include "../include/escalonador.h"

int main(int argc, char *argv[])
{
    // Verifica se o arquivo de entrada foi especificado
    erroAssert(argc == 2, "Uso: ./tp3.out <arquivo_de_entrada>");

    // Inicializa o leitor de arquivo
    LeitorArquivo leitor;
    inicializaLeitorArquivo(&leitor, argv[1]);

    // Inicializa o escalonador
    Escalonador escalonador;
    inicializaEscalonador(&escalonador, leitor.configuracoes.numPacientes, &leitor.configuracoes);

    // Insere eventos iniciais (entrada dos pacientes)
    for (int i = 0; i < leitor.configuracoes.numPacientes; i++)
    {
        insereEvento(&escalonador, leitor.pacientes[i].hora, 1, &leitor.pacientes[i]);
    }

    // Processa os eventos
    processaEventos(&escalonador);

    // Imprime as estatísticas dos pacientes
    for (int i = 0; i < leitor.configuracoes.numPacientes; i++)
    {
        imprimePaciente(&leitor.pacientes[i]);
    }

    // Finaliza o escalonador e o leitor de arquivo
    finalizaEscalonador(&escalonador);
    liberaLeitorArquivo(&leitor);

    return 0;
}
