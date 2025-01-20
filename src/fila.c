#include "../include/fila.h"

/**
 * Inicializa uma fila vazia.
 * Configura os ponteiros de início e fim como NULL e zera as variáveis estatísticas.
 */
void inicializaFila(Fila *fila)
{
    // Verifica se a estrutura da fila é válida
    erroAssert(fila != NULL, "Tentativa de inicializar uma fila nula.");

    // Inicializa os ponteiros para NULL
    fila->inicio = NULL;
    fila->fim = NULL;

    // Inicializa os valores da fila
    fila->tamanho = 0;
    fila->tempoTotalEspera = 0.0f;
    fila->somaTamanhosFila = 0.0f;
    fila->amostrasTamanhoFila = 0;
}

/**
 * Adiciona um paciente ao final da fila.
 * Atualiza as estatísticas relacionadas ao tamanho da fila.
 */
void enfileira(Fila *fila, Paciente *paciente, float tempoAtual)
{
    // Verifica se os parâmetros são válidos
    erroAssert(fila != NULL, "Tentativa de enfileirar em uma fila nula.");
    erroAssert(paciente != NULL, "Tentativa de enfileirar um paciente nulo.");

    // Aloca memória para um novo nó da fila
    NoFila *novoNo = (NoFila *)malloc(sizeof(NoFila));
    erroAssert(novoNo != NULL, "Erro ao alocar memória para um nó na fila.");

    // Configura o novo nó
    novoNo->paciente = paciente;
    novoNo->tempoEntrada = tempoAtual; // Armazena o tempo de entrada do paciente na fila
    novoNo->proximo = NULL;

    // Atualiza os ponteiros da fila
    if (fila->fim)
    {
        fila->fim->proximo = novoNo; // Adiciona o novo nó após o último nó existente
    }
    else
    {
        fila->inicio = novoNo; // Caso a fila esteja vazia, o novo nó será o início
    }
    fila->fim = novoNo; // Atualiza o ponteiro para o fim da fila

    // Atualiza estatísticas da fila
    fila->tamanho++;
    fila->somaTamanhosFila += fila->tamanho; // Soma o tamanho atual da fila
    fila->amostrasTamanhoFila++;             // Incrementa o número de amostras coletadas
}

/**
 * Remove e retorna o paciente do início da fila.
 */
Paciente *desenfileira(Fila *fila)
{
    // Verifica se a estrutura da fila é válida
    erroAssert(fila != NULL, "Tentativa de desenfileirar de uma fila nula.");

    // Retorna NULL se a fila estiver vazia
    if (filaVazia(fila))
    {
        return NULL;
    }

    // Remove o primeiro nó da fila
    NoFila *removido = fila->inicio;
    Paciente *paciente = removido->paciente;

    // Atualiza o ponteiro de início da fila
    fila->inicio = removido->proximo;
    if (!fila->inicio)
    {
        fila->fim = NULL; // Se a fila ficou vazia, atualiza o ponteiro do fim
    }

    // Libera a memória do nó removido
    free(removido);

    // Decrementa o tamanho da fila
    fila->tamanho--;

    // Retorna o paciente removido
    return paciente;
}

NoFila *buscaNoPorPaciente(Fila *fila, int idPaciente)
{
    // Verifica se a fila é válida
    erroAssert(fila != NULL, "Tentativa de buscar em uma fila nula.");

    // Percorre a fila procurando o nó com o ID do paciente
    NoFila *atual = fila->inicio;
    while (atual != NULL)
    {
        if (atual->paciente != NULL && atual->paciente->id == idPaciente)
        {
            return atual; // Retorna o nó correspondente
        }
        atual = atual->proximo;
    }

    // Se não encontrou, retorna NULL
    return NULL;
}

/**
 * Verifica se a fila está vazia.
 * Retorna 1 se a fila estiver vazia, 0 caso contrário.
 */
int filaVazia(Fila *fila)
{
    // Verifica se a estrutura da fila é válida
    erroAssert(fila != NULL, "Tentativa de verificar se uma fila nula está vazia.");
    return fila->inicio == NULL;
}

/**
 * Finaliza a fila, liberando todos os nós alocados.
 * Ignora o tempo atual ao remover os pacientes.
 */
void finalizaFila(Fila *fila)
{
    erroAssert(fila != NULL, "Tentativa de finalizar uma fila nula.");

    // Libera cada nó da fila
    NoFila *atual = fila->inicio;
    while (atual != NULL)
    {
        NoFila *proximo = atual->proximo;

        // Não libera o paciente, pois ele pode ser usado em outro lugar.
        free(atual); // Libera apenas o nó da fila
        atual = proximo;
    }

    // Redefine a fila para o estado inicial
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
    fila->tempoTotalEspera = 0.0f;
    fila->somaTamanhosFila = 0.0f;
    fila->amostrasTamanhoFila = 0;
}

