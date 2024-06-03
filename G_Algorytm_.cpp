#include "G_Algorytm_.h"

void A_::algorytm_(const string& city_1, const string& city_2) { /* Jedziemy od 1. do 2. */
  /* node<miasto> *C1 = G->get_city_wsk_by_id(city_1); */
  node<miasto> *C2 = G->get_city_wsk_by_id(city_2);
  
  LOM->addFront( G->get_city_wsk_by_id(city_1) );
  while( G->areAdjacent(LOM->front(), C2) ) {
    while( iteruje po LOM) {
/* ^127 -> polega na zampowaniu nazwy miasta i odwołaniu sie do TAB tablica_sasiedztwa */
      while( iteruje po każdum połączeniu pojedynczego elemnu LOM ) { /* <-- ^127 */
        Dla każdego połączenia z elem LOM liczy liczy g(x)+h(x)
        Wpisuje Je do nowej listy <f_od_X>.int = f(x) = g(x)+h(x)
        Oraz wpisuje skaźnik na polaczenie  <f_od_X>.wsk_na_polaczenie
          z którego połączenie liczymy
      }
    }
    szuka: MIN( lista<f_od_X>.int ) 
    PO zanalezieniu zwraca wskaźnik na połączenie wsk_na_polaczenie
    Szuka miasta po drugiej stronie połączenia wsk_na_polaczenie
    LOM->addFront( opposite(wsk_na_polaczenie) );
  }
  Wyświetla liste LOM
  Iteruje po kolejych elementach lom -> LOM_n i LOM_n+1 i korzysta z metody:
    get_road albo areAdjacent (raczej get_road) i zczytuje dane  które to zwraca
}

// STRUCT -> int; wsk_na_polaczenie; 
struct f_od_X {
  int f;
  node<polaczenia> *wsk_na_polaczenie;
};


/* OPTYMALIZACJA             USUNĄĆ 2' WHILE()
-> 
while( ...
  while( interuje po LOM  ----> USUNĄĆ -----> Bo Iteruje typo po FRONT()
    while( iteruje tylko po każym elemencie FRON()
*/