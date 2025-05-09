#include <iostream>
#include "G_graf.h"

int main(int argc, char *argv[]) {
  string miasto_1, miasto_2, LENGTH_TIME, maxclass, minclass;
  graf G;
  
  if (argc < 3) {
    cerr << "Not enough arguments" << endl;
    cout << "If the parameter is not to be set, enter 0." << endl;
    cout << "Enter the name of the starting city: "; cin >> miasto_1;
    cout << "Enter the name of the destination city: "; cin >> miasto_2;
    cout << "The route is to be calculated according to TIME or distance: ";
    cin >> LENGTH_TIME;
    cout << "Enter the maximum class: "; cin >> maxclass;
    cout << "Enter the minimum class: "; cin >> minclass;
    try {G.set_accessible_road(minclass, maxclass);}
    catch (const std::invalid_argument& e) {cerr << "NOTFAUND" << endl; return -1;}
  } else {
    miasto_1 = argv[1];
    miasto_2 = argv[2];
    if (argc >= 4) LENGTH_TIME = argv[3]; 
    else LENGTH_TIME = "LENGTH";

    try {
      if  (argc >= 6) G.set_accessible_road(argv[5], argv[4]);
      else if (argc == 5) G.set_accessible_road("0", argv[4]);
      else /* argc < 5 */ G.set_accessible_road("0", "0");
    } catch(const std::invalid_argument& e){cerr << "NOTFAUND" << endl; return -2;}

    if (argc >= 7) G.set_file_name(argv[6]);
  }

  
  try {G.load();} /*[3.]*/
  catch (json::parse_error& e) {cerr<<"Parse error: "<<e.what()<<endl; return -3;} 
  catch (const std::exception& e) {cerr<<"Exception: "<<e.what()<<endl;return -4;}
  
  G.add_ARRRAY_ptrs();
  
  if(LENGTH_TIME == "TIME" ) G.set_time(true);
  else G.set_time(false);

  G.algorytm_Astar(miasto_1, miasto_2);
  // G.algorytm_Dijkstra(miasto_1, miasto_2);

  return 0;
}


