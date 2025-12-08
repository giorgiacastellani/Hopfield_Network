#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP
#include <SFML/Graphics.hpp>
#include <cstdint> // Necessario per std::uint8_t
#include <iostream>
#include <stdexcept> // Per std::runtime_error
#include <string>
#include <vector>
namespace Hopfield {
// using Pattern = std::vector<int>;

struct Sfml // struttura per contenere elementi sfml
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

  // gemini: "std::uint8_t è il tipo standard per i canali colore (0-255)"
  // unsigned int con 8 bit di dimensione, il range è 256
  std::uint8_t R_;
  std::uint8_t G_;
  std::uint8_t B_;

  // costruttore
  MappedPixel(int x, int y, std::uint8_t red, std::uint8_t blue,
              std::uint8_t green)
      : x_{x}
      , y_{y}
      , R_{red}
      , G_{green}
      , B_{blue}
  {}

  // getter
  unsigned get_x() const {};
  unsigned get_y() const {};

  // Ritorna l'intensità del canale Rosso (Red)
  std::uint8_t get_R() const {};

  // Ritorna l'intensità del canale Verde (Green)
  std::uint8_t get_G() const {};

  // Ritorna l'intensità del canale Blu (Blue)
  std::uint8_t get_B() const {};

  //setter
  void setPixel(sf::Image& inputImage);
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