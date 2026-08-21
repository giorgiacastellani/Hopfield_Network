#ifndef TRAINING_HPP
#define TRAINING_HPP

#include "acquisition.hpp"

namespace Hopfield {

class NetworkTrainer
{
 private:
  std::size_t pattern_size_; // N : # componenti di un singolo pattern
  std::vector<HopfieldPattern>
      patterns_; // vettore di oggetti HopfieldPattern (width,height,data) dove
                 // N=data.getSize() Possiamo sfruttare tutti i metodi
                 // introdotti finora sugli elementi di questo vettore di
                 // vettori, es.binarize

 public:
  // Costruttore...patterns_ verrà inzializzato come un vettore di dimensione 0
  NetworkTrainer(std::size_t pattern_size)
      : pattern_size_(pattern_size)
  {}

  // METODI DELLA CLASSE E UTILITY
  // a partire da https://cppreference.com/cpp/container/vector
  // prendo i metodi che penso mi possano servire e li adatto alla classe
  // il metodo principale sarà quello di addestramento tramite matrice

  // modificatore
  void addPattern(const HopfieldPattern& pattern)
  {
    // 1. Controllo dimensione N
    if (pattern.getSize() != pattern_size_) {
      return;
    }

    // 2. Controllo presenza esclusiva di +1 e -1
    // ciclo for per scorrere tra i pixel di un pattern
    for (std::size_t i = 0; i < pattern.getSize(); ++i) {
      if (pattern[i] != 1 && pattern[i] != -1) {
        return;
      }
    }

    patterns_.push_back(pattern);
  }

  // gestione di dimensioni e dello stato
  std::size_t getPatternCount() const
  {
    return patterns_.size();
  }

  std::size_t getPatternSize() const
  {
    return pattern_size_;
  }

  bool empty() const
  {
    return patterns_.empty();
  }

  // accesso agli elementi
  const HopfieldPattern& operator[](std::size_t index) const
  {
    return patterns_[index];
  }

  // equivarrebbe a ricominciare da capo "l'addestramento"
  void clear()
  {
    patterns_.clear();
  }

  // METODO PRINCIPALE
  // Calcola la matrice dei pesi W come matrice 2D (vettore di vettori N x N)
  std::vector<std::vector<double>>
  train() const // è vuota in argomento perché tutto ciò che serve è già
                // contenuto nella classe NetworkTrainer su cui chiamiamo train
  {
    // Verifica che ci sia almeno un pattern caricato
    if (patterns_.empty()) {
      throw std::invalid_argument(
          "Impossibile addestrare: il dataset di pattern e' vuoto!");
    }

    std::size_t N = pattern_size_;
    std::size_t P = patterns_.size();

    // Inizializza una matrice 2D di dimensione N x N riempita di zeri
    std::vector<std::vector<double>> weights(N, std::vector<double>(N, 0.0));

    // Calcolo della Regola di Hebb con sintassi a due indici [i][j]
    for (std::size_t i = 0; i < N; ++i) {
      for (std::size_t j = 0; j < N; ++j) {
        if (i == j) {
          // Diagonale nulla (nessuna autoconnessione)
          weights[i][j] = 0.0;
        } else {
          double sum = 0.0;

          // Somma del prodotto dei neuroni su tutti i P pattern
          for (std::size_t mu = 0; mu < P; ++mu) {
            // patterns_[mu][i] accede al neurone i-esimo del pattern mu
            sum += patterns_[mu][i] * patterns_[mu][j];
          }

          // Normalizzazione dividendo per N
          weights[i][j] = sum / static_cast<double>(N);
        }
      }
    }

    return weights;
  }

  // ora devo: separare dichiarazioni da definizioni e metterle nell'.hpp già
  // creato, scrivere tutti i test per questi metodi e poi creare un metodo che
  // salvi la matrice dei pesi ed eventualmente uno che la stampi a schermo
  // (secondo Gemini ma bo)
};

}; // namespace Hopfield

#endif // TRAINING_HPP