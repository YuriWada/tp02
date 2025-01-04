#include "../include/fila.h"

void inicializaFila(Fila *fila)
{
    // Inicializacao da fila
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
    fila->tempoTotalEspera = 0.0f;
    fila->somaTamanhosFila = 0.0f;
    fila->amostrasTamanhoFila = 0;
}

void enfileira(Fila *fila, Paciente *paciente, float tempoAtual)
{
    // alocacao dinamica para novo no
    NoFila *novoNo = (NoFila *)malloc(sizeof(NoFila));

    // verifica se alocacao deu certo
    erroAssert(novoNo != NULL, "Erro ao alocar memoria para o node da fila!");

    novoNo->paciente = paciente;
    novoNo->tempoEntrada = tempoAtual;
    novoNo->proximo = NULL;

    if (fila->fim)
        fila->fim->proximo = novoNo;
    else
        fila->inicio = novoNo;

    fila->fim = novoNo;
    fila->tamanho++;

    fila->somaTamanhosFila += fila->tamanho;
    fila->amostrasTamanhoFila++;
}

Paciente *desenfileira(Fila *fila, float tempoAtual)
{
    if (filaVazia(fila))
        return NULL;

    NoFila *removido = fila->inicio;
    Paciente *paciente = removido->paciente;

    // Calcula o tempo de espera
    float tempoEspera = tempoAtual - removido->tempoEntrada;
    fila->tempoTotalEspera += tempoEspera;
    registraTempoEspera(paciente, tempoEspera);

    fila->inicio = removido->proximo;
    if (!fila->inicio)
    {
        fila->fim = NULL;
    }

    free(removido);
    fila->tamanho--;
    return paciente;
}

int filaVazia(Fila *fila)
{
    return fila->inicio == NULL;
}

void finalizaFila(Fila *fila)
{
    // Libera a memória restante
    while (!filaVazia(fila))
    {
        desenfileira(fila, 0.0f); // Ignoramos o tempo atual
    }
}