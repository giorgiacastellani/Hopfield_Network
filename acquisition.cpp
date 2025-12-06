#include "acquisition.hpp"

namespace Hopfield {
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

  // usiamo il metodo create() di Image per fissare lo "standard" fornito
  // dall'immagine già ridimensionata
  sf::Image resized_image;
  resized_image.create(width_, height_);

  // caso limite: l'immagine fornita è già delle dimensioni desiderate
  if (original_width == width_ && original_height == height_) {
    return original_image;
  }

  //in tutti gli altri casi, applichiamo il metodo di interpolazione bilineare
  //a questo punto ci servono 4 pixel adiacenti a quello protagonista, per trovare questi 4 punti
  //ci serviamo di un fattore di scala che stabiliamo arbitrariamente: i 4 pixel prossimi per una distanza di 0.5 (a sx,a dx, in alto, in basso
  //se il nostro punto è l'origine immaginiamo una circ. di raggio 0.5 e che i 4 pixel su cui fondiamo il metodo siano corrispondenti agli angoli 0, pi/2, pi, 3/2pi e 2pi)
  const float scale_x=static_cast
}

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
