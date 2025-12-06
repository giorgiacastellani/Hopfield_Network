#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdexcept> // Per std::runtime_error
#include <string>
#include <vector>
namespace Hopfield {
using Pattern = std::vector<int>;
struct Sfml // struttura per contenere elementi sfml
{
  sf::Image image_;
  sf::Color color_;
  sf::Texture texture_;
  sf::Sprite sprite_;
};

class Acquisition // classe per passare da immagine a "architettura" e fissare
                  // il numero di neuroni
{
  unsigned height_; // altezza finale desiderata del pattern
  unsigned width_;
  unsigned N_;

 public:
  Acquisition(int height, int width)
      : height_{height}
      , width_{width}
      , N_{height * width}
  {}

  unsigned getHeight() const;
  unsigned getWidth() const;
  unsigned getN() const;

  sf::Image resize_interpolation(const sf::Image& original_image) const;
};

sf::Image loadimage(const std::string& filename); // carica l'immagine

} // namespace Hopfield

#endif