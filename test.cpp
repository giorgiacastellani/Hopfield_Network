#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "acquisition.hpp"
#include "doctest.h"

using namespace Hopfield; // a giacomini va bene?? 

// -------------------------------------------------------------
// 1. TEST STATO INIZIALE, COSTRUTTORI E GETTER / UTILI
// -------------------------------------------------------------
TEST_CASE("HopfieldPattern - Costruttori e Getter/Utili")
{
  SUBCASE("Default Constructor")
  {
    HopfieldPattern pattern;
    CHECK(pattern.getWidth() == 0);
    CHECK(pattern.getHeight() == 0);
    CHECK(pattern.getSize() == 0);
  }

  SUBCASE("Costruttore")
  {
    unsigned int w = 3;
    unsigned int h = 2;
    HopfieldPattern pattern(w, h);

    CHECK(pattern.getWidth() == w);
    CHECK(pattern.getHeight() == h);
    CHECK(pattern.getSize() == 6);

    // verifichiamo anche che tutti i neuroni siano inizializzati a -1
    bool tutti_inattivi = true;
    for (std::size_t i = 0; i < pattern.getSize(); ++i) {
      if (pattern[i] != -1) {
        tutti_inattivi = false;
        break;
      }
    }
    CHECK(tutti_inattivi == true);
  }
  SUBCASE("Operatore d'accesso [] in Lettura e Scrittura")
  {
    HopfieldPattern pattern(2, 2); // 4 neuroni

    // Modifica tramite operator[] non-const
    pattern[0] = 1;
    pattern[3] = 1;

    // Lettura tramite operator[]
    CHECK(pattern[0] == 1);
    CHECK(pattern[1] == -1);
    CHECK(pattern[2] == -1);
    CHECK(pattern[3] == 1);
  }
}

// -------------------------------------------------------------
// 2. TEST METODO BINARIZE
// -------------------------------------------------------------
TEST_CASE("HopfieldPattern - Binarizzazione Immagine"){}

// -------------------------------------------------------------
// 3. TEST METODO INTERPOLATE (RIDIMENSIONAMENTO BILINEARE)
// ------------------------------------------------------------
TEST_CASE("HopfieldPattern - Interpolazione Bilineare") {}

// -------------------------------------------------------------
// 4. TEST METODO SAVETOFILE
// -------------------------------------------------------------
TEST_CASE("Hopfieldpattern - Salvataggio immagini") {}