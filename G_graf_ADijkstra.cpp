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
  
  /* Czy miasto docelowe, było już odwiedzone  (Wyznacano jego połączenia) */
  bool city_2_visited = false;
  /* Indeks aktualnie rozpatrywanego miasta.                               */
  unsigned int current = indeks_map.at(city_1);
  /* Ideks do zapisywania prev, przy wświetlaniu trasy.                    */
  unsigned int city_index;

  try{
    /* ################################ 1 ################################ */
    /* ########## Ustawnianie wyjściowych wartości  w tablicach ########## */
    for (unsigned int i = 0; i < liczba_miast; ++i) {
      distance[i] = std::numeric_limits<double>::infinity(); /* [6.] */
      visited[i] = false;     /* Ustawianie początkowych parametrów miastom*/
      prev[i] = liczba_miast; /* Ustawia indeks na liczbę poza tablicą     */
    }
    distance[current] = 0;  /* Ustawianie parametrów dla miata wyjściowego */
    visited[current] = true;
    prev[current] = current;

    /* ################################ 2 ################################ */
    /* ####### Odwiedzamy każde miasto  aż dojdziemy do docelowego ####### */
    while (city_2_visited == false) {
      aktualizuj_dystans_i_dodaj_do_Queue(current, distance, visited, prev, Queue);
      current = szukaj_min_Queue(distance, Queue);
      /* Jeżeli dodtarliśmy do miasta docelowego city_2_visited = true     */
      if(current == indeks_map.at(city_2)) city_2_visited = true;
      /* pasek_postepu_std_err_display(); */ 
    }
    /* ################################ 3 ################################ */
    /* ################## Wyświtlanie ścieżki do miasta ################## */
    city_index = indeks_map.at(city_2);
    cout 
      << "Dystans: " << distance[indeks_map.at(city_2)] 
      << "   Wężły przeanalizowane: " << Queue->get_nodeNumber()+1 << endl;
    while (city_index != indeks_map.at(city_1)) {
      cerr 
        << "name: "
        << get_str_miasto(tablica_indeksow_miast[city_index]).name
        << endl;
      city_index = prev[city_index];
    }
    cerr 
      << "name: "
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
      << "      fun: G_graf_ADijkstra.cpp -> void algorytm_Dijkstra(string&, string&)"
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
/*/‾\_/‾\_/‾\ Aktualizuj dystans wierzchołków  do wyjściowego /‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
void graf::aktualizuj_dystans_i_dodaj_do_Queue(const unsigned int current,
                    double* distance, bool* visited, unsigned int* prev,
                    lista<unsigned int>* Queue) const {
  /* Wskaźnik na node ze wskaźnikiem do node<polaczenia>                   */
  /* Służący do iteracji po wszytkich połączeniach danego miasta           */
  node< node<polaczenia>* >* ptr_TSC = nullptr; /* Tab Sąsiedztwa Current  */
  /* Wskaźnik do node<polaczenia> z jednym połączeniem aktualnego miasta   */
  node<polaczenia>* wsk_SPCC = nullptr;/* Struktura Połączenia Current City*/
  /* Ideks miasta po drugiej stronie aktulanego połączenia                 */
  unsigned int city_index;
  /* Dystans: trasa do tego miasta + połaczenie z następnym.               */
  double curr_distance;
  double predkosc;

  ptr_TSC = tablica_sasiedztwa[current] -> get_head();
  /* Pętla aż ptr_TSC przejdzie po wszytkich połączeniach danego miasta    */
  while( ptr_TSC != nullptr ) {
    /* Wskaźnik SPCC do noda z aktualnie rozpatrywanym połączeniem.        */
    wsk_SPCC = tablica_sasiedztwa[current] -> get_elem(ptr_TSC);
    if (time == true) {
      predkosc = get_speed_limit(get_str_polaczenia(wsk_SPCC).road_type);
    } else predkosc = 1;
    /* Ideks miasta po drugiej stronie połaczenia. opos(Miasto, połączenie)*/
    city_index= get_ind(opposite(tablica_indeksow_miast[current], wsk_SPCC));
    /* Dystance = dystans połączenia + dystan do aktualnego miasta.        */
    curr_distance = 
      distance[current] + (get_str_polaczenia(wsk_SPCC).distance / predkosc);

    if(distance[city_index] > curr_distance) { /* Jeżeli dotychczasowy     */
      distance[city_index] = curr_distance;    /* dystans > nowej ścieżki  */
      prev[city_index] = current;              /* to podmień długość oraz  */
    }                                          /* Zamień poprzednie miasto */
    if(visited[city_index] == false) { /* Dodaj do rozpatrywanych miast (Q)*/
      Queue->addFront(city_index);     /* Miasto oppos gdy nie wyznaczano  */
      visited[city_index] = true;      /* do niego wcześniej zadnej ścieżki*/
    }
    /* Przesuń na koleine połaczenie dla danego miasta                     */
    ptr_TSC = tablica_sasiedztwa[current] -> get_next(ptr_TSC);
  } /* Queue przechowuje indeksy miast do których już raz wyznaczono trasę,*/
}   /* więc jest sens rozpatrywać je jako kolejne miasto do odwiedzenia.   */




/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\  Szukanie miasta z najmniejszym dystancem do wyjściowego  \_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
unsigned int graf::szukaj_min_Queue(double* distance,
                                lista<unsigned int>* Queue) const {
  /* Wskaźnik na Queue mówiący gdzie znajduje się najmniejszy element      */
  /* Ustawiamy wskaźnik na pierwszy node Queue.                            */
  node<unsigned int>* delete_elem_Queue = Queue->get_head();
  /* Ideks do iterowania po kolejce Queue i  wybrania kolejnego miasta     */
  /* Ustawiamy odeks na pierwszy element Queue.                            */
  unsigned int city_index = Queue->front();

  if(Queue->empty()) throw std::runtime_error("Queue with visited cities is empty.\n           G_graf_ADijkstra.cpp -> szukaj_min_Q(double*,lista<unsigned int>*)"); 
  
  for(node<unsigned int>* q = Queue->get_head(); 
                  Queue->get_next(q) != nullptr;) {
    /* Zacznyna od przesunięcia by nie porównywać pierwszego elem ze sobą. */
    q = Queue->get_next(q);
    /* Jeżeli dystans aktualnie najmniejszy jest większy niż kolejny to:   */
    if (distance[city_index] > distance[Queue->get_elem(q)]) {
      city_index = Queue->get_elem(q);  /* 1. Ustaw indeks najmnijeszego   */
      delete_elem_Queue = q;            /* 2. Nastaw na niego wskaźnik Q   */
    }
  }
  /* Usuń najmniejszy element, bo w nastęnej pętli programu zwrócona przez */
  /* tą funcje wartość nastawi aktualnie rozpatrywane miasto na właśnie    */
  /* usuwane z Queue. Która to służy do szukania kolejnych miast do        */
  /* odwiedzenia. A miasto to będzie odwiedzane w następnej pętli.         */
  Queue->removeInside(delete_elem_Queue);
  /* Ustawnianie rozpatrywanego miasta na właśnie usunięte z Queue.        */
  return city_index; /* return city_index do zmiennej current              */
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

