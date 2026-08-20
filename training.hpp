#ifndef TRAINING_HPP
#define TRAINING_HPP

#include "acquisition.hpp"

namespace Hopfield {

class NetworkTrainer
{
 private:
  std::size_t pattern_size_;              // N=targetWidth*targetHeight
  std::vector<HopfieldPattern> patterns_; // vettore di P pattern () memorizzati

 public:
  // costruttore...patterns_ verrà inzializzato come un vettore di dimensione 0
  NetworkTrainer(std::size_t pattern_size)
      : pattern_size_(pattern_size)
  {}
  // aggiunge un pattern già binarizzato alla collezione P
  void addPattern(const HopfieldPattern& pattern)
  {
    // 1 controllo dimensione N
    if (pattern.getSize() != pattern_size_) {
      return;
    }

    // 2 controllo presenza esclusiva di +1 e -1
    
  
    patterns_.push_back(pattern);
  };

} // namespace Hopfield

#endif // TRAINING_HPP