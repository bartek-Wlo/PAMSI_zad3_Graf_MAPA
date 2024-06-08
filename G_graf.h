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
  map<string, unsigned int> indeks_map;

  void open();

public:
  void test(); /* <-- USUNĄĆ */
void set_file_name(string s) {file_name = s;}
  graf();
  ~graf();
  void load();
  void load_few();
  void add_ARRRAY_ptrs();
  node<miasto>* get_city_wsk_by_id(const string id) const;
  // node<miasto>* get_city_wsk_by_id(const unsigned int ind) const;
  unsigned int get_ind(node<miasto>* ptr) const;
  polaczenia get_str_polaczenia(node<polaczenia>* p) const;
  polaczenia get_str_polaczenia(const node< node<polaczenia>* >* p,
                                const unsigned int i) const;
  miasto get_str_miasto(node<miasto>* m) const;
  node<polaczenia>* get_road(node<miasto>* c1, node<miasto>* c2) const;


  /* Zwraca odległość między miastami w linii krzywej (ziemia jest kulą) */
  double curve_distance(const node<miasto>* c1, const node<miasto>* c2) const;
  /* Ustawia c1, c2 na wierzchołki podanej krawędzi road */
  void endVertices(node<miasto>** c1, node<miasto>** c2, const node<polaczenia>* road) const;
  /* Zwraca przeciw legły wierzchołek względem podanej krawędzi road */
  bool isTop(const node<miasto>* c1, const node<polaczenia>* road) const;
  /* Zwraca przeciw legły wierzchołek względem podanej krawędzi road */
  node<miasto>* opposite(const node<miasto>* c1, const node<polaczenia>* road) const;
  /* Zwraca czy wierzchołki mają połączenie */
  bool areAdjacent(const node<miasto>* c1, const node<miasto>* c2) const;
  /* A* Astar */
  void algorytm_Astar(const string& city_1, const string& city_2);
  /* Dijkstra */
  void algorytm_Dijkstra(const string& city_1, const string& city_2);

private:
/* A* Astar */
/* Dijkstra */
  void pasek_postepu_std_err_display();


};

#endif

/* Źródła kodu:
1. https://github.com/nlohmann/json/blob/develop/single_include/nlohmann/json.hpp
2. https://cpp0x.pl/kursy/Kurs-C++/Dodatkowe-materialy/Obsluga-plikow/305
3. https://learn.microsoft.com/pl-pl/cpp/cpp/try-throw-and-catch-statements-cpp?view=msvc-170
4. https://cpp0x.pl/forum/temat/?id=11057
5. https://en.cppreference.com/w/cpp/container/map
6. https://stackoverflow.com/questions/8690567/setting-an-int-to-infinity-in-c

*/

/* Komentarz:                                         (Do struktury listy tablicy połaczeń)
   lista< node<polaczenia>* >** tablica_sasiedztwa
   Jest tablicą wskaźników  ->  listę wskaźników  ->  node listy typu lista<polaczenia>.

   Co jest w zasadzie nie dokońca idealne, ponieważ powinny być:
   tablicą wskaźników  ->  listę wskaźników  ->  elementy node'ów listy typu lista<polaczenia>.
   inaczej (napisane to samo):
   tablicą wskaźników  ->  listę wskaźników  ->  poszczególne struktury "polaczenia".
   
   To niedociągnięcie powoduje, że w całym programie, korzystając z tablica_sasiedztwa,
     po odwołaniu się do konkretnej komórki tablicy dostajemy wskaźnik na całą listę
     wszystkich połączeń jakiegoś miasta. 
   Kiedy zaś wybieramy konkretny node tej listy, to jego elementem nie będzie wskaźnik
     na strukturę zwierającą informacje o połączeniu, 
     tylko wskaźnik na kolejnego noda, co ozacza że trzeba jeszcze raz wywołać funkjcę
     zwracającą element.
*/