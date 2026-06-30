#include <iostream>
#include <cstdlib>
#include <ctime>

#include "paciente.hpp"
#include "sistema_triagem.hpp"
#include "simulacao.hpp"

int main(int argc, char* argv[]) {
    srand(static_cast<unsigned>(time(nullptr)));

    int quantidade = 1000;
    int limiteAntiInanicao = 10;

    if (argc >= 2)
        quantidade = std::atoi(argv[1]);

    if (argc >= 3)
        limiteAntiInanicao = std::atoi(argv[2]);

    std::cout << "=========================================\n";
    std::cout << " Sistema de Triagem Hospitalar\n";
    std::cout << " Protocolo de Manchester\n";
    std::cout << "=========================================\n\n";

    std::cout << "Pacientes simulados : "
              << quantidade << "\n";

    std::cout << "Anti-inaniacao      : "
              << limiteAntiInanicao
              << " urgentes\n";

    std::cout << "\nIniciando simulacao...\n";

    // Sistema com prioridade
    LogAtendimentos log;

    SistemaTriagem sistema(log,
                           limiteAntiInanicao);

    gerarPacientes(sistema,
                   quantidade);

    double tempoTotal =
        simularAtendimentos(sistema,
                            quantidade);

    std::cout
        << "\nSimulacao concluida.\n";

    std::cout
        << "Tempo total: "
        << tempoTotal
        << " s\n";

    imprimirMetricas(log,
                     sistema);


    // Comparação com fila FIFO
    std::cout
        << "\n=========================================\n";
    std::cout
        << "Comparação com fila única\n";
    std::cout
        << "=========================================\n";

    compararFilaUnica(quantidade);

    // CSV para gráfico
    gerarCSV(log);

    std::cout
        << "\nO arquivo gráfico.csv foi criado.\n";

    std::cout
        << "Ele pode ser aberto no Excel,\n";
    std::cout
        << "LibreOffice Calc ou Google Planilhas\n";
    std::cout
        << "para gerar o gráfico solicitado.\n";

    return 0;
}