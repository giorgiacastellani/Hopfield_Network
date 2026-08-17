#include "acquisition.hpp"

namespace Hopfield {
/*
unsigned MappedPixel::get_x() const
{
  return x_;
}
unsigned MappedPixel::get_y() const
{
  return y_;
}
std::uint8_t MappedPixel::get_R() const
{
  return R_;
}
std::uint8_t MappedPixel::get_G() const
{
  return G_;
}
std::uint8_t MappedPixel::get_B() const
{
  return B_;
}
void MappedPixel::setPixel(sf::Image& inputImage)
{
  sf::Color color_to_set(R_, G_, B_);
  inputImage.setPixel(x_, y_, color_to_set);
}

// definizioni dei getter/setter/metodi della classe Acquisition
unsigned Acquisition::getHeight() const
{
  return height_;
}
unsigned Acquisition::getWidth() const
{
  return width_;
}
void Acquisition::setPixel(unsigned x, unsigned y, const sf::Color& color)
{
  if (x >= width_ || y >= height_) {
    return;
  }
  image_.setPixel(x, y, color);
};

void Acquisition::resize_interpolation(const sf::Image& original_image)
{
   const unsigned original_width  = original_image.getSize().x;
  const unsigned original_height = original_image.getSize().y;

  image_.create(width_, height_, sf::Color::Black);

  // caso limite: l'immagine fornita è già delle dimensioni desiderate
  if (original_width == width_ && original_height == height_) {
    image_ = original_image;
    return;
  }

  // in tutti gli altri casi, applichiamo il metodo di interpolazione bilineare
  // Calcola i fattori di scala, ovvero di quanto l'immagine originale è più
  // grande di quella nuova
  const float scale_x = static_cast<float>(original_width) / width_;
  const float scale_y = static_cast<float>(original_height) / height_;

  // doppio ciclo for
  for (int x_new = 0; x_new < width_; x_new++) {
    for (int y_new = 0; y_new < height_; y_new++) {
      // PARTE 1:DEFINISCO IL QUADRATO DI LAVORO
      const double x_src = x_new * scale_x;
      const double y_src = y_new * scale_y;
      // mi servono pixel interi
      const unsigned x1 = static_cast<unsigned>(x_src);
      const unsigned y1 = static_cast<unsigned>(y_src);
      // a partire da quello "in basso a sx" trovo gli altri 3 pixel
      // min (di cmath) evita che si considerino coordinate al di là dei limiti
      const unsigned x2 = std::min(x1 + 1, original_width - 1);
      const unsigned y2 = std::min(y1 + 1, original_height - 1);
      // calcoliamo i 4 coeff che nella formula su wiki vengono moltiplicati per
      // le intensità
      // sfruttando che lavoriamo su un quadrato unitario, avendo dx o 1-dx e
      // idem per y
      const double dx = x_src - x1; // dx è un valore tra 0.0 e quasi 1.0
      const double dy = y_src - y1; // dy è un valore tra 0.0 e quasi 1.0

      // PARTE 2:
      sf::Color C11 = original_image.getPixel(x1, y1);
      sf::Color C21 = original_image.getPixel(x2, y1);
      sf::Color C22 = original_image.getPixel(x2, y2);
      sf::Color C12 = original_image.getPixel(x1, y2);

      MappedPixel Q11(x1, y1, C11.r, C11.b, C11.g);
      MappedPixel Q21(x2, y1, C21.r, C21.b, C21.g);
      MappedPixel Q22(x2, y2, C22.r, C22.b, C22.g);
      MappedPixel Q12(x1, y1, C12.r, C12.b, C12.g);

      // interpolazione bilineare svolta per ciascun canale di colore RGB
      // funzione lambda per eseguire l'interpolazione per un singolo canale
      auto interpolate_channel = [&](std::uint8_t C11, std::uint8_t C21,
                                     std::uint8_t C12,
                                     std::uint8_t C22) -> std::uint8_t {
        // Interpolazione lineare su X (ai livelli y1 e y2)
        const double C_x1 = C11 * (1.0 - dx) + C21 * dx;
        const double C_x2 = C12 * (1.0 - dx) + C22 * dx;

        // Interpolazione lineare su Y (tra c_x1 e c_x2)
        const double C_final = C_x1 * (1.0 - dy) + C_x2 * dy;

        // Arrotonda e limita il valore nell'intervallo [0, 255]
        return static_cast<std::uint8_t>(
            std::clamp(std::round(C_final), 0.0, 255.0));
      };

      std::uint8_t R_interp = interpolate_channel(C11.r, C21.r, C12.r, C22.r);
      std::uint8_t G_interp = interpolate_channel(C11.g, C21.g, C12.g, C22.g);
      std::uint8_t B_interp = interpolate_channel(C11.b, C21.b, C12.b, C22.b);

      //sf::Color P_interp(R_interp, G_interp, B_interp);
resized_image.setPixel(x_new, y_new, P_interp); MappedPixel
P_interp_mapped(x_new, y_new, R_interp, G_interp, B_interp);
      P_interp_mapped.setPixel(image_);
    }
  }
}

// definizioni delle funzioni libere
sf::Image loadimage(const std::string& filename)
{
  sf::Image image;

  if (!image.loadFromFile(filename)) {
    throw std::runtime_error("The image could not be loaded correctly");
  }
  return image;
}
*/

HopfieldPattern HopfieldPattern::interpolate(const sf::Image& originalImage,
                                             unsigned int targetWidth,
                                             unsigned int targetHeight) const
{                      // Implementazione temporanea/stub
  (void)originalImage; // Silenzia il warning di parametro non usato
  return HopfieldPattern(targetWidth, targetHeight);
}

void HopfieldPattern::binarize(const sf::Image& image, float threshold)
{
  width_  = image.getSize().x; // https://cppreference.com/cpp/types/size_t
  height_ = image.getSize().y;

  data_.resize(static_cast<std::size_t>(width_) * height_);

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
              / 3.0f;
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
  sf::Image image;
  image.create(
      width_,
      height_); // sulla documentazione corrente non si trova
                // create...versione datata di SFML
                // https://www.sfml-dev.org/documentation/3.1.0/classsf_1_1Image.html

  // mappatura dei valori binari (+/-1) sui pixel dell'immagine
  for (unsigned int y = 0; y < height_; ++y) {
    for (unsigned int x = 0; x < width_; ++x) {
      std::size_t index = static_cast<std::size_t>(y) * width_ + x;

      sf::Color color;

      if (data_[index] == 1) {
        color = sf::Color::White;
      } else {
        color = sf::Color::Black;
      }

      image.setPixel(x, y, color);
    }
  }

  // salvataggio dell'immagine su percorso specifico
  return image.saveToFile(filepath.string());
}
} // namespace Hopfield