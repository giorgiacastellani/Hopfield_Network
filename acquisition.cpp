#include "acquisition.hpp"

namespace Hopfield {

void HopfieldPattern::binarize(const sf::Image& image, float threshold)
{
  width_  = image.getSize().x; // https://cppreference.com/cpp/types/size_t
  height_ = image.getSize().y;

  data_.resize(static_cast<std::size_t>(width_)
               * height_); // static_cast<std::size_t> converte in modo sicuro
                           // il tipo per evitare avvisi del compilatore.

  // ciclo che "ricopre" tutti i pixel, occupandosene riga per riga
  for (unsigned int y = 0; y < height_; ++y) {
    for (unsigned int x = 0; x < width_; ++x) {
      // estrazione del colore del pixel
      sf::Color color = image.getPixel(
          x,
          y); // https://www.sfml-dev.org/documentation/3.1.0/classsf_1_1Image.html#a166e09f1c57c5d186c77682ae898f852

      // calcolo del parametro g
      float g = (static_cast<float>(color.r) + static_cast<float>(color.g)
                 + static_cast<float>(color.b))
              / 3.0f; // Si convertono i singoli canali in float per eseguire
                      // una divisione decimale precisa per 3.0f.
      // calcolo dell'indice uno-dimensionale per scorrere data_
      std::size_t index = static_cast<std::size_t>(y) * width_ + x;

      // ciclo
      if (g >= threshold) {
        data_[index] = 1; // Pixel chiaro o sopra la soglia
      } else {
        data_[index] = -1; // Pixel scuro o sotto la soglia
      }
    }
  }
}
HopfieldPattern HopfieldPattern::interpolate(const sf::Image& originalImage,
                                             unsigned int targetWidth,
                                             unsigned int targetHeight) const
{
  // Controllo di sicurezza: se l'immagine sorgente o le dimensioni target sono
  // nulle,interrompe

  if (originalImage.getSize().x == 0 || originalImage.getSize().y == 0
      || targetWidth == 0 || targetHeight == 0) {
    return HopfieldPattern(0, 0, {});
  }

  // Memorizza la larghezza e l'altezza dell'immagine originale SFML tipo
  // unsigned int
  unsigned int og_width  = originalImage.getSize().x;
  unsigned int og_height = originalImage.getSize().y;

  // Fattori di scala (rapporto tra dimensioni vecchie e nuove) tipo float per
  // avere maggiore precisione
  float scale_x = static_cast<float>((og_width) / (targetWidth));
  float scale_y = static_cast<float>((og_height) / (targetHeight));

  // Crea una tela grafica temporanea SFML ridimensionata
  sf::Image resized_image;
  resized_image.create(targetWidth, targetHeight);

  // Scorre ogni pixel della nuova griglia ridimensionata (targetWidth x
  // targetHeight)
  for (unsigned int y = 0; y < targetHeight; ++y) {
    for (unsigned int x = 0; x < targetWidth; ++x) {
      // MAPPA LA COORDINATA: calcola dove cade il pixel (x, y) nell'immagine
      // originale
      float og_x = x * scale_x;
      float og_y = y * scale_y;

      // Trova le coordinate intere dei pixel vicini nell'immagine originale.
      unsigned int x1 = static_cast<unsigned int>(og_x);
      unsigned int y1 = static_cast<unsigned int>(og_y);

      // Calcola la coordinata adiacente (evitando di uscire dai bordi
      // dell'immagine).Per l'interpolazione bilineare mi servono (x1,y1)
      // (x2,y1) (x1,y2) (x2,y2) attorno al generico pixel (x,y)
      unsigned int x2;
      if (x1 + 1 < og_width) {
        x2 = x1 + 1;
      } else {
        x2 = x1;
      }

      unsigned int y2;
      if (y1 + 1 < og_height) {
        y2 = y1 + 1;
      } else {
        y2 = y1;
      }

      // Distanze frazionarie (la parte decimale delle coordinate). Infatti x1 è
      // stato ottenuto da og_x troncando la parte decimale, la conversione
      // avviene sistematicamente per difetto.
      float dx = og_x - static_cast<float>(x1);
      float dy = og_y - static_cast<float>(y1);

      // Estrae i colori dei 4 pixel vicini dall'immagine originale
      sf::Color c11 = originalImage.getPixel(x1, y1);
      sf::Color c21 = originalImage.getPixel(x2, y1);
      sf::Color c12 = originalImage.getPixel(x1, y2);
      sf::Color c22 = originalImage.getPixel(x2, y2);

      // funzione interna per fare la media pesata di un singolo canale di
      // colore (R, G o B)
      // ho usato gemini, la parentesi quadrata rappresenta la clausola di
      // cattura (capture clause).

      // Servono a indicare al compilatore quali variabili esterne (definite
      // fuori dalla lambda, nella funzione principale) la lambda ha il permesso
      // di "importare" e usare al suo interno. In questo caso conosce la
      // posizione esatta dei pixel all'interno del quadrato formato dai 4
      // vicini

      auto interpolate_channel = [dx, dy](uint8_t v11, uint8_t v21, uint8_t v12,
                                          uint8_t v22) -> uint8_t {
        float top       = static_cast<float>(v11) * (1.0f - dx)
                        + static_cast<float>(v21) * dx;
        float bottom    = static_cast<float>(v12) * (1.0f - dx)
                        + static_cast<float>(v22) * dx;
        float final_val = top * (1.0f - dy) + bottom * dy;
        return static_cast<uint8_t>(final_val);
      };

      // Applica la formula ai canali Rosso (r), Verde (g) e Blu (b)
      uint8_t r = interpolate_channel(c11.r, c21.r, c12.r, c22.r);
      uint8_t g = interpolate_channel(c11.g, c21.g, c12.g, c22.g);
      uint8_t b = interpolate_channel(c11.b, c21.b, c12.b, c22.b);

      // Imposta il colore risultante nell'immagine temporanea ridimensionata
      resized_image.setPixel(x, y, sf::Color(r, g, b));
    }
  }

  // Instanzia l'oggetto finale e binarizza l'immagine ridimensionata
  HopfieldPattern result_pattern;
  result_pattern.binarize(resized_image);

  return result_pattern;
}

// gemini: "Per salvare il pattern binarizzato su file occorre eseguire il
// processo inverso rispetto a binarize: si convertono i valori binari (+1 e
// -1) presenti in data_ nei colori corrispondenti di un'immagine sf::Image,
// per poi delegare il salvataggio su disco a SFML."
bool HopfieldPattern::saveToFile(const std::filesystem::path& filepath) const
{
  // controllo di validità del pattern...per il metodo empty
  // https://en.cppreference.com/cpp/container/vector/empty
  if (width_ == 0 || height_ == 0 || data_.empty()) {
    return false;
  }
  // creazione dell'immagine SFML con le dimensioni del pattern
  // NOTA questo metodo può essere usato solo con immagini che siano già state
  // ridimensionate esattamente a width_ e height_!!!
  sf::Image image;
  image.create(
      width_,
      height_); // membro della classe, ha accesso ai suoi membri privati
                //  sulla documentazione corrente non si trova
                //  create...versione datata di SFML
                //  https://www.sfml-dev.org/documentation/3.1.0/classsf_1_1Image.html

  // mappatura dei valori binari (+/-1) sui pixel dell'immagine
  for (unsigned int y = 0; y < height_; ++y) {
    for (unsigned int x = 0; x < width_;
         ++x) { // il doppio ciclo potrebbe diventare lento
      std::size_t index =
          static_cast<std::size_t>(y) * width_ + x; // calcolo indice 1D

      sf::Color color;

      if (data_[index] == 1) {
        color = sf::Color::White;
      } else if (data_[index] == -1) {
        color = sf::Color::Black;
      } else {
        // gestione del valore non binarizzato (+1 / -1)
        return false;
      }

      image.setPixel(x, y, color);
    }
  }

  // salvataggio dell'immagine su percorso specifico
  return image.saveToFile(filepath.string());
}
} // namespace Hopfield