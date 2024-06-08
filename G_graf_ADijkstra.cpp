#include "G_graf.h"

void graf::algorytm_Dijkstra(const string& city_1, const string& city_2) {
  const unsigned int liczba_miast = lista_miast->get_nodeNumber();
  /* https://pl.wikipedia.org/wiki/Algorytm_Dijkstry */
  /* d -> odległości od źródła dla wszystkich wierzchołków grafu           */
  double* distance = new double[liczba_miast];
  bool* visited = new bool[liczba_miast];
  bool city_2_visited = false;
  unsigned int* prev = new unsigned int[liczba_miast];
  /* kolejks priorytetowa Q -> nieodwiedzone spotkane wierzchołki          */
  lista<unsigned int>* Queue = new lista<unsigned int>;
  node<unsigned int>* delete_elem_Queue = nullptr;
  node< node<polaczenia>* >* ptr_TSC = nullptr; /* Tab Sąsiedztwa Current  */
  node<polaczenia>* wsk_SPCC = nullptr;/* Struktura Połączenia Current City*/
  unsigned int current = indeks_map.at(city_1);
  unsigned int city_index;
  double curr_distance;

  try{
  for (unsigned int i = 0; i < liczba_miast; ++i) {
    distance[i] = std::numeric_limits<double>::infinity(); /* [6.] */
    visited[i] = false;
    prev[i] = liczba_miast; /* Ustawia indeks na liczbę poza tablicą      */
  }
  distance[current] = 0;
  visited[current] = true;
  prev[current] = current;
  
  while (city_2_visited == false) {
    cout <<"|  "<<get_str_miasto(tablica_indeksow_miast[current]).name << endl;
    ptr_TSC = tablica_sasiedztwa[current] -> get_head();
    while( ptr_TSC != nullptr ) {
      // cout 
      //   << " C1: "<<left<< setw(15) << get_str_polaczenia(ptr_TSC, current).city_1
      //   << " C2: "<<left<<setw(15) << get_str_polaczenia(ptr_TSC, current).city_2;
      //   // << " distance: " << get_str_polaczenia(ptr_TSC, current).distance;
      //   // << endl; 
      wsk_SPCC = tablica_sasiedztwa[current] -> get_elem(ptr_TSC);
      city_index = get_ind(opposite(tablica_indeksow_miast[current], wsk_SPCC));
      curr_distance = get_str_polaczenia(wsk_SPCC).distance + distance[current];
      // cout
      //   << "       dis[city_index] " << distance[city_index]
      //   << "  curr_dis " << curr_distance
      //   << "  " << get_str_miasto(tablica_indeksow_miast[city_index]).name
      //   << endl;
      if(distance[city_index] > curr_distance) {
        distance[city_index] = curr_distance;
        prev[city_index] = current;
      }
      if(visited[city_index] == false) {
        Queue->addFront(city_index);
        visited[city_index] = true;
      } //else cout << "VISITED!!!\n";
      ptr_TSC = tablica_sasiedztwa[current] -> get_next(ptr_TSC);
    }
    city_index = Queue->front();
    delete_elem_Queue = Queue->get_head();
    for(node<unsigned int>* q = Queue->get_head(); q != nullptr; q = Queue->get_next(q)) {
      cout 
        << "distance[city_index] " 
        <<left<<setw(6)<< distance[city_index]
        << "    distance[Queue->get_elem(q)] " 
        <<left<<setw(6)<< distance[Queue->get_elem(q)]
        <<left<<setw(15)<< get_str_miasto(tablica_indeksow_miast[city_index]).name
        <<" "<<left<<setw(15)
        << get_str_miasto(tablica_indeksow_miast[Queue->get_elem(q)]).name
        << endl;
      if (distance[city_index] > distance[Queue->get_elem(q)]) {
        city_index = Queue->get_elem(q);
        delete_elem_Queue = q;
      }
    }
    cout << endl;
    Queue->removeInside(delete_elem_Queue);
    // cout << "Current city " << city_index 
    //   << "  " << get_str_miasto(tablica_indeksow_miast[city_index]).name
    //   << endl;
    current = city_index;
    //visited[current] = true;
    if(current == indeks_map.at(city_2)) city_2_visited = true;
    pasek_postepu_std_err_display();
    

    // cin >> city_index; ///////////////////////////////////////////////////
  }
    city_index = indeks_map.at(city_2);
    while (city_index != indeks_map.at(city_1)){
      cerr 
        << get_str_miasto(tablica_indeksow_miast[city_index]).name
        << endl;
      city_index = prev[city_index];
    }
    cerr 
    << get_str_miasto(tablica_indeksow_miast[city_index]).name
    << endl;
    

    
  } catch (const std::exception& e) {
    cerr 
      << "Exception: " << e.what() << endl
      << "     type: " << typeid(e).name() << endl
      << "      fun: G_graf.cpp -> void graf::algorytm_Dijkstra(const string& city_1, const string& city_2)"
      << endl;
      //<< " Wrong ID: " << id << endl;
    
    unsigned int liczba_nie_odwiedzonych_miast = 0;
    unsigned int liczba_____odwiedzonych_miast = 0;
    for(unsigned int i = 0; i < liczba_miast; ++i) {
      if(visited[i] == false) ++liczba_nie_odwiedzonych_miast;
      else ++liczba_____odwiedzonych_miast;
    }
    cerr 
      << "Liczba nie odwiedzoych miast: " << liczba_nie_odwiedzonych_miast << endl
      << "   Liczba odwiedzonych miast: " << liczba_____odwiedzonych_miast << endl
      << "             Wszytkie miasta: " 
      << liczba_nie_odwiedzonych_miast + liczba_____odwiedzonych_miast << endl
      << "            Wielkość tablicy: " << liczba_miast << endl;
  }
  
  delete[] distance;
  delete[] visited;
  delete[] prev;
  delete   Queue;
  distance = nullptr;
  visited  = nullptr; 
  prev     = nullptr;
  Queue    = nullptr;
  return;
}

// ///////////////////////////////////////////////////


void graf::pasek_postepu_std_err_display() {
  static int zakres = lista_miast->get_nodeNumber();
  static int licznik = 0;
  static int pamiec = 0;
  ++licznik;
  int procent = (licznik * 100) / zakres;
  /* każde wywołanie f. zwiększa licznik +1, przy tworzeniu procent. */

  if (pamiec < procent - 4) {
    pamiec = procent;
    /* https://cpp0x.pl/forum/temat/?id=11057 setw(), <iomanip> */
    cerr << "( " <<setw(4)<< licznik << " / " <<setw(4)<< zakres << " )";
    cerr << " " <<setw(3)<< procent << " % |";// << endl;
    for (int i=5; i<= procent; i+=5) cerr << "▇";
    for (int j=100; j>procent; j-=5) cerr << " ";
      cerr << "|" << endl;
  }
}

