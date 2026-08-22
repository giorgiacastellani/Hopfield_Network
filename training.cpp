#include "training.hpp"
#include <fstream> //lo usiamo in saveWeightsToFile
#include <iomanip>
#include <iostream>
#include <stdexcept> // serve per la gestione delle due clausole di addPattern
namespace Hopfield {

// METODI NECESSARI ALLA GESTIONE DI BASE DELLA CLASSE E PROPEDEUTICI A
// train E saveWeightsToFile:

//  modificatore
void NetworkTrainer::addPattern(const HopfieldPattern& pattern)
{
  // 1. Controllo dimensione N
  if (pattern.getSize() != pattern_size_) {
    throw std::invalid_argument("Dimensione del pattern non valida!");
  }

  // 2. Controllo presenza esclusiva di +1 e -1
  // ciclo for per scorrere tra i pixel di un pattern
  for (std::size_t i = 0; i < pattern.getSize(); ++i) {
    if (pattern[i] != 1 && pattern[i] != -1) {
      throw std::invalid_argument(
          "Il pattern contiene valori diversi da +1 e -1!");
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
  return patterns_[index]; // dati P pattern nel mio vettore di pattern, a quale
                           // faccio riferimento? index appartiene a [0;P-1]
}

// METODI IMPORTANTI E CARATTERISTICI DELLA CLASSE

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

// gemini lo consiglia per verificare facilmente che la matrice sia simmetrica e
// per controllarla prima di stamparla su file (lo inseriamo nel main)
void NetworkTrainer::printWeights(
    const std::vector<std::vector<double>>& weights) const
{
  if (weights.empty()) {
    std::cout << "La matrice dei pesi e' vuota!\n";
    return;
  }

  std::size_t N =
      weights.size(); // usando pattern_size_ anziché N nel ciclo non impedisco
                      // che si passi al metodo una qualsiasi matrice...che non
                      // sia affatto il risultato di train e quindi N x N

  std::cout << "\n Matrice dei Pesi W di dimensione" << N << "x" << N << "\n";

  for (std::size_t i = 0; i < N; ++i) {
    if (weights[i].size() != N) {
      std::cout << "La matrice fornita non è quadrata! \n";
      return;
    }
    for (std::size_t j = 0; j < N; ++j) {
      // std::fixed e std::setprecision(2) bloccano i decimali a 2 cifre
      // std::setw(7) allinea i numeri in colonne larghe 7 caratteri
      std::cout << std::fixed << std::setprecision(2)
                << std::setw(7) // gemini puro
                << weights[i][j] << " ";
    }
    std::cout << "\n"; // Vada a capo alla fine di ogni riga
  }
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

  std::size_t N = weights.size(); // weights è un vettore di vettori e
                                  // quindi posso usare il metodo di <vector>

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

} // namespace Hopfield
