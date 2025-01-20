#include "../include/procedimento.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void inicializaProcedimento(Procedimento *procedimento, int numUnidades, float tempoMedioExecucao)
{
    assert(procedimento != NULL);
    assert(numUnidades > 0);
    assert(tempoMedioExecucao > 0);

    procedimento->unidades = (Unidade *)malloc(numUnidades * sizeof(Unidade));
    assert(procedimento->unidades != NULL);

    procedimento->numUnidades = numUnidades;
    procedimento->tempoMedioExecucao = tempoMedioExecucao;

    for (int i = 0; i < numUnidades; i++)
    {
        procedimento->unidades[i].id = i;
        procedimento->unidades[i].ocupado = 0;
        procedimento->unidades[i].tempoUltimoServico = 0.0f;
        procedimento->unidades[i].tempoOcioso = 0.0f;
    }
}

void finalizaProcedimento(Procedimento *procedimento)
{
    assert(procedimento != NULL);

    free(procedimento->unidades);
    procedimento->unidades = NULL;
    procedimento->numUnidades = 0;
    procedimento->tempoMedioExecucao = 0.0f;
}

int encontraUnidadeDisponivel(Procedimento *procedimento, float tempoAtual)
{
    assert(procedimento != NULL);

    for (int i = 0; i < procedimento->numUnidades; i++)
    {
        if (tempoAtual >= procedimento->unidades[i].tempoUltimoServico)
        {
            return i;
        }
    }
    return -1; // Nenhuma unidade disponível
}

void alocaUnidade(Procedimento *procedimento, int indice, float tempoAtual, float duracaoServico)
{
    assert(procedimento != NULL);
    assert(indice >= 0 && indice < procedimento->numUnidades);

    Unidade *unidade = &procedimento->unidades[indice];

    // Atualiza tempo ocioso antes de alocar
    if (tempoAtual > unidade->tempoUltimoServico)
    {
        unidade->tempoOcioso += tempoAtual - unidade->tempoUltimoServico;
    }

    unidade->ocupado = 1;
    unidade->tempoUltimoServico = tempoAtual + duracaoServico;
}

void liberaUnidade(Procedimento *procedimento, int indice, float tempoAtual)
{
    assert(procedimento != NULL);
    assert(indice >= 0 && indice < procedimento->numUnidades);

    Unidade *unidade = &procedimento->unidades[indice];

    // Atualiza tempo ocioso caso a unidade tenha ficado desocupada
    if (tempoAtual > unidade->tempoUltimoServico)
    {
        unidade->tempoOcioso += tempoAtual - unidade->tempoUltimoServico;
    }

    unidade->ocupado = 0;
}
