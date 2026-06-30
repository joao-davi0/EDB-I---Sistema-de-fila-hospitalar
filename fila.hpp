#ifndef FILA_ENCADEADA_HPP
#define FILA_ENCADEADA_HPP
 
#include <stdexcept>
 
template <typename T>
class FilaEncadeada {
private:
    struct No {
        T dado;
        No* prox;
        No(const T& d) : dado(d), prox(nullptr) {}
    };
 
    No* cabeca;
    No* cauda;
    int tamanho;
    int tamanho_maximo;
 
public:
    FilaEncadeada() : cabeca(nullptr), cauda(nullptr), tamanho(0), tamanho_maximo(0) {}
 
    ~FilaEncadeada() {
        while (!vazia()) dequeue();
    }
 
    // O(1) — insere no fim usando ponteiro de cauda
    void enqueue(const T& dado) {
        No* novo = new No(dado);
        if (cauda == nullptr) {
            cabeca = cauda = novo;
        } else {
            cauda->prox = novo;
            cauda = novo;
        }
        tamanho++;
        if (tamanho > tamanho_maximo)
            tamanho_maximo = tamanho;
    }
 
    // O(1) — remove do início usando ponteiro de cabeça
    T dequeue() {
        if (vazia())
            throw std::runtime_error("Fila vazia");
        No* temp = cabeca;
        T dado = temp->dado;
        cabeca = cabeca->prox;
        if (cabeca == nullptr)
            cauda = nullptr;
        delete temp;
        tamanho--;
        return dado;
    }
 
    // O(1)
    T& frente() {
        if (vazia())
            throw std::runtime_error("Fila vazia");
        return cabeca->dado;
    }
 
    bool vazia() const { return tamanho == 0; }
    int getTamanho() const { return tamanho; }
    int getTamanhoMaximo() const { return tamanho_maximo; }
};
 
#endif