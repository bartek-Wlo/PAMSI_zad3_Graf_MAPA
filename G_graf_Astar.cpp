#include "G_graf.h"

/*_________________________________________________________________________*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\  Astar  /‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾*/
/*                          Jedziemy od city 1. do  city 2.                */
void graf::algorytm_Astar(const string& city_1, const string& city_2) { 
  /* Weryfikacja istnienia podanych miast w pliku                          */
  try {indeks_map.at(city_1); indeks_map.at(city_2);}
  catch (const std::out_of_range& e) {cerr << "NOTFAUND" << endl; return;}
  
  /* Lista Odwiedzonych Miast (zapisuje inteksy tablicy)                   */
  lista<SOM>* LOM = new lista<SOM>;
  /* Lista F(x) = G(x) + H(x) zapisuje obliczone wartości f. + wsk na pol. */
  lista<Fx>* LFX = new lista<Fx>;
  /* Lista stringów do wyświetlenia ścieżki w odwrotnej kolejności.        */
  lista<string>* LS = new lista<string>;
  
  /* Wsk do iterowania w po wszytkich elementach listy LOM                 */
  node<SOM>* display_ptr_LOM = nullptr;
  /* Wsk do aktualnie rozpatrywanego miasta (current city pointer LOM      */
  node<SOM>* CCPL = nullptr;
  /* Wsk do najmniejszego elementu listy LFX                               */
  node<Fx>* LFX_MIN = nullptr;
  /* Wsk na miasto docelowe (city_2)                                       */
  const node<miasto>* DOCELOWE = get_city_wsk_by_id(city_2);

  try
  {
  /* Weryfikacja istnienia podanych miast w pliku                          */
  indeks_map.at(city_1); indeks_map.at(city_2);
  /* Dodawanie indeksu miasta startowego do listy indeksów odwiedzonych m. */
  LOM->addFront( {indeks_map.at(city_1), nullptr, 0} );
  CCPL = LOM->get_head(); /* Rozpatrujemy miasto startowe                  */

  /* ################################# 1 ################################# */
  /* Przeszukujemy aż wsk najnowszego odwiedzonego == wsk miasta docelowego*/
  while(tablica_indeksow_miast[LOM->get_elem(CCPL).index] != DOCELOWE) {
    dla_wszytkich_polaczen_dodaj_F_od_x(LOM, CCPL, DOCELOWE, LFX);
    if (LFX->empty()) throw std::logic_error("List LFX->empty() == TRUE\n           G_graf_Astar.cpp -> algorytm_Astar(const string&,const string&)");

    find_min_Fx_in_LFX(LFX, LFX_MIN);

    /* Jeżeli miasto z index nie jest częścią połączenia wsk_na_polaczenie */
    /* przesuwaj CCPL wsk rozpatrywanego miasta na kolejne w Liście OM.    */
    while( isTop(tablica_indeksow_miast[LOM->get_elem(CCPL).index],
           LFX->get_elem(LFX_MIN).wsk_na_polaczenie) == false ) {
      CCPL = LOM->get_next(CCPL);
    }
    /* INNACZEJ: jeżeli wybrane połączenie LFX_MIN nie jest połączeniem z  */
    /* aktualnie rozpatrywanego miasta to trzeba przesunąć wsk CCPL tak by */
    /* aktualnie rozpatrywane miasto było częścią MIN połączenia F(x).     */

    LOM->addFront({  /* Dodaj do Listy Odwiedzonych Miast: {1, 2, 3}       */
      indeks_map.at( /* Zamień id na indeks tego miasta                    */
        lista_miast->get_elem( /* Zamień wsk na node z miastem na id       */
          opposite(  /* Wzróć wsk na node z miastem c2 za połączeniem      */
            tablica_indeksow_miast[LOM->get_elem(CCPL).index],/* wsk na c1 */
            LFX->get_elem(LFX_MIN).wsk_na_polaczenie /* połaczenie c1-->c2 */
          )
        ).id   /* Dodaj do LOM node z 1: indeks najnowszego miasta         */
      ), CCPL, /* Dodaj do LOM node z 2: wsk na node z poprzednim miastem  */
      LFX->get_elem(LFX_MIN).Gx /* Dodaj do LOM node z 3: dystans G(x)     */
    });
    /* Ustawiamy rozpatrywane miasto na właśnie dodane miasto              */
    CCPL = LOM->get_head();
    /* Usuwamy użyte połączenie, By nie brać go po uwagę find_min_Fx_in_LFX*/
    LFX->removeInside(&LFX_MIN);
    remove_old_connections(LFX, LOM->get_elem(CCPL).index);
    /*Może się zdarzyć, że F(x) dla połączeń z nowego rozpatrywanego miasta*/
    /*nie będą MIN, w tedy trzeba przesunąć wsk w sekscji while(isTop(...))*/
  }
  
  /* ################################# 2 ################################# */
  /* ################### Wyświtlanie ścieżki do miasta ################### */
  /* Wczytywanie do LS ścieżki w odwrotnej kolejności                      */
  display_ptr_LOM = LOM->get_head();
  while (display_ptr_LOM != nullptr) {
    LS->addFront(lista_miast->get_elem(
          tablica_indeksow_miast[LOM->get_elem(display_ptr_LOM).index] ).id);
    display_ptr_LOM = LOM->get_elem(display_ptr_LOM).prev;
  }
  /* Wyświetlanie z LS ściezki w dobrej kolejności                         */
  for (node<string>* i = LS->get_head(); i != nullptr; ) {
    cout << LS->front() << " ";
    i = LS->get_next(i);
    LS->removeFront();
  }/*    Dystans                        Wężły przeanalizowane              */
  cout << LOM->front().dis_Gx << " " <<  LOM->get_nodeNumber() << endl;

  /* ################################# 3 ################################# */
  /* ############################### CATCH ############################### */
  } catch (const std::exception& e) {
    cerr 
      << "NOTFAUND " << endl;
      // << "Exception: " << e.what() << endl
      // << "     type: " << typeid(e).name() << endl
      // << "      fun: G_graf_Astar.cpp -> void algorytm_Astar(string&, string&)"
      // << endl;
  }
  

  delete LOM; 
  delete LFX;
  delete LS;
  LOM = nullptr;
  LFX = nullptr;
  LS =  nullptr;
  return;
}


/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/* Dla wszytkich połączeń danego miasta dodaj do LFX f(x) i wsk połączenia */
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
void graf::dla_wszytkich_polaczen_dodaj_F_od_x(lista<SOM>* LOM, 
const node<SOM>* CCPL, const node<miasto>* DOCELOWE, lista<Fx>* LFX) const {
  /* tablica_indeksow_miast[LOM->front()] zwraca wsk liste połączeń miasta */
  /* ptr_LOM iterujący tab[get_elem()] zwraca wsk na node listy połączeń */
  /* Wskaźnik na node ze wskaźnikiem do node<polaczenia>                   */
  /* Służący do iteracji po wszytkich połączeniach danego miasta           */
  node< node<polaczenia>* >* ptr_LOM = nullptr;
  /* Wskaźnik do node<polaczenia> z jednym połączeniem aktualnego miasta   */
  /* Wsk do Konkretnej Struktury Połączenia. Inicjaliowany ze wsk ptr_LOM  */
  node<polaczenia>* ptr_conect = nullptr;
  /* Wsk na miasto po drugiej stronie połączenia, zwracane z opposite(...) */
  node<miasto>* opposite_city = nullptr; /* Zmienna do upożątkowania kodu  */
  double predkosc;
  static unsigned int curve_distance_predkosc = 130;
  
  ptr_LOM = tablica_sasiedztwa[LOM->get_elem(CCPL).index] -> get_head();
  /* Pętla aż ptr_LOM przejdzie po wszytkich połączeniach danego miasta    */
  while( ptr_LOM != nullptr ) {
    /* Wskaźnik ptr_conect do noda z aktualnie rozpatrywanym połączeniem.  */
    ptr_conect = tablica_sasiedztwa[LOM->get_elem(CCPL).index] -> get_elem(ptr_LOM);
    /* WSK miasta po drugiej stronie połaczenia.                           */
    opposite_city = opposite( /*              opposite(Miasto, połączenie) */
      tablica_indeksow_miast[LOM->get_elem(CCPL).index],
      ptr_conect
    );
    if (time == true) {
      predkosc = get_speed_limit(get_str_polaczenia(ptr_conect).road_type);
        } else predkosc = 1;
    /* hort-circuit evaluation, NIE sprawdzać 2' war. jeżeli 1.==nullptr   */
    /* 1. prev == nullptr Ten warunek spełnia tylko miasto wyjściowe       */
    /* 2. opposite_city != tablica_indeksow_miast[...index] nie dodajemy   */
    /* F(x) dla połączeń wracających do miast z których wyruszyliśmy do    */
    /* aktualnego miasta.                                                  */
    if (IsInLOM(LOM, opposite_city) == false) {
      LFX->addFront({
        /* Dając 0 do LFX do 1' zmiennej "wyłączam" dokładniejszy wzór     */
        LOM->get_elem(CCPL).dis_Gx +               /* G_2(x)               */
        ( get_str_polaczenia(ptr_conect).distance  /* G_1(x)    = G(x)     */
        / predkosc ),    
        LOM->get_elem(CCPL).dis_Gx +                 /*     G_2(x)         */
        ( get_str_polaczenia(ptr_conect).distance /  /*     G_1(x)  = F(x) */
        predkosc ) +
        ( curve_distance(opposite_city, DOCELOWE) /  /*     H(x)           */
        curve_distance_predkosc ) ,
        ptr_conect /* Wskaźnik na aktualne połączenie               WSK    */
      });
    }
    ptr_LOM = tablica_sasiedztwa[LOM->get_elem(CCPL).index] -> get_next(ptr_LOM);
  }
}/*G(x) = G_1(x)               + G_2(x)       {LOM->get_elem(CCPL).dis_Gx} */
/* G(x) = get_str_polaczenia() + odległość aktualnie rozpatrywanego miasta */
/* G(x) -> droga pomiędzy wierzchołkiem początkowym opposite_city          */
/* H(x) -> przewidywana przez heurystykę droga od opposite_city do celowego*/




/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/ Znajdz F(x)min w LFX, i odpowiadające mu połączenie \_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
void graf::find_min_Fx_in_LFX(lista<Fx>* LFX, node<Fx>*& LFX_MIN) const {
  /* Wsk do iterowanie po list LFX                                         */
  node<Fx>* ptr_LFX = nullptr;
  /* Najmniejsza wartość wpisanych F(x)                                    */
  double FxMIN;

  ptr_LFX = LFX->get_head();
  /* Szukanie MIN zaczynamy od ustawnienia wsk LFX_MIN na pierwszy elem    */
  LFX_MIN = ptr_LFX;
  /* FxMIN potencjalnie aktualnie najmniejsza znaleziona wartość           */
  FxMIN = LFX->front().Gx_Hx; /* Zaczynamy od war. pierwszego elementu.    */
  /* Iterujemy po wszytkich elem LFX by znaleść F_{MIN}(x)                 */
  while(ptr_LFX != nullptr) {                   /* Jeżeli znajdziemy war.  */
    if (LFX->get_elem(ptr_LFX).Gx_Hx < FxMIN) { /* miejszą od FxMIN to:    */
      FxMIN = LFX->get_elem(ptr_LFX).Gx_Hx;     /* Ustaw FxMIN na nią oraz */
      LFX_MIN = ptr_LFX;                        /* wsk na MIN też przestaw */
    }
    ptr_LFX = LFX->get_next(ptr_LFX); /* Ustaw wsk. na kolejny elem LFX    */
  }
}




/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/ Czy to miasto zostało już odzwiedzone  i jest w LOM \_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
bool graf::IsInLOM(const lista<SOM>* LOM, const node<miasto>* opposite_city) const {
  node<SOM>* LOM_elem_ptr = LOM->get_head();
  
  while (LOM_elem_ptr != nullptr) {
    /* Zamiast porównywać id, sprawdzam czy Wskaźniki wskazują na to samo */
    if ( tablica_indeksow_miast[LOM->get_elem(LOM_elem_ptr).index]
         == opposite_city ) {
      return true;
    }
    LOM_elem_ptr = LOM->get_next(LOM_elem_ptr);
  }
  return false;
}



/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\ Usuwanie starych połączeń  do właśnie doweidzonego miasta \_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
void graf::remove_old_connections(lista<Fx>* LFX, const unsigned int ind) const {
  node<Fx>* LFX_elem_ptr = LFX->get_head();

  while (LFX_elem_ptr != nullptr) {
    if (isTop(
      tablica_indeksow_miast[ind],
      LFX->get_elem(LFX_elem_ptr).wsk_na_polaczenie
    )) {
      LFX->removeInside(&LFX_elem_ptr);
    }
    if(LFX->empty()) LFX_elem_ptr = nullptr;
    else LFX_elem_ptr = LFX->get_next(LFX_elem_ptr);
  }
}


