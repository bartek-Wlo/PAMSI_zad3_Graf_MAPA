#ifndef G_GRAF_H
#define G_GRAF_H

#include "G_struckt.h"
#include "G_lista.h"
#include "json.hpp" /* https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp */
#include <fstream>
using json = nlohmann::json;

void test();

class graf {
private:
  lista<miasto>* lista_miast = nullptr;
  lista<polaczenia>* lista_polaczen = nullptr;
  node<miasto>** tablica_indeksow_miast = nullptr;
  node<polaczenia>** tablica_sasiedztwa = nullptr;

  void load();
public:
  graf();
  ~graf();
};

#endif