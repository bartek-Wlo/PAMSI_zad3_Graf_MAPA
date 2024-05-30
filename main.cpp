#include <iostream>
#include "G_graf.h"

int main() {
  std::cout << "Hello World!\n";
  //test();
  graf G;
  try {G.load();}
  catch (json::parse_error& e) {cerr<<"Parse error: "<<e.what()<<endl;} /*[3.]*/
  catch (const std::exception& e) {cerr<<"Exception: "<<e.what()<<endl;}
  G.add_ARRRAY_ptrs();

  
  return 0;
}

