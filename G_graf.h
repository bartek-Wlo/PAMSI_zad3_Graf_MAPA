#ifndef G_GRAF_H
#define G_GRAF_H

#include "G_struckt.h"
#include "G_lista.h"
#include "json.hpp" /* [1.] */
#include <fstream>
using json = nlohmann::json;

void test();

class graf {
private:
  lista<miasto>* lista_miast = nullptr;
  lista<polaczenia>* lista_polaczen = nullptr;
  node<miasto>** tablica_indeksow_miast = nullptr;
  node<polaczenia>** tablica_sasiedztwa = nullptr;
  string file_name;
  json data_json;

  void open();

public:
  graf();
  ~graf();
  void load();
  void add_ARRRAY_ptrs();
};

#endif

/* Źródła kodu:
1. https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp
2. https://cpp0x.pl/kursy/Kurs-C++/Dodatkowe-materialy/Obsluga-plikow/305
3. https://learn.microsoft.com/pl-pl/cpp/cpp/try-throw-and-catch-statements-cpp?view=msvc-170
4. https://cpp0x.pl/forum/temat/?id=11057

*/