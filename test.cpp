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
TEST_CASE("HopfieldPattern - Binarizzazione Immagine")
{
  SUBCASE("Caso standard con pixel bianco e nero")
  {
    sf::Image img;
    img.create(2, 1);
    img.setPixel(0, 0, sf::Color::White); // g = 255 -> +1
    img.setPixel(1, 0, sf::Color::Black); // g = 0   -> -1

    HopfieldPattern pattern;
    pattern.binarize(img, 127.0f);

    CHECK(pattern.getWidth() == 2);
    CHECK(pattern.getHeight() == 1);
    CHECK(pattern.getSize()
          == static_cast<std::size_t>(img.getSize().x)
                 * static_cast<std::size_t>(img.getSize().y));
    CHECK(pattern[0] == 1);
    CHECK(pattern[1] == -1);
  }

  SUBCASE("Valore limite")
  {
    sf::Image img;
    img.create(2, 1);

    img.setPixel(0, 0, sf::Color(127, 127, 127));
    img.setPixel(1, 0, sf::Color(126, 126, 126));

    HopfieldPattern pattern;
    pattern.binarize(img, 127.0f);

    CHECK(pattern[0] == 1);
    CHECK(pattern[1] == -1);
  }

  SUBCASE("Soglia Custom")
  {
    sf::Image img;
    img.create(1, 1);

    img.setPixel(0, 0, sf::Color(180, 180, 180));

    HopfieldPattern pattern;

    // Con soglia 127.0f -> deve essere +1
    pattern.binarize(img, 127.0f);
    CHECK(pattern[0] == 1);

    // Con soglia più severa 200.0f -> lo stesso pixel deve diventare -1
    pattern.binarize(img, 200.0f);
    CHECK(pattern[0] == -1);
  }

  SUBCASE("Correttezza Indici")
  {
    sf::Image img;
    img.create(2, 2);

    // (0,0): Bianco -> index 0=0*2+0
    // (1,0): Nero   -> index 1=0*2+1
    // (0,1): Nero   -> index 2=2*1+0
    // (1,1): Bianco -> index 3=1*2+1
    img.setPixel(0, 0, sf::Color::White);
    img.setPixel(1, 0, sf::Color::Black);
    img.setPixel(0, 1, sf::Color::Black);
    img.setPixel(1, 1, sf::Color::White);

    HopfieldPattern pattern;
    pattern.binarize(img, 127.0f);

    CHECK(pattern[0] == 1);
    CHECK(pattern[1] == -1);
    CHECK(pattern[2] == -1);
    CHECK(pattern[3] == 1);
  }
}

// -------------------------------------------------------------
// 3. TEST METODO INTERPOLATE (RIDIMENSIONAMENTO BILINEARE)
// ------------------------------------------------------------
TEST_CASE("HopfieldPattern - Interpolazione Bilineare")
{}

// -------------------------------------------------------------
// 4. TEST METODO SAVETOFILE
// -------------------------------------------------------------
TEST_CASE("Hopfieldpattern - Salvataggio immagini")
{
  SUBCASE("Test Immagine Vuota")
  {
    HopfieldPattern pattern;

    CHECK(pattern.saveToFile("out.png") == false);
  }

  SUBCASE("Test Solo Bianco e Nero") // necessità? testa il percorso inverso a
                                     // quello di binarize...non si riduce alla
                                     // verifica che il pattern sia realmente
                                     // binarizzato...è troppo complicato come test? rivedi
  {
    sf::Image img;
    img.create(2, 2);
    img.setPixel(0, 0, sf::Color::White);
    img.setPixel(1, 0, sf::Color(100, 100, 100));
    img.setPixel(0, 1, sf::Color::Black);
    img.setPixel(1, 1, sf::Color(200, 200, 200));

    HopfieldPattern pattern;
    pattern.binarize(img, 127.0f);

    std::filesystem::path testPath = "test_colors.png";

    CHECK(pattern.saveToFile(testPath) == true);

    sf::Image reloaded;
    CHECK(reloaded.loadFromFile(testPath.string()) == true);

    for (unsigned int y = 0; y < reloaded.getSize().y; ++y) {
      for (unsigned int x = 0; x < reloaded.getSize().x; ++x) {
        sf::Color c = reloaded.getPixel(x, y);
        CHECK((c == sf::Color::White || c == sf::Color::Black));
      }
    }

    std::filesystem::remove(testPath); // Pulizia file temporaneo
  }
}

//completa/rivedi i test su saveToFile
//capisci come gestire (o non gestire - come ora) il quarto parametro di opacità