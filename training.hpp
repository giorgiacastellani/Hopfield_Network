#ifndef TRAINING_HPP
#define TRAINING_HPP

#include "acquisition.hpp"
#include <cstddef> //prevede il tipo size_t
#include <filesystem>
#include <vector>
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
  // Costruttore (lo lascio solo nel hpp perché se no lo vede come doppia
  // definizione e dà errore di compilazione)...patterns_ verrà inzializzato
  // come un vettore di dimensione 0
  NetworkTrainer(std::size_t pattern_size)
      : pattern_size_(pattern_size)
  {}

  // METODI DELLA CLASSE E UTILITY
  // a partire da https://cppreference.com/cpp/container/vector
  // prendo i metodi che penso mi possano servire e li adatto alla classe
  // il metodo principale sarà quello di addestramento tramite matrice

  // modificatore
  void addPattern(const HopfieldPattern& pattern);

  std::size_t getPatternCount() const;

  std::size_t getPatternSize() const;

  bool empty() const;
  void clear();

  // accesso agli elementi
  const HopfieldPattern& operator[](std::size_t index) const;

  // METODO PRINCIPALE
  // Calcola la matrice dei pesi W come matrice 2D (vettore di vettori N x N)
  std::vector<std::vector<double>>
  train() const; // è vuota in argomento perché tutto ciò che serve è già
                 // contenuto nella classe NetworkTrainer su cui chiamiamo train

  void printWeights(const std::vector<std::vector<double>>& weights) const;

  bool saveWeightsToFile(const std::filesystem::path& filepath,
                         const std::vector<std::vector<double>>& weights) const;
};

} // namespace Hopfield

#endif // TRAINING_HPP