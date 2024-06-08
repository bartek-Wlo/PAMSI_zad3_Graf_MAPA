#include "G_graf.h"

/*_________________________________________________________________________*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/  Dijkstra   \_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾*/
/*                             Jedziemy od city 1. do  city 2.             */
void graf::algorytm_Dijkstra(const string& city_1, const string& city_2) {
  const unsigned int liczba_miast = lista_miast->get_nodeNumber();
  
  /* https://pl.wikipedia.org/wiki/Algorytm_Dijkstry                       */
  /* d -> odległości od źródła dla wszystkich wierzchołków grafu           */
  double* distance = new double[liczba_miast];
  /* Czy miasto już zostało wpisane do listy Queue                         */
  bool* visited = new bool[liczba_miast];
  /* Tablica indeksów na poprzednie miasto na drodzie do celu              */
  unsigned int* prev = new unsigned int[liczba_miast];
  /* kolejka priorytetowa Q -> Wierzchołki którym raz wyzaczano połaczenie */
  lista<unsigned int>* Queue = new lista<unsigned int>;

  /* Wskaźnik na Queue do usuwania wierzchołków z wyzanczonymi połączeniami*/
  node<unsigned int>* delete_elem_Queue = nullptr;
  /* Wskaźnik do listy połaczeń aktualnie rozpatrywanego miasta.           */
  node< node<polaczenia>* >* ptr_TSC = nullptr; /* Tab Sąsiedztwa Current  */
  /* Wskaźnik do struktury z jednym połączeniem  aktualnego miasta         */
  node<polaczenia>* wsk_SPCC = nullptr;/* Struktura Połączenia Current City*/

  /* Czy miasto docelowe, było już odwiedzone  (Wyznacano jego połączenia) */
  bool city_2_visited = false;
  /* Indeks aktualnie rozpatrywanego miasta.                               */
  unsigned int current = indeks_map.at(city_1);
  /* 1. Ideks miasta po drugiej stronie połączenia                         */
  /* 2. Ideks do iterowania po kolejce Queue i  wybrania kolejnego miasta  */
  /* 3. Ideks do zapisywania prev, przy wświetlaniu trasy.                 */
  unsigned int city_index;
  /* Dystans: trasa do tego miasta + połaczenie z następnym.               */
  double curr_distance;

  try{
    for (unsigned int i = 0; i < liczba_miast; ++i) {
      distance[i] = std::numeric_limits<double>::infinity(); /* [6.] */
      visited[i] = false;
      prev[i] = liczba_miast; /* Ustawia indeks na liczbę poza tablicą     */
    }
    distance[current] = 0;
    visited[current] = true;
    prev[current] = current;
    
    while (city_2_visited == false) {
      ptr_TSC = tablica_sasiedztwa[current] -> get_head();
      while( ptr_TSC != nullptr ) {
        wsk_SPCC = tablica_sasiedztwa[current] -> get_elem(ptr_TSC);
        city_index = get_ind(opposite(tablica_indeksow_miast[current], wsk_SPCC));
        curr_distance = get_str_polaczenia(wsk_SPCC).distance + distance[current];
  
        if(distance[city_index] > curr_distance) {
          distance[city_index] = curr_distance;
          prev[city_index] = current;
        }
        if(visited[city_index] == false) {
          Queue->addFront(city_index);
          visited[city_index] = true;
        }
        ptr_TSC = tablica_sasiedztwa[current] -> get_next(ptr_TSC);
      }
      city_index = Queue->front();
      delete_elem_Queue = Queue->get_head();
      for(node<unsigned int>* q = Queue->get_head(); q != nullptr; q = Queue->get_next(q)) {
        if (distance[city_index] > distance[Queue->get_elem(q)]) {
          city_index = Queue->get_elem(q);
          delete_elem_Queue = q;
        }
      }
      Queue->removeInside(delete_elem_Queue);
      current = city_index;
      if(current == indeks_map.at(city_2)) city_2_visited = true;
      /* pasek_postepu_std_err_display(); */
      
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
    unsigned int liczba_nie_odwiedzonych_miast = 0;
    unsigned int liczba_____odwiedzonych_miast = 0;
    for(unsigned int i = 0; i < liczba_miast; ++i) {
      if(visited[i] == false) ++liczba_nie_odwiedzonych_miast;
      else ++liczba_____odwiedzonych_miast;
    }
    cerr 
      << "Exception: " << e.what() << endl
      << "     type: " << typeid(e).name() << endl
      << "      fun: G_graf.cpp -> void algorytm_Dijkstra(const string&, const string&)"
      << endl
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



/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\  Pasek postępu  /‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
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

