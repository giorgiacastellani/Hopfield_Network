#include "acquisition.hpp"

namespace Hopfield {
int Acquisition::getHeight() const
{
  return height_;
}
int Acquisition::getWidth() const
{
  return width_;
}
int Acquisition::getN() const
{
  return width_ * height_;
}

sf::Image Acquisition::loadimage(const std::string& filename)
{
  sf::Image image;

  // ciclo che serve per caricare le immagini, filename funziona da path
  if (!image.loadFromFile(filename)) {
    throw std::runtime_error("The image could not be loaded correctly");
  }
  return image;
}

} // namespace Hopfield
