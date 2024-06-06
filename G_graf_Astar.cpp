#include "G_graf.h"

/*_________________________________________________________________________*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\  Astar  /‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾*/
/*                          Jedziemy od city 1. do  city 2.                */
void graf::algorytm_Astar(const string& city_1, const string& city_2) { 
  /* Lista Odwiedzonych Miast (zapisuje inteksy tablicy)                   */
  lista<unsigned int>* LOM = new lista<unsigned int>;
  /* Wsk do iterowania w najnowszym elem list LOM po wszytkich połączeniach*/
  node< node<polaczenia>* >* ptr_LOM = nullptr;
  /* tablica_indeksow_miast[LOM->front()] zwraca wsk liste połączeń miasta */
  /* Wsk do Konkretnej Struktury Połączenia. Inicjaliowany ze wsk ptr_LOM  */
  node<polaczenia>* ptr_conect = nullptr;
  /* Wsk do iterowania w po wszytkich elementach listy LOM                 */
  node<unsigned int>* display_ptr_LOM = nullptr;

  /* Lista F(x) = G(x) + H(x) zapisuje obliczone wartości f. + wsk na pol. */
  lista<Fx>* LFX = new lista<Fx>;
  /* Wsk do iterowanie po list LFX                                         */
  node<Fx>* ptr_LFX = nullptr;
  /* Wsk do najmniejszego elementu listy LFX                               */
  node<Fx>* LFX_MIN = nullptr;
  /* Najmniejsza wartość wpisanych F(x)                                    */
  double FxMIN;

  /* Wsk na miasto docelowe (city_2)                                       */
  const node<miasto>* DOCELOWE = get_city_wsk_by_id(city_2);
  /* Dodawanie indeksu miasta startowego do listy indeksów odwiedzonych m. */
  LOM->addFront( indeks_map.at(city_1) );

  /* while( wsk najnowszego odwiedzonego miasta != wsk miasta docelowego)  */
  while( areAdjacent( tablica_indeksow_miast[LOM->front()], DOCELOWE ) == false ) {
    ptr_LOM = tablica_sasiedztwa[LOM->front()] -> get_head();

    cout << lista_miast->get_elem(tablica_indeksow_miast[LOM->front()]).name << endl;
    /* while( iteruje po elementach tablica_sasiedztwa[LOM->front()] )    */
    while( ptr_LOM != nullptr ) {
      ptr_conect = tablica_sasiedztwa[LOM->front()] -> get_elem(ptr_LOM);
      cout
        << " długosc polączenia = "
        << get_str_polaczenia(ptr_conect).distance
        << "   curve distance = "
        << curve_distance(
          opposite(tablica_indeksow_miast[LOM->front()], ptr_conect),
          DOCELOWE
        )
        << "   " << lista_miast->get_elem(
          opposite(tablica_indeksow_miast[LOM->front()], ptr_conect)
        ).name
        << endl;
      LFX->addFront({
        get_str_polaczenia(ptr_conect).distance +            /* G(x) + H(x)*/
        curve_distance(
          opposite(tablica_indeksow_miast[LOM->front()], ptr_conect),
          DOCELOWE
        ),
        ptr_conect
      });
      ptr_LOM = tablica_sasiedztwa[LOM->front()] -> get_next(ptr_LOM);
    }

    /* CO JEŻELI LFX jest empty ? <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
    if (LFX->empty()) throw std::logic_error ("LFX->empty() == TRUE");

    ptr_LFX = LFX->get_head();
    LFX_MIN = ptr_LFX;
    FxMIN = LFX->front().Gx_Hx;
    /* while( iteruje po elementach LFX (lista F(x) ) Szuka F_{MIN}(x)     */
    while(ptr_LFX != nullptr) {
      cout 
        << " F(x)= "<< LFX->get_elem(ptr_LFX).Gx_Hx << " "
        << " C1: " 
        << lista_polaczen->get_elem(LFX->get_elem(ptr_LFX).wsk_na_polaczenie).city_1
        << " C2: "
        << lista_polaczen->get_elem(LFX->get_elem(ptr_LFX).wsk_na_polaczenie).city_2
        << endl; 
      if (LFX->get_elem(ptr_LFX).Gx_Hx < FxMIN) {
        FxMIN = LFX->get_elem(ptr_LFX).Gx_Hx;
        LFX_MIN = ptr_LFX;
      }
      ptr_LFX = LFX->get_next(ptr_LFX);
    }
    cout << endl;

// While (wsk_na_polączeni NIE zawiera tablica_indeksow_miast[LOM->front()] )
// Czyli tak długo jak najnowsze miasto nie jest częścią najkrótszejego połączenia
// USUSŃ najnowsze odwiedzone miasto remove front()
// I usuń LFX tego miasta w pętli.
    while( isTop(tablica_indeksow_miast[LOM->front()],   
           LFX->get_elem(LFX_MIN).wsk_na_polaczenie) == false ) {
      while (
          lista_miast->get_elem( tablica_indeksow_miast[LOM->front()] ).id ==
          lista_polaczen->get_elem(LFX->front().wsk_na_polaczenie).city_1 ||
          lista_miast->get_elem( tablica_indeksow_miast[LOM->front()] ).id ==
          lista_polaczen->get_elem(LFX->front().wsk_na_polaczenie).city_2
        ) {
        LFX->removeFront();
      }
      LOM->removeFront();
    }

    LOM->addFront( 
      indeks_map.at(
        lista_miast->get_elem(
          opposite(  
            tablica_indeksow_miast[LOM->front()], 
            LFX->get_elem(LFX_MIN).wsk_na_polaczenie
          )
        ).id
      )
    );
    cout 
      << " Dlugość między miastami F(x) = G(x) + H(x) = "
      << LFX->get_elem(LFX_MIN).Gx_Hx
      << "    city 1: "
      << lista_polaczen->get_elem(LFX->get_elem(LFX_MIN).wsk_na_polaczenie).city_1
      << "    city 2: "
        << lista_polaczen->get_elem(LFX->get_elem(LFX_MIN).wsk_na_polaczenie).city_2
      << endl;
    LFX->removeInside(LFX_MIN);
  }


  display_ptr_LOM = LOM->get_head();
  cout 
    << "id: " << left << setw(20) << lista_miast->get_elem(DOCELOWE).id
    << " lati: " << left << setw(10) << lista_miast->get_elem(DOCELOWE).latitude
    << " long: " << left << setw(10) << lista_miast->get_elem(DOCELOWE).longitude
    << " name: " << left << setw(20) << lista_miast->get_elem(DOCELOWE).name
    << std::endl;
  while (display_ptr_LOM != nullptr) {
    cout 
      << "id: " << left << setw(20) <<
      lista_miast->get_elem(
        tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM)]
      ).id
      << " lati: " << left << setw(10) <<
      lista_miast->get_elem(
        tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM)]
      ).latitude
      << " long: " << left << setw(10) <<
      lista_miast->get_elem
        (tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM)]
      ).longitude
      << " name: " << left << setw(20) <<
      lista_miast->get_elem(
        tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM)]
      ).name
      << std::endl;

    display_ptr_LOM = LOM->get_next(display_ptr_LOM);
  }

  delete LOM; 
  delete LFX;
  LOM = nullptr;
  LFX = nullptr;
}