#include "G_Algorytm_.h"



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