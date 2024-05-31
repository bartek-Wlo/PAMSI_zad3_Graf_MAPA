#include "G_graf.h"

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/ KONSTRUKTOR \_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
graf::graf() {
  size_TAB_sasiedztwa = 0;
  lista_miast = new lista<miasto>;
  lista_polaczen = new lista<polaczenia>;

  file_name = "lubuskie.json";

}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/ DESTRUKTOR  \_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
graf::~graf() {
  delete lista_miast;
  delete lista_polaczen;

  /* Node'y wskazywane przez liste wskaźniów wazywanej przez tablice wsk.  */
  /* Czyli lista_polaczen zostały już usunięte.         tablica_sasiedztwa */
  for(unsigned int i = 0; i < size_TAB_sasiedztwa; ++i) {
    delete tablica_sasiedztwa[i];
  } /* Więc można usunąć listy "bez" usuwania ich zawartości.              */
  
  delete[] tablica_sasiedztwa;
  delete[] tablica_indeksow_miast;
  lista_miast = nullptr;
  lista_polaczen = nullptr;
  tablica_sasiedztwa = nullptr;
  tablica_indeksow_miast = nullptr;
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\   LOAD  /‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
void graf::load() {
  open();
  
  for (const auto& city : data_json["cities"]) {
    lista_miast->addFront({
      city["name"].get<string>(),
      city["id"].get<string>(),
      city["latitude"].get<double>(),
      city["longitude"].get<double>()
    });
  }

  for (const auto& road : data_json["connections"]) {
    lista_polaczen->addFront({
      road["city_1"].get<string>(),
      road["city_2"].get<string>(),
      road["road_name"].get<string>(),
      road["road_type"].get<string>(),
      road["distance"].get<double>()
    });
  }
}

/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_  OPEN & CLOSE _/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
void graf::open() {
  ifstream plik(file_name, std::ios::in); /*[2.]*/
  
  if ( ! plik) {
    throw std::runtime_error ("Can not open file: \"" + file_name 
             + "\"\n           G_graf.cpp -> open()");
  }  /*[3.]*/
  
  data_json = json::parse(plik); /*[1.]*/
  /* json::parse() może throw parse error. */
  plik.close();
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/  add ARRRAY  ptr's  \_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/

void graf::add_ARRRAY_ptrs() {
  /* ################ Inicjalizacja  zmienej i wskaźników ################ */
  node<miasto>* miasto_ptr = lista_miast->get_head();       /* ptr -> head */
  node<polaczenia>* polaczenia_ptr = lista_polaczen->get_head();
  unsigned int ind;

  /* ############## Warunkowa reinicjalizacja tablic i mapy ############## */
  /*                           1. Czyszczenie mapowania tablicy wskaźników */
  /*                     2. USUWA tablice wskaźników na node'y typu miasto */
  /*         3. Usuwanie tab. wsk. na liste wsk. na node'y typu połączenia */
  /*         3.1.  USUWA poszczególne listy wsk. na node'y typu połączenia */
  /*                               3.2.  USUWA tablice wskaźników na listy */
  indeks_map.clear(); /*                                                1. */
  if(tablica_indeksow_miast != nullptr) 
    { delete[] tablica_indeksow_miast;} /*                              2. */
  if(tablica_sasiedztwa != nullptr) { /*                                3. */
    for(unsigned int i = 0; i < size_TAB_sasiedztwa; ++i)  
      { delete tablica_sasiedztwa[i]; } /*                            3.1. */
    delete[] tablica_sasiedztwa; /*                                   3.2. */
  }

  /* ################# Wczytuje aktualną wielkość tablic ################# */
  size_TAB_sasiedztwa = lista_miast->get_nodeNumber();

  /* ##################### Inicjalizuje nowe tablice ##################### */
  /*                    1. TWORZY tablice wskaźników na node'y typu miasto */
  /*                            2. TWORZY tablice wskaźników na listy wsk. */
  /*                  3. TWORZY listy wskaźników na node'y typu połączenia */
  tablica_indeksow_miast = new node<miasto>* [size_TAB_sasiedztwa]; /*  1. */
  tablica_sasiedztwa = new lista<node<polaczenia>*>* [size_TAB_sasiedztwa];
  for(unsigned int i = 0; i < size_TAB_sasiedztwa; ++i)            /* ^ 2. */
    { tablica_sasiedztwa[i] = new lista<node<polaczenia>*>; } /*        3. */

  /* ############# WYPEŁNIA  tab. wsk. na node'y typu miasto ############# */
  /*            1. PRZYPISUJE kolejne wskaźniki z lista_miast do tab. wsk. */
  /*                                         2. MAPUJE nazwy string na int */
  /*                         3. PRZESUWA wsk. na następny elem lista_miast */
  for (int i=0; miasto_ptr != nullptr; ++i) {
    tablica_indeksow_miast[i] = miasto_ptr; /*                          1. */
    indeks_map[ lista_miast->get_elem(miasto_ptr).id ] = i; /*[5.]      2. */
    miasto_ptr = lista_miast->get_next(miasto_ptr); /*                  3. */
  } /* Wskaźnik miasto_ptr idzie po każdym elemencie listy lista_miast.    */

  /* ######## WYPEŁNIA tab. wsk. na list wsk. na node typu miasto ######## */
  /*              1. Iteracja po wszytkich elemętach listy: lista_polaczen */
  /*                       2. city ID -> indeks tablicy (dzięki mapowaniu) */
  /*                          3. Dodawanie do listy wsk. w tab nowego wsk. */
  /*                      4. PRZESUWA wsk. na następny elem lista_polaczen */
  while (polaczenia_ptr != nullptr) { /*                                1. */
    ind = indeks_map[ lista_polaczen->get_elem(polaczenia_ptr).city_1 ];// 2.
    tablica_sasiedztwa[ind]->addFront(polaczenia_ptr); /*               3. */

    ind = indeks_map[ lista_polaczen->get_elem(polaczenia_ptr).city_2 ];// 2.
    tablica_sasiedztwa[ind]->addFront(polaczenia_ptr); /*               3. */
    
    polaczenia_ptr = lista_polaczen->get_next(polaczenia_ptr); /*       4. */
  } /* Wsk. polaczenia_ptr idzie po każdym elemencie listy lista_polaczen. */
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/   GETTERS   \_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/

node<miasto>* graf::get_city_wsk_by_id(string id) const {
  try {
    return tablica_indeksow_miast[indeks_map.at(id)];
  } catch (const std::exception& e) {
    cerr 
      << "Exception: " << e.what() << endl
      << "     type: "<< typeid(e).name() << endl
      << "      fun: G_graf.cpp -> node<miasto>* get_city_wsk_by_id(string)"
      << endl;
    return nullptr;
  }
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
node<polaczenia>* graf::get_road(node<miasto>* c1, node<miasto>* c2) const {
  if (c1 == nullptr || c2 == nullptr) {
    throw std::invalid_argument ("node<miasto>* == nullptr\n           G_graf.cpp -> get_road(node<miasto>*,node<miasto>*)");
  }
  if (lista_miast->empty() || lista_polaczen->empty()) {
    throw std::invalid_argument ("List.empty() == TRUE\n           G_graf.cpp -> get_road(node<miasto>*,node<miasto>*)");
  }

  int inde = indeks_map.at(lista_miast->get_elem(c1).id);
  polaczenia str_pol;
  node<node<polaczenia>*>* ptr = tablica_sasiedztwa[inde] -> get_head();
  
  while (ptr != nullptr) {
    str_pol = lista_polaczen->get_elem(tablica_sasiedztwa[inde]->get_elem(ptr));
    if (str_pol.city_1 == lista_miast->get_elem(c1).id &&
        str_pol.city_2 == lista_miast->get_elem(c2).id) {
      return tablica_sasiedztwa[inde]->get_elem(ptr);
    }
    else if (str_pol.city_1 == lista_miast->get_elem(c2).id &&
             str_pol.city_2 == lista_miast->get_elem(c1).id) {
      return tablica_sasiedztwa[inde]->get_elem(ptr);
    }
    ptr = tablica_sasiedztwa[inde] -> get_next(ptr);
  }
  throw std::logic_error  ("There is no road connecting both cities\n           G_graf.cpp -> get_road(node<miasto>*,node<miasto>*)");
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\ curve  distance /‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*           https://en.wikipedia.org/wiki/Geographical_distance           */
/*           https://www.calculator.net/distance-calculator.html           */
/*                           Haversine  formula:                           */
/* d = 2R * asin{ √[ sin²((φ1-φ2)/2) + cosφ1 * cosφ2 * sin²((λ1-λ2)/2) ] } */

double graf::curve_distance(node<miasto>* c1, node<miasto>* c2) {
  static const int R2 = 2*6371; /* [km] */
  double lat1 = (lista_miast->get_elem(c1).latitude ) * (M_PI / 180.0);
  double lat2 = (lista_miast->get_elem(c2).latitude ) * (M_PI / 180.0);
  double lon1 = (lista_miast->get_elem(c1).longitude) * (M_PI / 180.0);
  double lon2 = (lista_miast->get_elem(c2).longitude) * (M_PI / 180.0);
  return R2 * asin(
    sqrt(
      pow( sin((lat1-lat2)/2) , 2 ) 
      + cos(lat1) 
      * cos(lat2) 
      * pow( sin((lon1-lon2)/2) , 2 )
    )
  );
  
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/ endVertices \_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/

void graf::endVertices(node<miasto>** c1, 
                       node<miasto>** c2, node<polaczenia>* road) {
  if (road == nullptr) {
    throw std::invalid_argument("node<polaczenia>* road == nullptr \n           G_graf.cpp -> endVertices(node<miasto>** c1, node<miasto>** c2, node<polaczenia>* road)");
  } 
  
  *c1 = tablica_indeksow_miast [
    indeks_map[ lista_polaczen->get_elem(road).city_1 ] 
  ];
  *c2 = tablica_indeksow_miast [
    indeks_map[ lista_polaczen->get_elem(road).city_2 ] 
  ];
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/  opposite   \_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/

node<miasto>* graf::opposite(node<miasto>* c1, node<polaczenia>* road) const {
  if (road == nullptr) {
    throw std::invalid_argument("node<polaczenia>* road == nullptr \n           G_graf.cpp -> opposite(node<miasto>* c1, node<polaczenia>* road)");
  }
  
  if (c1 == nullptr) {
    throw std::invalid_argument("node<miasto>* c1 == nullptr \n           G_graf.cpp -> opposite(node<miasto>* c1, node<polaczenia>* road)");
  }
  
  if (lista_polaczen->get_elem(road).city_1 == lista_miast->get_elem(c1).id) {
    return tablica_indeksow_miast [
      indeks_map.at( lista_polaczen->get_elem(road).city_2 ) 
    ];
  }
  if (lista_polaczen->get_elem(road).city_2 == lista_miast->get_elem(c1).id) {
    return tablica_indeksow_miast [
      indeks_map.at( lista_polaczen->get_elem(road).city_1 ) 
    ];
  } else {
    throw std::invalid_argument(
      "Road: \"" + lista_polaczen->get_elem(road).road_name + 
      "\" is not connected to city: \"" + lista_miast->get_elem(c1).name +
      "\"\n           G_graf.cpp -> opposite(node<miasto>*, node<polaczenia>*)"
    );
  }
  
};

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/ areAdjacent \_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
bool graf::areAdjacent(node<miasto>* c1, node<miasto>* c2) {
  int inde = indeks_map [ lista_miast->get_elem(c1).id ];
  polaczenia str_pol;
  node<node<polaczenia>*>* ptr = tablica_sasiedztwa [inde] -> get_head();
  while (ptr != nullptr) {  
    str_pol = lista_polaczen->get_elem(tablica_sasiedztwa[inde]->get_elem(ptr));
    if (str_pol.city_1 == lista_miast->get_elem(c1).id &&
        str_pol.city_2 == lista_miast->get_elem(c2).id) { return true; }
    else if (str_pol.city_1 == lista_miast->get_elem(c2).id &&
        str_pol.city_2 == lista_miast->get_elem(c1).id) { return true; }
    ptr = tablica_sasiedztwa[inde] -> get_next(ptr);
  }
  return false;
}


/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/

void graf::test() {
  miasto tmp;
  node<miasto>* A1 = get_city_wsk_by_id("Bogatynia");
  node<miasto>* A2 = get_city_wsk_by_id("Sejny");
  node<polaczenia>* D1 = nullptr;

  cout << "curve_distance: " << curve_distance(A1, A2) << endl;
  if (areAdjacent(A1, A2)) {
    cout << "A1 and A2 are adjacent" << endl;
    D1 = get_road(A1, A2);
  } else {
    cout << "A1 and A2 are NOT adjacent, error incoming!" << endl;
    D1 = get_road(A1, A2);
  }
  std::cout 
    << " city_1: " << left << setw(20) << lista_polaczen->get_elem(D1).city_1
    << " city_2: " << left << setw(20) << lista_polaczen->get_elem(D1).city_2
    << " road_name: " <<left<<setw(20) << lista_polaczen->get_elem(D1).road_name
    << std::endl << "                             "
    << " road_type: " <<left<<setw(5) << lista_polaczen->get_elem(D1).road_type
    << " distance: " <<left<<setw(10) << lista_polaczen->get_elem(D1).distance
    << std::endl; /* setw() [4.] */
  
  tmp = lista_miast->get_elem(A1);
  std::cout 
    << " id: " << left << setw(20) << tmp.id
    << " lati: " << left << setw(10) << tmp.latitude
    << " long: " << left << setw(10) << tmp.longitude
    << " name: " << left << setw(20) << tmp.name
    << std::endl; /* setw() [4.] */
  
  tmp = lista_miast->get_elem(A2);
  std::cout 
    << " id: " << left << setw(20) << tmp.id
    << " lati: " << left << setw(10) << tmp.latitude
    << " long: " << left << setw(10) << tmp.longitude
    << " name: " << left << setw(20) << tmp.name
    << std::endl; /* setw() [4.] */
}


/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\  DEBUG display  /‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/* DEBUG display list - miasto */
    // miasto tmp;
    // node<miasto>* miasto_ptr = lista_miast->get_head();
    // cout << lista_miast->get_nodeNumber() << endl;
    // while(miasto_ptr != nullptr) {
    //   tmp = lista_miast->get_elem(miasto_ptr);
    //   miasto_ptr = lista_miast->get_next(miasto_ptr);
    //   std::cout 
    //     << " id: " << left << setw(20) << tmp.id
    //     << " lati: " << left << setw(10) << tmp.latitude
    //     << " long: " << left << setw(10) << tmp.longitude
    //     << " name: " << left << setw(20) << tmp.name
    //     << std::endl; /* setw() [4.] */
    // }

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/* DEBUG display array - miasto */
    // miasto tmp;
    // for(int i = 0; i < lista_miast->get_nodeNumber(); ++i) {
    //   tmp = lista_miast->get_elem(tablica_indeksow_miast[i]);
    //   std::cout 
    //     << " id: " << left << setw(20) << tmp.id
    //     << " lati: " << left << setw(10) << tmp.latitude
    //     << " long: " << left << setw(10) << tmp.longitude
    //     << " name: " << left << setw(20) << tmp.name
    //     << std::endl; /* setw() [4.] */
    // }

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/* DEBUG display list - polaczenia */
    // polaczenia tmp2;
    // node<polaczenia>* polaczenia_ptr = lista_polaczen->get_head();
    // cout << lista_polaczen->get_nodeNumber() << endl;
    // while(polaczenia_ptr != nullptr) {
    //   tmp2 = lista_polaczen->get_elem(polaczenia_ptr);
    //   polaczenia_ptr = lista_polaczen->get_next(polaczenia_ptr);
    //   std::cout 
    //     << " city_1: " << left << setw(20) << tmp2.city_1
    //     << " city_2: " << left << setw(20) << tmp2.city_2
    //     << " road_name: " << left << setw(6) << tmp2.road_name
    //     << " road_type: " << left << setw(3) << tmp2.road_type
    //     << " distance: " << left << setw(5) << tmp2.distance
    //     << std::endl; /* setw() [4.] */
    // }

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/* DEBUG display array - polaczenia */
    // polaczenia tmp2;
    // for(int i = 0; i < lista_polaczen->get_nodeNumber(); ++i) {
    //   tmp2 = lista_polaczen->get_elem(tablica_polaczen[i]);
    //   std::cout 
    //     << " city_1: " << left << setw(20) << tmp2.city_1
    //     << " city_2: " << left << setw(20) << tmp2.city_2
    //     << " road_name: " << left << setw(6) << tmp2.road_name
    //     << " road_type: " << left << setw(3) << tmp2.road_type
    //     << " distance: " << left << setw(5) << tmp2.distance
    //     << std::endl; /* setw() [4.] */
    // } 

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/* DEBUG display array of poiters to list of pointers to list               
(tablica_sasiedztwa) Można wstawić na koniec funkcji -> add_ARRRAY_ptrs()  */
    // cout << "miast "<< lista_miast->get_nodeNumber() << " polaczen " << lista_polaczen->get_nodeNumber() << endl;
    // polaczenia tmp2;
    // node<node<polaczenia>*>* tab_list_ptr = nullptr;
    // for (int i=0; i < lista_miast->get_nodeNumber(); ++i) {
    //   tab_list_ptr = tablica_sasiedztwa[i]->get_head();
    //   std::cout << i << ".   " 
    //     << lista_miast->get_elem( tablica_indeksow_miast[i] ).name 
    //     << endl;
    
    //   while(tab_list_ptr != nullptr) {
    //     tmp2 = lista_polaczen->get_elem(
    //       tablica_sasiedztwa[i]->get_elem(tab_list_ptr)
    //     );
    //     tab_list_ptr = tablica_sasiedztwa[i]->get_next(tab_list_ptr);
    //     std::cout 
    //       << " city_1: " << left << setw(20) << tmp2.city_1
    //       << " city_2: " << left << setw(20) << tmp2.city_2
    //       << " road_name: " << left << setw(6) << tmp2.road_name
    //       << " road_type: " << left << setw(3) << tmp2.road_type
    //       << " distance: " << left << setw(5) << tmp2.distance
    //       << std::endl; /* setw() [4.] */
    //   }
    // }