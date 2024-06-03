#include <iostream>
#include "G_Algorytm_.h"

int main() {
  graf G;
  try {G.load();}
  catch (json::parse_error& e) {cerr<<"Parse error: "<<e.what()<<endl;} /*[3.]*/
  catch (const std::exception& e) {cerr<<"Exception: "<<e.what()<<endl;}
  G.X("json/dolnoslaski.json"); G.load();
  G.X("json/kujawsko-pomorskie.json"); G.load();
  G.X("json/podlaskie-wm-poprawione.json"); G.load();
  
  G.add_ARRRAY_ptrs();
  
  
  G.test();

  std::cout << "\n\nGood bye World!\n";
  return 0;
}

