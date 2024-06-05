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
  lista< node<polaczenia>* >** tablica_sasiedztwa = nullptr;
  /* tablica_sasiedztwa -> TABLICA wskaźników -> na listy wskaźników ->
     na listy typu lista<polaczenia> (lista_polaczen)
     [•][•][•][•][•][•][•][•]
      ↓  ↓  ↓  ↓  ↓  ↓  ↓  ↓   lista_polaczen
      ■  ■ nul ■  ■ nul ■  ■ ----→ ■
      ↓  ↓     ↓  ↓     ↓  ↓       ↓
      ■  ■   null ■   null ■ ----→ ■
      ↓  ↓        ↓        ↓       ↓
    null null    null      ■ ----→ ■ 
  */
  unsigned int size_TAB_sasiedztwa;
  string file_name;
  json data_json;
  map<string, int> indeks_map;

  void open();

public:
  void test(); /* <-- USUNĄĆ */
  void X(string s ) {file_name = s;}
  graf();
  ~graf();
  void load();
  void add_ARRRAY_ptrs();
  node<miasto>* get_city_wsk_by_id(const string id) const;
  // node<miasto>* get_city_wsk_by_id(const unsigned int ind) const;
  unsigned int get_ind(node<miasto>* ptr) const;
  polaczenia get_str_polaczenia(node<polaczenia>* p) const;
  miasto get_str_miasto(node<miasto>* m) const;
  node<polaczenia>* get_road(node<miasto>* c1, node<miasto>* c2) const;



  /* Zwraca odległość między miastami w linii krzywej (ziemia jest kulą) */
  double curve_distance(const node<miasto>* c1, const node<miasto>* c2) const;
  /* Ustawia c1, c2 na wierzchołki podanej krawędzi road */
  void endVertices(node<miasto>** c1, node<miasto>** c2, const node<polaczenia>* road) const;
  /* Zwraca przeciw legły wierzchołek względem podanej krawędzi road */
  node<miasto>* opposite(const node<miasto>* c1, const node<polaczenia>* road) const;
  /* Zwraca czy wierzchołki mają połączenie */
  bool areAdjacent(const node<miasto>* c1, const node<miasto>* c2) const;
  /* A* Astar */
  void algorytm_Astar(const string& city_1, const string& city_2);
};

#endif

/* Źródła kodu:
1. https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp
2. https://cpp0x.pl/kursy/Kurs-C++/Dodatkowe-materialy/Obsluga-plikow/305
3. https://learn.microsoft.com/pl-pl/cpp/cpp/try-throw-and-catch-statements-cpp?view=msvc-170
4. https://cpp0x.pl/forum/temat/?id=11057
5. https://en.cppreference.com/w/cpp/container/map

*/