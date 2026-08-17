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
/*
Struct Sfml // struttura per contenere elementi sfml
{
  sf::Image image_;
  sf::Color color_;
  sf::Texture texture_;
  sf::Sprite sprite_;
};

struct MappedPixel
{
  unsigned x_;
  unsigned y_;

  std::uint8_t R_;
  std::uint8_t G_;
  std::uint8_t B_;

  MappedPixel(int x, int y, std::uint8_t red, std::uint8_t green,
              std::uint8_t blue)
      : x_{x}
      , y_{y}
      , R_{red}
      , G_{green}
      , B_{blue}
  {}

  unsigned get_x() const {};
  unsigned get_y() const {};

  std::uint8_t get_R() const {};

  std::uint8_t get_G() const {};

  std::uint8_t get_B() const {};

  void setPixel(sf::Image& inputImage);
};

class Acquisition // classe per passare da immagine a "architettura" e fissare
                  // il numero di neuroni...si appoggia totalmente a SFML, ma
                  // includendo image_ tra le var private manteniamo la libertà
                  // di agire separatamente tra corpo del codice e oggetti di
                  // supporto
{
  unsigned height_;
  unsigned width_;
  sf::Image image_;

 public:
  Acquisition(int height, int width)
      : height_{height}
      , width_{width} // image_ viene creata con il default constructor

  {
    image_.create(width_, height_, sf::Color::Black);
  }

  unsigned getHeight() const;
  unsigned getWidth() const;

  void setPixel(unsigned x, unsigned y, const sf::Color& color);
  void resize_interpolation(
      const sf::Image&
          original_image); // non creiamo un nuovo oggetto ma modifichiamo lo
stato dell'originale
};

sf::Image loadimage(const std::string& filename);
*/
class HopfieldPattern
{
 private:
  unsigned int width_{0}; // capisci che cosa cambia con inizializzazione
  unsigned int height_{0};
  std::vector<int> data_;

 public:
  HopfieldPattern()
  {} // utile o meno? costruttore default
  HopfieldPattern(unsigned int width, unsigned int height)
      : width_(width)
      , height_(height)
      , data_(width * height, -1)
  {}
  // -- METODI DELLA CLASSE --
  
  HopfieldPattern interpolate(const sf::Image& originalImage,
                              unsigned int targetHeight,
                              unsigned int targetWidth) const;

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
};

} // namespace Hopfield

#endif