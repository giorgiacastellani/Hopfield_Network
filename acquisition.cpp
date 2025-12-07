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

  // in tutti gli altri casi, applichiamo il metodo di interpolazione bilineare
  // calcoliamo gli "scale factor" sia per x che per y, dobbiamo poi effettuare
  // interpolazione lineare su entrambe le variabili, gemini consiglia di usare
  // un +o.5 scale factor per centrare il campionamento static cast per maggiore
  // precisione
  const float scale_x = static_cast<float>(original_width) / width_;
  const float scale_y = static_cast<float>(original_height) / height_;

  // doppio ciclo for
  for (int x_new = 0; x_new < width_; x_new++) {
    for (int y_new = 0; y_new < height_; y_new++) {
      // per calcolare P(x_old,y_old)
      const float x_old = (x_new + 0.5f) * scale_x - 0.5f;
      const float y_old = (y_new + 0.5f) * scale_y - 0.5f;
      // passiamo da float a int, avremmo potuto usare int x_i=float x_old;
      const int x_i = static_cast<int>(x_old);
      const int y_i = static_cast<int>(y_old);

      // immaginiamo di lavorare su un quadrato di lato 1, Q11,Q12,Q21,Q22
      // voglio definire le distanze tra coordinate coppia a coppia

      // mi serve una funzione Sfml per prendere l'intensità di ciascun pixel

      // infine applico la formula wikipedia, 3 volte, una per colore
    }
  }
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
