#include "acquisition.hpp" //vedi giacomini come vuole gli include
#include <iostream>

int main()
{
  // 1. Caricamento dell'immagine mediante SFML
  sf::Image original_image;
  if (!original_image.loadFromFile("img.png")) {
    std::cerr << "Errore: impossibile caricare l'immagine! \n";
    return 1;
  }
  // 2. Ridimensionamento e binarizzazione dell'immagine

  unsigned int targetWidth=20;
  unsigned int targetHeight=20;

  Hopfield::HopfieldPattern pattern;
  Hopfield::HopfieldPattern ready_pattern =
      pattern.interpolate(original_image, targetWidth, targetHeight); //costruiremo interpolate in modo che chiami internamente binarize

  std::cout << "pattern acquisito con successo! Numero neuroni (N):"
            <<ready_pattern.getSize() << "\n";

  // 3. Salvataggio dell'immagine ottenuta
  if (ready_pattern.saveToFile("ready_img.png")) {
    std::cout << "Immagine ridimensionata e binarizzata salvata in "
                 "'ready_img.png'. \n ";
  }
  return 0;
}