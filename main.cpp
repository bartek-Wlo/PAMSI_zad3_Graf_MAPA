#include <iostream>
#include "G_graf.h"

int main() {
  graf G;
  try {G.load();} /*[3.]*/
  catch (json::parse_error& e) {cerr<<"Parse error: "<<e.what()<<endl;return -1;} 
  catch (const std::exception& e) {cerr<<"Exception: "<<e.what()<<endl;return -2;}
  
  G.add_ARRRAY_ptrs();
  
  G.test();

  std::cout << "\n\nGood bye World!\n";
  return 0;
}


