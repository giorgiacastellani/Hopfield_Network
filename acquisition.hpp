#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <algorithm>
#include <cmath>
#include <cstdint> // Necessario per std::uint8_t
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
namespace Hopfield {

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
          original_image); // non creiamo un nuovo oggetto ma modifichiamo lo stato dell'originale
};

sf::Image loadimage(const std::string& filename); 

} // namespace Hopfield

#endif