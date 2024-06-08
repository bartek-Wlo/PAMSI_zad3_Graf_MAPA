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


