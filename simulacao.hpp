#ifndef SIMULACAO_HPP
#define SIMULACAO_HPP

#include "sistema_triagem.hpp"
#include <queue>

// Sorteia categoria segundo a distribuição do enunciado
Categoria sortearCategoria();

// Gera pacientes e os coloca no sistema
void gerarPacientes(SistemaTriagem& sistema,
                    int quantidade,
                    double intervaloMedio = 1.0);

// Simula o atendimento
double simularAtendimentos(SistemaTriagem& sistema,
                           int quantidade,
                           double intervaloMedio = 1.0,
                           double tempoAtendimento = 3.0);

// Imprime estatísticas
void imprimirMetricas(const LogAtendimentos& log,
                      const SistemaTriagem& sistema);

// Compara com fila FIFO simples
void compararFilaUnica(int quantidade,
                       double intervaloMedio = 1.0,
                       double tempoAtendimento = 3.0);

// Gera arquivo CSV para construção do gráfico
void gerarCSV(const LogAtendimentos& log,
              const char* nomeArquivo = "grafico.csv");

#endif
