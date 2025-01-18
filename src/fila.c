#include "../include/fila.h"

/**
 * Inicializa uma fila vazia.
 * Configura os ponteiros de início e fim como NULL e zera as variáveis estatísticas.
 */
void inicializaFila(Fila *fila) {
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
void enfileira(Fila *fila, Paciente *paciente, float tempoAtual) {
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
    if (fila->fim) {
        fila->fim->proximo = novoNo; // Adiciona o novo nó após o último nó existente
    } else {
        fila->inicio = novoNo; // Caso a fila esteja vazia, o novo nó será o início
    }
    fila->fim = novoNo; // Atualiza o ponteiro para o fim da fila

    // Atualiza estatísticas da fila
    fila->tamanho++;
    fila->somaTamanhosFila += fila->tamanho; // Soma o tamanho atual da fila
    fila->amostrasTamanhoFila++; // Incrementa o número de amostras coletadas
}

/**
 * Remove e retorna o paciente do início da fila.
 * Atualiza o tempo de espera do paciente e as estatísticas da fila.
 */
Paciente *desenfileira(Fila *fila, float tempoAtual) {
    // Verifica se a estrutura da fila é válida
    erroAssert(fila != NULL, "Tentativa de desenfileirar de uma fila nula.");
    
    // Retorna NULL se a fila estiver vazia
    if (filaVazia(fila)) {
        return NULL;
    }

    // Remove o primeiro nó da fila
    NoFila *removido = fila->inicio;
    Paciente *paciente = removido->paciente;

    // Calcula o tempo de espera do paciente
    erroAssert(tempoAtual >= removido->tempoEntrada, "Tempo atual inválido para desenfileirar.");
    float tempoEspera = tempoAtual - removido->tempoEntrada;
    fila->tempoTotalEspera += tempoEspera; // Atualiza o tempo total de espera
    registraTempoEspera(paciente, tempoEspera); // Registra o tempo de espera no paciente

    // Atualiza o ponteiro de início da fila
    fila->inicio = removido->proximo;
    if (!fila->inicio) {
        fila->fim = NULL; // Se a fila ficou vazia, atualiza o ponteiro do fim
    }

    // Libera a memória do nó removido
    free(removido);

    // Decrementa o tamanho da fila
    fila->tamanho--;

    // Retorna o paciente removido
    return paciente;
}

/**
 * Verifica se a fila está vazia.
 * Retorna 1 se a fila estiver vazia, 0 caso contrário.
 */
int filaVazia(Fila *fila) {
    // Verifica se a estrutura da fila é válida
    erroAssert(fila != NULL, "Tentativa de verificar se uma fila nula está vazia.");
    return fila->inicio == NULL;
}

/**
 * Finaliza a fila, liberando todos os nós alocados.
 * Ignora o tempo atual ao remover os pacientes.
 */
void finalizaFila(Fila *fila) {
    // Verifica se a estrutura da fila é válida
    erroAssert(fila != NULL, "Tentativa de finalizar uma fila nula.");

    // Remove todos os nós da fila
    while (!filaVazia(fila)) {
        desenfileira(fila, 0.0f); // O tempo atual não é relevante para a finalização
    }
}

/**
 * Calcula as estatísticas da fila.
 * Retorna o tempo médio de espera e o tamanho médio da fila.
 */
void calculaEstatisticasFila(Fila *fila, float *tempoMedioEspera, float *tamanhoMedioFila) {
    // Verifica se a estrutura da fila é válida
    erroAssert(fila != NULL, "Tentativa de calcular estatísticas de uma fila nula.");

    // Calcula o tempo médio de espera
    *tempoMedioEspera = fila->amostrasTamanhoFila > 0 ? fila->tempoTotalEspera / fila->amostrasTamanhoFila : 0.0f;

    // Calcula o tamanho médio da fila
    *tamanhoMedioFila = fila->amostrasTamanhoFila > 0 ? fila->somaTamanhosFila / fila->amostrasTamanhoFila : 0.0f;
}
