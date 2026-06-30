#ifndef SISTEMA_TRIAGEM_HPP
#define SISTEMA_TRIAGEM_HPP
 
#include "fila.hpp"
#include "paciente.hpp"
#include <stdexcept>
 
// ------------------------------------------------------------------
// SistemaTriagem
//
// Mantém 5 FilaEncadeada<Paciente> (uma por nível de Manchester).
// O atendente chama proximoAtendimento() e recebe sempre o paciente
// de maior prioridade disponível, respeitando a regra anti-inanição.
//
// Regra anti-inanição (starvation prevention):
//   A cada <limiteAntiInanicao> pacientes atendidos das filas de
//   prioridade 0-2 (Vermelho/Laranja/Amarelo), obrigatoriamente
//   serve 1 paciente da fila de menor prioridade disponível
//   (Verde ou Azul), se houver.
// ------------------------------------------------------------------
class SistemaTriagem {
private:
    static const int NUM_NIVEIS = 5;
 
    FilaEncadeada<Paciente> filas[NUM_NIVEIS];
    LogAtendimentos&        log;
 
    int  limiteAntiInanicao;    // padrão: 10
    int  contadorUrgentes;      // urgentes atendidos desde último "verde/azul"
    double relogioAtual;        // tempo de simulação (preenchido externamente)
 
public:
    SistemaTriagem(LogAtendimentos& log, int limiteAntiInanicao = 10)
        : log(log),
          limiteAntiInanicao(limiteAntiInanicao),
          contadorUrgentes(0),
          relogioAtual(0.0) {}
 
    // Adiciona paciente à fila correta
    void chegada(Paciente p) {
        int idx = static_cast<int>(p.categoria);
        filas[idx].enqueue(p);
    }
 
    // Avança o relógio interno (chamado pela simulação da Pessoa B)
    void setRelogio(double t) { relogioAtual = t; }
 
    // Retorna true e preenche 'atendido' se há alguém na fila
    bool proximoAtendimento(Paciente& atendido) {
        // Verifica se deve forçar anti-inanição (Verde ou Azul)
        if (contadorUrgentes >= limiteAntiInanicao) {
            for (int i = NUM_NIVEIS - 1; i >= 3; i--) {
                if (!filas[i].vazia()) {
                    atendido = filas[i].dequeue();
                    atendido.timestamp_saida = relogioAtual;
                    log.registrar(atendido);
                    contadorUrgentes = 0;
                    return true;
                }
            }
            // Nenhum verde/azul disponível: reseta contador e continua normal
            contadorUrgentes = 0;
        }
 
        // Atende pela prioridade: Vermelho → Laranja → Amarelo → Verde → Azul
        for (int i = 0; i < NUM_NIVEIS; i++) {
            if (!filas[i].vazia()) {
                atendido = filas[i].dequeue();
                atendido.timestamp_saida = relogioAtual;
                log.registrar(atendido);
                if (i <= 2) contadorUrgentes++;  // urgente atendido
                else        contadorUrgentes = 0; // verde/azul "normal" reseta
                return true;
            }
        }
        return false;  // todas as filas vazias
    }
 
    bool todasVazias() const {
        for (int i = 0; i < NUM_NIVEIS; i++)
            if (!filas[i].vazia()) return false;
        return true;
    }
 
    // Estatísticas de tamanho máximo por nível
    int getTamanhoMaximo(Categoria c) const {
        return filas[static_cast<int>(c)].getTamanhoMaximo();
    }
 
    int getTamanhoAtual(Categoria c) const {
        return filas[static_cast<int>(c)].getTamanho();
    }
 
    int getLimiteAntiInanicao() const { return limiteAntiInanicao; }
    void setLimiteAntiInanicao(int lim) { limiteAntiInanicao = lim; }
};

#endif