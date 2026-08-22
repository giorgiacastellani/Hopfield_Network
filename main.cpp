#include "acquisition.hpp"
#include "training.hpp" //vedi giacomini come vuole gli include
#include <iostream>
#include <string>
#include <vector>

int main()
{ // 1. Configurazione Dimensioni
  unsigned int targetWidth  = 400;
  unsigned int targetHeight = 400;
  std::size_t N             = targetWidth * targetHeight; // numero di neuroni
  std::size_t P = 5; // numero di immagini da caricare, deve corrispondere alla
                     // size di patterns_

  Hopfield::NetworkTrainer trainer(N);

  std::cout << "ACQUISIZIONE DEI PATTERN \n";

  for (std::size_t i = 0; i < P; ++i) {
    sf::Image original_image;

    // costruiamo il nome del file: "img0.png", "img1.png" etc
    std::string input_filename =
        "img" + std::to_string(i)
        + ".png"; // https://cppreference.com/cpp/string/basic_string/to_string

    if (!original_image.loadFromFile(input_filename)) {
      std::cerr << "Errore: impossibile caricare l'immagine! \n";
      return 1;
    }

    // interpolazione e bilinearizzazione
    Hopfield::HopfieldPattern pattern;
    Hopfield::HopfieldPattern ready_pattern =
        pattern.interpolate(original_image, targetWidth, targetHeight);

    std::cout << "pattern" << i << "acquisito con successo ("
              << ready_pattern.getSize() << "neuroni)\n";

    // Salvataggio dell'immagine ottenuta
    std::string output_filename = "ready_img" + std::to_string(i) + ".png";
    if (ready_pattern.saveToFile(output_filename)) {
      std::cout << "Salvato in:" << output_filename << "\n";
    }
    // Aggiungiamo il pattern alla rete
    trainer.addPattern(ready_pattern);

  } // fine del ciclo for

  // 2. Addestramento e calcolo della matrice dei pesi W
  std::cout << "\n ADDESTRAMENTO RETE DI HOPFIELD \n";

  std::vector<std::vector<double>> weights = trainer.train();

  trainer.printWeights(weights);

  if (trainer.saveWeightsToFile("weights.txt", weights)) {
    std::cout << "Matrice dei pesi salvata con successo in 'weights.txt'\n";
  } else {
    std::cerr << "Errore nel salvataggio!\n";
    return 1;
  }

  return 0;
}