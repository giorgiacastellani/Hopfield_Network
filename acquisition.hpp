#ifndef ACQUISITION_HPP
#define ACQUISITION_HPP
#include <SFML/Graphics.hpp>
namespace Hopfield{


class Acquisition // classe per passare da immagine a "architettura" e fissare il
              // numero di neuroni
{
  int height_;
  int width_;
  int N_;

 public:
  Acquisition(double height, double width)
      : height_{height}
      , width_{width}
  {}

  int getHeight() const;
  int getWidth() const;
  int getN() const;
};
}

#endif