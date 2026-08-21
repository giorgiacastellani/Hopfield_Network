#include "Network.hpp"
#include <fstream> //serve per leggere e scrivere file
#include <stdexcept>
namespace Hopfield {
HopfieldNetwork::HopfieldNetwork()
{
  N_       = 0;
  weights_ = std::vector<double>();
}
// METODO train
// Calcola la matrice W con la Regola di Hebb.
// Ricava automaticamente N_ dalla dimensione del primo pattern fornito.
// ============================================================================
void HopfieldNetwork::train(const std::vector<HopfieldPattern>& patterns)
{
  // verifichiamo che ci sia almeno un'immagine (forse non necessario)
  if (patterns.empty()) {
    throw std::invalid_argument(
        "Impossibile addestrare: il dataset di pattern e' vuoto!");
  }

  // ricaviamo la dimensione N_ direttamente dalla prima immagine, così non lo
  // imponiamo a priori ma è da capire come vogliamo strutturare il progetto
  N_ = patterns[0].getSize();

  // matrice dei pesi N x N riempita di zeri
  weights_ = std::vector<double>(N_ * N_, 0.0);

  // P e' il numero totale di immagini memorizzate
  std::size_t P = patterns.size();

  // Calcolo della Regola di Hebb per ogni elemento W_ij
  for (std::size_t i = 0; i < N_; ++i) {
    for (std::size_t j = 0; j < N_; ++j) {
      if (i == j) {
        // Diagonale nulla
        weights_[i * N_ + j] = 0.0; //formula matematica per calcolare l'indice del vettore 1 dim
      } else {
        double sum = 0.0;

        // Somma del prodotto dei neuroni su tutti i P pattern
        for (std::size_t mu = 0; mu < P; ++mu) {
          sum += patterns[mu][i] * patterns[mu][j];
        }

        // Normalizzazione dividendo per N_
        weights_[i * N_ + j] = sum / static_cast<double>(N_);
      }
    }
  }
}
// BOZZA DI GEMINI SULLA PARTE DOPO, MA IO NON CI SONO ANCORA ARRIVATA, INVECE
// LA PARTE SOPRA VA BENE
//  Scrive la dimensione N_ e poi tutti i pesi della matrice W su file.
//  ============================================================================
void HopfieldNetwork::saveWeights(const std::string& filename) const
{
  std::ofstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error(
        "Errore: impossibile aprire il file per salvare la matrice pesi!");
  }

  // Scriviamo N_ nella prima riga
  file << N_ << "\n";

  // Scriviamo i pesi riga per riga
  for (std::size_t i = 0; i < N_; ++i) {
    for (std::size_t j = 0; j < N_; ++j) {
      file << weights_[i * N_ + j] << " ";
    }
    file << "\n";
  }
}

// ============================================================================
// METODO loadWeights
// Legge N_ dal file, adatta la dimensione del vettore e carica i pesi W.
// ============================================================================
void HopfieldNetwork::loadWeights(const std::string& filename)
{
  std::ifstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error(
        "Errore: impossibile aprire il file della matrice pesi!");
  }

  std::size_t loaded_N = 0;
  file >> loaded_N; // Legge il numero di neuroni salvato nel file

  if (loaded_N == 0) {
    throw std::runtime_error("File corrotto o dimensione N non valida.");
  }

  // Aggiorniamo N_ e ridimensioniamo il vettore dei pesi
  N_       = loaded_N;
  weights_ = std::vector<double>(N_ * N_, 0.0);

  // Leggiamo in sequenza gli N x N pesi
  for (std::size_t k = 0; k < N_ * N_; ++k) {
    file >> weights_[k];
  }
}

// ============================================================================
// METODO getWeight
// Restituisce il valore del peso W_ij posizionato alla riga i e colonna j.
// ============================================================================
double HopfieldNetwork::getWeight(std::size_t i, std::size_t j) const
{
  if (i >= N_ || j >= N_) {
    throw std::out_of_range("Indici i o j fuori dai limiti della matrice!");
  }
  return weights_[i * N_ + j];
}
} // namespace Hopfield