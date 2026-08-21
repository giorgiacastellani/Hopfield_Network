#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP
#include <SFML/Graphics.hpp>
// #  include <algorithm>
// #  include <cmath>
// #  include <cstdint> // Necessario per std::uint8_t
// #  include <iostream>
// #  include <stdexcept>
// #  include <string>
#include <filesystem>
#include <string>
#include <vector>

namespace Hopfield {
class HopfieldPattern
{
 private:
  unsigned int width_{0}; // capisci che cosa cambia con inizializzazione
  unsigned int height_{0};
  std::vector<int> data_;

 public:
  HopfieldPattern()
  {} // utile o meno? costruttore default
  HopfieldPattern(unsigned int width, unsigned int height, const std::vector<int>& data)
      : width_(width)
      , height_(height)
      , data_(width * height, -1)
  {}
  // -- METODI DELLA CLASSE --
  HopfieldPattern interpolate(const sf::Image& originalImage,
                              unsigned int targetWidth,
                              unsigned int targetHeight) const;

  // per ciascun pixel - corrispondente a una componente di data_ - vogliamo
  // estrarre R G B e calcolare g in maniera da confrontarlo con il valore
  // soglia ... serve costruire un ciclo
  void binarize(const sf::Image& image, float threshold = 127.0f);

  bool saveToFile(const std::filesystem::path& filepath) const;

  // -- GETTER E UTILI --
  unsigned int getWidth() const
  {
    return width_;
  }
  unsigned int getHeight() const
  {
    return height_;
  }
  std::size_t getSize() const
  {
    return data_.size();
  }
  // Accesso in sola lettura
  int operator[](std::size_t index) const
  {
    return data_[index];
  }

  // Accesso in scrittura
  int& operator[](std::size_t index)
  {
    return data_[index];
  }
  std::size_t size() const
  {
    return data_.size();
  };
};
} // namespace Hopfield

#endif