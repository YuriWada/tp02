#ifndef PROCEDIMENTO_H
#define PROCEDIMENTO_H

typedef struct
{
    int tipo; // 1.Triagem 2.Atendimento 3.MedidasHosp 4.TesteLab 5.ExamesImag 6.Instrumentos/Med 
    int numTipo; // Quantidade de atendentes para o procedimento
    float ocioso; // Tempo ocioso da unidade
    float tempo; // DataHora até fim de ocupacao
} Procedimento;

#endif /** PROCEDIMENTO_H */