#include <iostream>
#include "G_graf.h"

int main(int argc, char *argv[]) {
  string miasto_1, miasto_2, LENGTH_TIME, maxclass, minclass;
  if (argc < 3) {cerr << "Not enough arguments" << endl; return argc;}
  /* throw std::invalid_argument ("Not enough arguments"); */
  miasto_1 = argv[1];
  miasto_2 = argv[2];
  if (argc >= 4) LENGTH_TIME = argv[3]; 
  else LENGTH_TIME = "LENGTH";
  if (argc >= 5) maxclass = argv[4];
  if (argc >= 6) minclass = argv[5];

  
  graf G;
  
  if (argc >= 7) G.set_file_name(argv[6]);
  
  try {G.load();} /*[3.]*/
  catch (json::parse_error& e) {cerr<<"Parse error: "<<e.what()<<endl;return -1;} 
  catch (const std::exception& e) {cerr<<"Exception: "<<e.what()<<endl;return -2;}
  
  G.add_ARRRAY_ptrs();
  
  if(LENGTH_TIME == "TIME") G.set_time(true);
  else G.set_time(false);

  G.algorytm_Astar(miasto_1, miasto_2);
  G.algorytm_Dijkstra(miasto_1, miasto_2);
  // G.test();


  return 0;
}


