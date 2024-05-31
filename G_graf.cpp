#include "G_graf.h"

graf::graf() {
  lista_miast = new lista<miasto>;
  lista_polaczen = new lista<polaczenia>;

  file_name = "kujawsko-pomorskie.json";

}

graf::~graf() {
  delete lista_miast;
  delete lista_polaczen;
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
  node<miasto>* miasto_ptr = lista_miast->get_head();       /* ptr -> head */
  node<polaczenia>* polaczenia_ptr = lista_polaczen->get_head();
  indeks_map.clear(); /* Czyszczenie mapowania tablicy wskaźników */
  
  /* Jeżeli tablice już istnieją, to robimy nowe, usuwając stare */
  if(tablica_indeksow_miast != nullptr) { delete[] tablica_indeksow_miast;}
  if(tablica_sasiedztwa != nullptr) { delete[] tablica_sasiedztwa;}
  
  tablica_indeksow_miast = new node<miasto>*[lista_miast->get_nodeNumber()];
  tablica_sasiedztwa = new node<polaczenia>*[lista_polaczen->get_nodeNumber()];

  for (int i=0; miasto_ptr != nullptr; ++i) {
    tablica_indeksow_miast[i] = miasto_ptr;
    indeks_map[ lista_miast->get_elem(miasto_ptr).id ] = i; /*[5.]*/
    miasto_ptr = lista_miast->get_next(miasto_ptr);
  }

  for (int i=0; polaczenia_ptr != nullptr; ++i) {
    tablica_sasiedztwa[i] = polaczenia_ptr;
    polaczenia_ptr = lista_polaczen->get_next(polaczenia_ptr);
  }

  // cout 
  //   << lista_miast->get_elem( tablica_indeksow_miast[
  //     indeks_map["AleksandrowKujawski"]
  //   ] ).name
  //   << endl;
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
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/* START ##### GRAF ##### START */
void graf::test() {
  miasto tmp;
  node<miasto>* A1 = lista_miast->get_head();
  node<miasto>* A2 = nullptr;
  node<polaczenia>* D1 = lista_polaczen->get_head();

  D1 = lista_polaczen->get_next(D1); D1 = lista_polaczen->get_next(D1);
  A1 = lista_miast->get_next(A1); A1 = lista_miast->get_next(A1);
  A2 = opposite(A1, D1);
  tmp = lista_miast->get_elem(A1);
  std::cout 
    << " city_1: " << left << setw(20) << lista_polaczen->get_elem(D1).city_1
    << " city_2: " << left << setw(20) << lista_polaczen->get_elem(D1).city_2
    << " road_name: " <<left<<setw(20) << lista_polaczen->get_elem(D1).road_name
    << std::endl; /* setw() [4.] */
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
    //   tmp2 = lista_polaczen->get_elem(tablica_sasiedztwa[i]);
    //   std::cout 
    //     << " city_1: " << left << setw(20) << tmp2.city_1
    //     << " city_2: " << left << setw(20) << tmp2.city_2
    //     << " road_name: " << left << setw(6) << tmp2.road_name
    //     << " road_type: " << left << setw(3) << tmp2.road_type
    //     << " distance: " << left << setw(5) << tmp2.distance
    //     << std::endl; /* setw() [4.] */
    // } 