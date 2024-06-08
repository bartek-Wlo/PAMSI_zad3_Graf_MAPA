#include "G_graf.h"

/*_________________________________________________________________________*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\  Astar  /‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾*/
/*                          Jedziemy od city 1. do  city 2.                */
void graf::algorytm_Astar(const string& city_1, const string& city_2) { 
  /* Lista Odwiedzonych Miast (zapisuje inteksy tablicy)                   */
  lista<SOM>* LOM = new lista<SOM>;
  /* Wsk do iterowania w najnowszym elem list LOM po wszytkich połączeniach*/        // XXX
  /* Wsk do iterowania w aktualnym elem list LOM po wszytkich połączeniach */
  /* Aktualny elem jest wskazywany przez wsk CCPL                          */
  node< node<polaczenia>* >* ptr_LOM = nullptr;
  /* tablica_indeksow_miast[LOM->front()] zwraca wsk liste połączeń miasta */
  /* Wsk do Konkretnej Struktury Połączenia. Inicjaliowany ze wsk ptr_LOM  */
  node<polaczenia>* ptr_conect = nullptr;
  /* Wsk do iterowania w po wszytkich elementach listy LOM                 */
  node<SOM>* display_ptr_LOM = nullptr;
  /* Wsk do aktualnie rozpatrywanego miasta (current city pointer LOM      */
  node<SOM>* CCPL = nullptr;

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
  /* Wsk na miasto po drugiej stronie połączenia, zwracane z opposite(...) */
  node<miasto>* opposite_city = nullptr; /* Zmienna do upożątkowania kodu  */
  /* Dodawanie indeksu miasta startowego do listy indeksów odwiedzonych m. */
  LOM->addFront( {indeks_map.at(city_1), nullptr } );
  CCPL = LOM->get_head(); /* Rozpatrujemy miasto startowe         */

  /* while( wsk najnowszego odwiedzonego miasta != wsk miasta docelowego)  */
  while(   false == areAdjacent( 
      tablica_indeksow_miast[LOM->get_elem(CCPL).index], DOCELOWE
    )   ) {
    ptr_LOM = tablica_sasiedztwa[LOM->get_elem(CCPL).index] -> get_head();

    cout 
      << lista_miast->get_elem(tablica_indeksow_miast[LOM->get_elem(CCPL).index]).name
      << "   first city in LOM: "
      << lista_miast->get_elem(tablica_indeksow_miast[LOM->front().index]).name 
      << endl;
    /* while( iteruje po elementach tablica_sasiedztwa[LOM->front().index] )    */        // XXX
    /* while( iteruje po elemen tablica_sasiedztwa[LOM->get_elem(CCPL).index] ) */
    while( ptr_LOM != nullptr ) {
      /* Wspomniana inicjSalizacja ptr_conect za pomocą ptr_LOM                  */
      /* tab...[LOM->front().index] wsk do listy wszystkich połaczeń tego miasta*/
      /*    ptr_LOM    iterujący tab[front()] zwraca wsk na node listy połączeń */
      /* ptr_conect    dostaje wsk na str z danymi połaczenia z tym miastem     */
      ptr_conect = tablica_sasiedztwa[LOM->get_elem(CCPL).index] -> get_elem(ptr_LOM);
      opposite_city = opposite(tablica_indeksow_miast[LOM->get_elem(CCPL).index], ptr_conect);
          cout
            << " długosc polączenia = "
            << get_str_polaczenia(ptr_conect).distance
            << "   curve distance = "
            << curve_distance(opposite_city, DOCELOWE)
            << "   " << lista_miast->get_elem(opposite_city).name
            << endl;
      /* hort-circuit evaluation, NIE sprawdzać 2' war. jeżeli 1.==nullptr */
      if (LOM->get_elem(CCPL).prev == nullptr || opposite_city != 
      tablica_indeksow_miast[LOM->get_elem(LOM->get_elem(CCPL).prev).index])
      {
        LFX->addFront({
          get_str_polaczenia(ptr_conect).distance +  /* G(x) + */
          curve_distance(opposite_city, DOCELOWE),   /* H(x)   */
          ptr_conect
        });
      }
      ptr_LOM = tablica_sasiedztwa[LOM->get_elem(CCPL).index] -> get_next(ptr_LOM);
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

// While (wsk_na_polączeni NIE zawiera tablica_indeksow_miast[LOM->front().index] )
// Czyli tak długo jak najnowsze miasto nie jest częścią najkrótszejego połączenia
// USUSŃ najnowsze odwiedzone miasto remove front()
// I usuń LFX tego miasta w pętli.
    //while( isTop(tablica_indeksow_miast[LOM->front().index],  
    while( isTop(tablica_indeksow_miast[LOM->get_elem(CCPL).index],
           LFX->get_elem(LFX_MIN).wsk_na_polaczenie) == false ) {
      // while (
      //     // lista_miast->get_elem( tablica_indeksow_miast[LOM->front().index] ).id ==
      //     // lista_polaczen->get_elem(LFX->front().wsk_na_polaczenie).city_1 ||
      //     // lista_miast->get_elem( tablica_indeksow_miast[LOM->front().index] ).id ==
      //     // lista_polaczen->get_elem(LFX->front().wsk_na_polaczenie).city_2
      //     lista_miast->get_elem( tablica_indeksow_miast[LOM->get_elem(CCPL).index] ).id ==
      //     lista_polaczen->get_elem(LFX->front().wsk_na_polaczenie).city_1 ||
      //     lista_miast->get_elem( tablica_indeksow_miast[LOM->get_elem(CCPL).index] ).id ==
      //     lista_polaczen->get_elem(LFX->front().wsk_na_polaczenie).city_2
      //   ) {
      //   LFX->removeFront();
      // }
      CCPL = LOM->get_next(CCPL);
      // LOM->removeFront();
    }

    LOM->addFront({  /* Dodaj do Listy Odwiedzonych Miast: {1, 2}          */
      indeks_map.at( /* Zamień id na indeks tego miasta                    */
        lista_miast->get_elem( /* Zamień wsk na node z miastem na id       */
          opposite(  /* Wzróć wsk na node z miastem c2 za połączeniem      */
            tablica_indeksow_miast[LOM->get_elem(CCPL).index],/* wsk na c1 */
            LFX->get_elem(LFX_MIN).wsk_na_polaczenie /* połaczenie c1-->c2 */
          )
        ).id  /* Dodaj do LOM node z 1: indeks najnowszego miasta          */
      ), CCPL /* Dodaj do LOM node z 2: wsk na node z poprzednim miastem   */
    });
    CCPL = LOM->get_head();
    
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

  /*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\ DEBUG DISPLAY /‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/

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
        tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM).index]
      ).id
      << " lati: " << left << setw(10) <<
      lista_miast->get_elem(
        tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM).index]
      ).latitude
      << " long: " << left << setw(10) <<
      lista_miast->get_elem
        (tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM).index]
      ).longitude
      << " name: " << left << setw(20) <<
      lista_miast->get_elem(
        tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM).index]
      ).name
      << std::endl;

    display_ptr_LOM = LOM->get_next(display_ptr_LOM);
  }

  cout << endl;

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
        tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM).index]
      ).id
      << " lati: " << left << setw(10) <<
      lista_miast->get_elem(
        tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM).index]
      ).latitude
      << " long: " << left << setw(10) <<
      lista_miast->get_elem
        (tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM).index]
      ).longitude
      << " name: " << left << setw(20) <<
      lista_miast->get_elem(
        tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM).index]
      ).name
      << std::endl;

    display_ptr_LOM = LOM->get_elem(display_ptr_LOM).prev;
  }
  

  delete LOM; 
  delete LFX;
  LOM = nullptr;
  LFX = nullptr;
  return;
}