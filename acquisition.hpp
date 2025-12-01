#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP
#include <SFML/Graphics.hpp>
#include <vector>
namespace Hopfield {
using Pattern = std::vector<int>;
struct Sfml // struttura per contenere elementi sfml
{
  sf::Image image_;
  sf::Texture texture_;
  sf::Sprite sprite_;
};

class Acquisition // classe per passare da immagine a "architettura" e fissare
                  // il numero di neuroni
{
  int height_; // altezza finale desiderata del pattern
  int width_;
  int N_;

 public:
  Acquisition(int height, int width)
      : height_{height}
      , width_{width}
      , N_{height * width}
  {}

  int getHeight() const;
  int getWidth() const;
  int getN() const;
  sf::Image loadimmage(const std::string& filename);
};
} // namespace Hopfield

#endif