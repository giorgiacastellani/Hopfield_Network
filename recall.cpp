#include "recall.hpp"
#include <vector>
#include <stdexcept>
#include <iostream>

namespace Hopfield {

double calculateEnergy(const HopfieldPattern& pattern,
                       const std::vector<std::vector<double>>& weights)
{
  std::size_t N = pattern.getSize();

  if (weights.empty() || weights.size() != N) {
    throw std::invalid_argument(
        "Errore: dimensioni della matrice e del pattern non coincidenti!");
  }

  double energy = 0.0;

  for (std::size_t i = 0; i < N; ++i) {
    for (std::size_t j = 0; j < N; ++j) {
      energy += weights[i][j] * pattern[i] * pattern[j];
    }
  }

  return -0.5 * energy;
}

void patternUpdate(Hopfield::HopfieldPattern& pattern,
                   std::vector<std::vector<double>>& weights)
{
  std::size_t N = pattern.getSize();

  if (weights.empty() || weights.size() != N) {
    throw std::invalid_argument("Errore: la matrice dei pesi non e' valida!");
  }

  bool converged = false;

  double current_energy = calculateEnergy(pattern, weights);
  std::cout << "Energia iniziale (corrotto): " << current_energy << "\n";

  while (!converged) {
    converged = true; // assumiamo che la rete sia stabile al principio di ogni
                      // iterazione

    for (size_t i = 0; i < N; ++i) {
      if (weights[i].size() != N) {
        throw std::invalid_argument(
            "Errore: la matrice dei pesi non è quadrata!");
      }

      double sum = 0.0;

      // Somma pesata: W_ij * s_j
      for (std::size_t j = 0; j < N; ++j) {
        sum += weights[i][j] * pattern[j];
      }

      // Calcolo del nuovo stato
      int new_state = 0;
      if (sum >= 0.0) {
        new_state = 1;
      } else {
        new_state = -1;
      }

      // Se il neurone cambia stato, aggiorniamo e continuiamo il while
      if (new_state != pattern[i]) {
        pattern[i] = new_state;
        converged  = false;
      }
    }

    double new_energy = calculateEnergy(pattern, weights);
    std::cout << "Energia al momento: " << new_energy << "\n";
  }
}
} // namespace Hopfield