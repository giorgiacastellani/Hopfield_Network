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

TEST_CASE("HopfieldPattern - Bilinear Interpolation")
{
  SUBCASE("Edge case: Empty source image")
  {
    HopfieldPattern pattern;

    // Se l'immagine sorgente è vuota (0x0), anche richiedendo un target 20x20
    // la funzione deve fallire in sicurezza e restituire un pattern vuoto (0x0)
    sf::Image emptyImage;
    HopfieldPattern emptyImagePattern = pattern.interpolate(emptyImage, 20, 20);

    CHECK(emptyImagePattern.getWidth() == 0);
    CHECK(emptyImagePattern.getHeight() == 0);
    CHECK(emptyImagePattern.getSize() == 0);
  }

  SUBCASE("Edge case: Zero target dimensions")
  {
    HopfieldPattern pattern;

    sf::Image validImage;
    validImage.create(50, 50, sf::Color::White);

    // 1. Larghezza target pari a 0 -> Ritorna pattern vuoto (0x0)
    HopfieldPattern zeroWidthPattern = pattern.interpolate(validImage, 0, 20);
    CHECK(zeroWidthPattern.getWidth() == 0);
    CHECK(zeroWidthPattern.getHeight() == 0);
    CHECK(zeroWidthPattern.getSize() == 0);

    // 2. Altezza target pari a 0 -> Ritorna pattern vuoto (0x0)
    HopfieldPattern zeroHeightPattern = pattern.interpolate(validImage, 20, 0);
    CHECK(zeroHeightPattern.getWidth() == 0);
    CHECK(zeroHeightPattern.getHeight() == 0);
    CHECK(zeroHeightPattern.getSize() == 0);
  }
  SUBCASE("Dimensions and neuron count")
  {
    sf::Image sourceImage;
    sourceImage.create(137, 89, sf::Color::Red);

    HopfieldPattern pattern;
    HopfieldPattern resizedPattern = pattern.interpolate(sourceImage, 20, 20);

    CHECK(resizedPattern.getWidth() == 20);
    CHECK(resizedPattern.getHeight() == 20);
    CHECK(resizedPattern.getSize() == 400);
  }
  SUBCASE("Solid colors (All White and All Black)")
  {
    sf::Image whiteImage, blackImage;
    whiteImage.create(100, 100, sf::Color::White);
    blackImage.create(100, 100, sf::Color::Black);

    HopfieldPattern pattern;
    HopfieldPattern whitePattern = pattern.interpolate(whiteImage, 10, 10);
    HopfieldPattern blackPattern = pattern.interpolate(blackImage, 10, 10);

    // 1. Immagine tutta bianca -> Ogni singolo neurone deve essere +1
    for (std::size_t i = 0; i < whitePattern.getSize(); ++i) {
      CHECK(whitePattern[i] == 1);
    }

    // 2. Immagine tutta nera -> Ogni singolo neurone deve essere -1
    for (std::size_t i = 0; i < blackPattern.getSize(); ++i) {
      CHECK(blackPattern[i] == -1);
    }
  }
  SUBCASE("Binarization Threshold (Split Image)")
  {
    // 1. Creiamo un'immagine 100x100: Metà superiore BIANCA, metà inferiore
    // NERA
    sf::Image splitImage;
    splitImage.create(100, 100, sf::Color::Black);
    for (unsigned int x = 0; x < 100; ++x) {
      for (unsigned int y = 0; y < 50; ++y) {
        splitImage.setPixel(x, y, sf::Color::White);
      }
    }

    HopfieldPattern pattern;
    HopfieldPattern scaledPattern = pattern.interpolate(splitImage, 10, 10);

    // 2. Controllo metà superiore (primi 50 neuroni -> indici da 0 a 49):
    // devono essere +1
    for (std::size_t i = 0; i < 50; ++i) {
      CHECK(scaledPattern[i] == 1);
    }

    // 3. Controllo metà inferiore (ultimi 50 neuroni -> indici da 50 a 99):
    // devono essere -1
    for (std::size_t i = 50; i < 100; ++i) {
      CHECK(scaledPattern[i] == -1);
    }
  }
  SUBCASE("Non-Square Resizing and Dimension Getters")
{
  // 1. Immagine rettangolare sorgente (200x100 pixel)
  sf::Image srcImage;
  srcImage.create(200, 100, sf::Color::White);

  HopfieldPattern pattern;
  // Ridimensioniamo a una griglia rettangolare di 20x5 neuroni
  HopfieldPattern resized = pattern.interpolate(srcImage, 20, 5);

  // 2. Verifica dei getter delle dimensioni
  CHECK(resized.getWidth() == 20);
  CHECK(resized.getHeight() == 5);
  CHECK(resized.getSize() == 100); // 20 * 5 = 100 neuroni totali

  // 3. Verifica che il valore dei neuroni sia coerente (+1)
  for (std::size_t i = 0; i < resized.getSize(); ++i) {
    CHECK(resized[i] == 1);
  }
}
}

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
                                     // binarizzato...è troppo complicato come
                                     // test? rivedi
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

// completa/rivedi i test su saveToFile
// capisci come gestire (o non gestire - come ora) il quarto parametro di
// opacità