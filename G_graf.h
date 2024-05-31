#ifndef G_GRAF_H
#define G_GRAF_H

#include "G_struckt.h"
//#include "G_lista.h"
#include "json.hpp" /* [1.] */
#include <fstream>
#include <map> /* [5.] */
using json = nlohmann::json;

class graf {
private:
  lista<miasto>* lista_miast = nullptr;
  lista<polaczenia>* lista_polaczen = nullptr;
  node<miasto>** tablica_indeksow_miast = nullptr;
  //node<polaczenia>** tablica_sasiedztwa = nullptr;
  lista< node<polaczenia>* >** tablica_sasiedztwa = nullptr;
  /* tablica_sasiedztwa -> TABLICA wskaźników -> na listy wskaźników ->
     na listy typu lista<polaczenia> (lista_polaczen)
     [•][•][•][•][•][•][•][•]
      ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓   lista_polaczen
      ■  ■  ■  ■  ■  ■  ■  ■ ----→ ■
      ↓  ↓ null↓  ↓ null↓  ↓       ↓
      ■  ■     ■  ■   null ■ ----→ ■
   null null null null     ↓       ↓
                           ■ ----→ ■ 
  */
  unsigned int size_TAB_sasiedztwa;
  string file_name;
  json data_json;
  map<string, int> indeks_map;

  void open();

public:
  void test(); /* <-- USUNĄĆ */
  graf();
  ~graf();
  void load();
  void add_ARRRAY_ptrs();

  void endVertices(node<miasto>** c1, node<miasto>** c2, node<polaczenia>* road);
  node<miasto>* opposite(node<miasto>* c1, node<polaczenia>* road) const;
};

#endif

/* Źródła kodu:
1. https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp
2. https://cpp0x.pl/kursy/Kurs-C++/Dodatkowe-materialy/Obsluga-plikow/305
3. https://learn.microsoft.com/pl-pl/cpp/cpp/try-throw-and-catch-statements-cpp?view=msvc-170
4. https://cpp0x.pl/forum/temat/?id=11057
5. https://en.cppreference.com/w/cpp/container/map

*/