#include "training.hpp"

#include <fstream>
#include <stdexcept> // Necessario per scrivere su file
namespace Hopfield {
NetworkTrainer::NetworkTrainer(std::size_t pattern_size)
    : pattern_size_(pattern_size)
{}

// METODI DELLA CLASSE E UTILITY
// a partire da https://cppreference.com/cpp/container/vector
// prendo i metodi che penso mi possano servire e li adatto alla classe
// il metodo principale sarà quello di addestramento tramite matrice

// modificatore
void NetworkTrainer::addPattern(const HopfieldPattern& pattern)
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
std::size_t NetworkTrainer::getPatternCount() const
{
  return patterns_.size();
}

std::size_t NetworkTrainer::getPatternSize() const
{
  return pattern_size_;
}

bool NetworkTrainer::empty() const
{
  return patterns_.empty();
}
// equivarrebbe a ricominciare da capo "l'addestramento"
void NetworkTrainer::clear()
{
  patterns_.clear();
}
// accesso agli elementi
const HopfieldPattern& NetworkTrainer::operator[](std::size_t index) const
{
  return patterns_[index];
}

// METODO PRINCIPALE
// Calcola la matrice dei pesi W come matrice 2D (vettore di vettori N x N)
std::vector<std::vector<double>> NetworkTrainer::train()
    const // è vuota in argomento perché tutto ciò che serve è già
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
bool NetworkTrainer::saveWeightsToFile(
    const std::filesystem::path& filepath,
    const std::vector<std::vector<double>>& weights) const
{
  // Apre il file di output
  std::ofstream file(filepath);

  // Controlla che il file si sia aperto correttamente
  if (!file.is_open()) {
    return false;
  }

  std::size_t N = weights.size();

  // Scrive N nella prima riga (dimensione della matrice N x N)
  file << N << "\n";

  // Ciclo semplice riga per riga
  for (std::size_t i = 0; i < N; ++i) {
    for (std::size_t j = 0; j < N; ++j) {
      file << weights[i][j] << " ";
    }
    file << "\n"; // Vada a capo a fine riga
  }

  return true;
}

// ora devo: separare dichiarazioni da definizioni e metterle nell'.hpp già
// creato, scrivere tutti i test per questi metodi e poi creare un metodo che
// salvi la matrice dei pesi ed eventualmente uno che la stampi a schermo
// (secondo Gemini ma bo)
}; // namespace Hopfield

; // namespace Hopfield
