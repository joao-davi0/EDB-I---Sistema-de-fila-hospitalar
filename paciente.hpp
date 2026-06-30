#ifndef PACIENTE_HPP
#define PACIENTE_HPP
 
#include <string>
#include <ctime>
 
// Níveis do Protocolo de Manchester (menor índice = maior prioridade)
enum class Categoria {
    VERMELHO = 0,   // Imediato
    LARANJA  = 1,   // Muito urgente
    AMARELO  = 2,   // Urgente
    VERDE    = 3,   // Pouco urgente
    AZUL     = 4    // Não urgente
};
 
inline std::string nomeCategoria(Categoria c) {
    switch (c) {
        case Categoria::VERMELHO: return "Vermelho";
        case Categoria::LARANJA:  return "Laranja";
        case Categoria::AMARELO:  return "Amarelo";
        case Categoria::VERDE:    return "Verde";
        case Categoria::AZUL:     return "Azul";
    }
    return "?";
}
 
struct Paciente {
    int id;
    Categoria categoria;
    double timestamp_entrada;   // segundos desde início da simulação
    double timestamp_saida;     // preenchido após atendimento
 
    Paciente() : id(0), categoria(Categoria::AZUL),
                 timestamp_entrada(0), timestamp_saida(-1) {}
 
    Paciente(int id, Categoria cat, double entrada)
        : id(id), categoria(cat),
          timestamp_entrada(entrada), timestamp_saida(-1) {}
 
    double tempoEspera() const {
        if (timestamp_saida < 0) return -1;
        return timestamp_saida - timestamp_entrada;
    }
};
 
// ------------------------------------------------------------------
// Lista encadeada de atendimentos (log histórico)
// ------------------------------------------------------------------
struct NoLog {
    Paciente paciente;
    NoLog* prox;
    NoLog(const Paciente& p) : paciente(p), prox(nullptr) {}
};
 
class LogAtendimentos {
private:
    NoLog* cabeca;
    NoLog* cauda;
    int total;
 
public:
    LogAtendimentos() : cabeca(nullptr), cauda(nullptr), total(0) {}
 
    ~LogAtendimentos() {
        NoLog* cur = cabeca;
        while (cur) {
            NoLog* prox = cur->prox;
            delete cur;
            cur = prox;
        }
    }
 
    void registrar(const Paciente& p) {
        NoLog* novo = new NoLog(p);
        if (!cauda) { cabeca = cauda = novo; }
        else        { cauda->prox = novo; cauda = novo; }
        total++;
    }
 
    int getTotal() const { return total; }
 
    // Itera pelo log e calcula tempo médio por categoria
    // Retorna array indexado por (int)Categoria
    void calcularMedias(double medias[5], int contagens[5]) const {
        for (int i = 0; i < 5; i++) { medias[i] = 0; contagens[i] = 0; }
        NoLog* cur = cabeca;
        while (cur) {
            int idx = static_cast<int>(cur->paciente.categoria);
            medias[idx]    += cur->paciente.tempoEspera();
            contagens[idx] += 1;
            cur = cur->prox;
        }
        for (int i = 0; i < 5; i++)
            if (contagens[i] > 0)
                medias[i] /= contagens[i];
    }
};
 
#endif