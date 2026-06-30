#include "simulacao.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>

Categoria sortearCategoria() {
    int r = rand() % 100;

    if (r < 5)
        return Categoria::VERMELHO;

    if (r < 20)
        return Categoria::LARANJA;

    if (r < 50)
        return Categoria::AMARELO;

    if (r < 85)
        return Categoria::VERDE;

    return Categoria::AZUL;
}

void gerarPacientes(SistemaTriagem& sistema,
                    int quantidade,
                    double intervaloMedio)
{
    for (int i = 1; i <= quantidade; i++)
    {
        double chegada = i * intervaloMedio;

        Paciente p(i,
                   sortearCategoria(),
                   chegada);

        sistema.chegada(p);
    }
}

double simularAtendimentos(SistemaTriagem& sistema,
                           int quantidade,
                           double intervaloMedio,
                           double tempoAtendimento)
{
    double relogio = quantidade * intervaloMedio;

    Paciente paciente;

    while (!sistema.todasVazias())
    {
        relogio += tempoAtendimento;

        sistema.setRelogio(relogio);

        sistema.proximoAtendimento(paciente);
    }

    return relogio;
}

void imprimirMetricas(const LogAtendimentos& log,
                      const SistemaTriagem& sistema)
{
    double medias[5];
    int contagens[5];

    log.calcularMedias(medias, contagens);

    Categoria categorias[5] =
    {
        Categoria::VERMELHO,
        Categoria::LARANJA,
        Categoria::AMARELO,
        Categoria::VERDE,
        Categoria::AZUL
    };

    std::cout << "\n";

    std::cout
        << std::left
        << std::setw(12) << "Categoria"

        << std::right
        << std::setw(12) << "Pacientes"

        << std::setw(18) << "Espera media"

        << std::setw(15) << "Fila maxima"

        << "\n";

    std::cout << std::string(57,'-') << "\n";

    for(int i=0;i<5;i++)
    {
        std::cout
            << std::left
            << std::setw(12)
            << nomeCategoria(categorias[i])

            << std::right
            << std::setw(12)
            << contagens[i]

            << std::setw(17)
            << std::fixed
            << std::setprecision(2)
            << medias[i]

            << "s"

            << std::setw(12)
            << sistema.getTamanhoMaximo(categorias[i])

            << "\n";
    }

    std::cout
        << "\nTotal atendidos: "
        << log.getTotal()
        << "\n";
}

void compararFilaUnica(int quantidade,
                       double intervaloMedio,
                       double tempoAtendimento)
{
    std::queue<Paciente> fila;

    for(int i=1;i<=quantidade;i++)
    {
        fila.push(
            Paciente(
                i,
                sortearCategoria(),
                i*intervaloMedio
            )
        );
    }

    double relogio = 0;

    double soma = 0;

    int atendidos = 0;

    while(!fila.empty())
    {
        relogio += tempoAtendimento;

        Paciente p = fila.front();

        fila.pop();

        p.timestamp_saida = relogio;

        soma += p.tempoEspera();

        atendidos++;
    }

    std::cout
        << "\nFila unica (std::queue)\n";

    std::cout
        << "Tempo medio de espera: "
        << std::fixed
        << std::setprecision(2)
        << soma/atendidos
        << " s\n";
}

void gerarCSV(const LogAtendimentos& log,
              const char* nomeArquivo)
{
    std::ofstream arq(nomeArquivo);

    if(!arq.is_open())
        return;

    double medias[5];
    int contagens[5];

    log.calcularMedias(medias,contagens);

    arq << "Categoria,Pacientes,TempoMedio\n";

    Categoria categorias[5]=
    {
        Categoria::VERMELHO,
        Categoria::LARANJA,
        Categoria::AMARELO,
        Categoria::VERDE,
        Categoria::AZUL
    };

    for(int i=0;i<5;i++)
    {
        arq
            << nomeCategoria(categorias[i])
            << ","
            << contagens[i]
            << ","
            << medias[i]
            << "\n";
    }

    arq.close();

    std::cout
        << "\nArquivo "
        << nomeArquivo
        << " gerado com sucesso.\n";
}