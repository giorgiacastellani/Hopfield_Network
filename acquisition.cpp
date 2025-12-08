#include "acquisition.hpp"
#include <cmath> //forse non dovrebbe stare qui??

namespace Hopfield {
// definizioni dei getter/setter della struct MappedPixel
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

// definizioni dei getter/metodi della classe Acquisition
unsigned Acquisition::getHeight() const
{
  return height_;
}
unsigned Acquisition::getWidth() const
{
  return width_;
}
unsigned Acquisition::getN() const
{
  return width_ * height_;
}

sf::Image
Acquisition::resize_interpolation(const sf::Image& original_image) const
{
  // otteniamo le dimensioni dell'immagine sottoposta a ridimensionamento
  // il metodo getSize() di sfml resistuisce un vettore v2(vx,vy) per cui
  // vx=larghezza e vy=altezza

  const unsigned original_width  = original_image.getSize().x;
  const unsigned original_height = original_image.getSize().y;

  Acquisition original_image(original_width, original_height); // necessario ?

  Acquisition resized_image(width_, height_);

  // caso limite: l'immagine fornita è già delle dimensioni desiderate
  if (original_width == width_ && original_height == height_) {
    return original_image;
  }

  // in tutti gli altri casi, applichiamo il metodo di interpolazione bilineare
  const float scale_x = static_cast<float>(original_width) / width_;
  const float scale_y = static_cast<float>(original_height) / height_;

  // doppio ciclo for
  for (int x_new = 0; x_new < width_; x_new++) {
    for (int y_new = 0; y_new < height_; y_new++) {
      // PARTE 1:DEFINISCO IL QUADRATO DI LAVORO
      const double x_src = x_new * scale_x;
      const double y_src = y_new * scale_y;
      // mi servono pixel interi
      //  Alternativa a floor per valori positivi
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
      auto interpolate_channel = [&](std::uint8_t c11, std::uint8_t c21,
                                     std::uint8_t c12,
                                     std::uint8_t c22) -> std::uint8_t {
        // Interpolazione lineare su X (ai livelli y1 e y2)
        const double c_x1 = c11 * (1.0 - dx) + c21 * dx;
        const double c_x2 = c12 * (1.0 - dx) + c22 * dx;

        // Interpolazione lineare su Y (tra c_x1 e c_x2)
        const double c_final = c_x1 * (1.0 - dy) + c_x2 * dy;

        // Arrotonda e limita il valore nell'intervallo [0, 255]
        return static_cast<std::uint8_t>(
            std::clamp(std::round(c_final), 0.0, 255.0));
      };

      std::uint8_t R_interp = interpolate_channel(C11.r, C21.r, C12.r, C22.r);
      std::uint8_t G_interp = interpolate_channel(C11.g, C21.g, C12.g, C22.g);
      std::uint8_t B_interp = interpolate_channel(C11.b, C21.b, C12.b, C22.b);

      sf::Color P_interp(R_interp, G_interp, B_interp);

      // confusione tra sfml image e acquisition
      resized_image.setPixel(x_new, y_new, P_interp);
    }
  }
  return resized_image;
}

// definizioni delle funzioni libere
sf::Image loadimage(const std::string& filename)
{
  sf::Image image;

  // ciclo che serve per caricare le immagini, filename funziona da path
  if (!image.loadFromFile(filename)) {
    throw std::runtime_error("The image could not be loaded correctly");
  }
  return image;
}

} // namespace Hopfield
