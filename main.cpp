#include <iostream>
#include "G_graf.h"

int main() {
  graf G;
  G.load_few();
  
  G.add_ARRRAY_ptrs();
  
  G.test();

  std::cout << "\n\nGood bye World!\n";
  return 0;
}

/* Przebudować: lista< node<polaczenia>* >** tablica_sasiedztwa = nullptr;
   na         : lista< polaczenia* >** tablica_sasiedztwa = nullptr;  
   
   
   
   2. Dodać zabezpieczenie by algorytm nie chodził w koło po tych samych miastach 
   czyli nie dodawać połączaeń do LFX do miats z których właśnie przyszliśmy */

