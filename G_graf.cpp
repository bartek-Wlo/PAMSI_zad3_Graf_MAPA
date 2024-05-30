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

  /* Jeżeli tablice już istnieją, to robimy nowe, usuwając stare */
  if(tablica_indeksow_miast != nullptr) { delete[] tablica_indeksow_miast;}
  if(tablica_sasiedztwa != nullptr) { delete[] tablica_sasiedztwa;}
  
  tablica_indeksow_miast = new node<miasto>*[lista_miast->get_nodeNumber()];
  tablica_sasiedztwa = new node<polaczenia>*[lista_polaczen->get_nodeNumber()];

  for (int i=0; miasto_ptr != nullptr; ++i) {
    tablica_indeksow_miast[i] = miasto_ptr;
    miasto_ptr = lista_miast->get_next(miasto_ptr);
  }

  for (int i=0; polaczenia_ptr != nullptr; ++i) {
    tablica_sasiedztwa[i] = polaczenia_ptr;
    polaczenia_ptr = lista_polaczen->get_next(polaczenia_ptr);
  }
}

/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/*\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\_/‾\*/
/* START ##### GRAF ##### START */
void test()
{
lista<miasto>* lista_miast = new lista<miasto>;
lista<polaczenia>* lista_polaczen = new lista<polaczenia>;
node<miasto>** tablica_indeksow_miast = nullptr;
node<polaczenia>** tablica_sasiedztwa = nullptr;

node<miasto>* miasto_ptr = nullptr;

miasto tmp;
unsigned int inkes = 0;

lista_miast->addFront({"Wrocław", "Wroclaw", 10, 10});
lista_miast->addFront({"W1", "1", 100, 100});
lista_miast->addFront({"W2", "2", 200, 200});
lista_miast->addFront({"W3", "3", 300, 300});
lista_miast->addFront({"W4", "4", 400, 400});
lista_miast->addFront({"W5", "5", 500, 500});
lista_miast->addFront({"W6", "6", 600, 600});
lista_miast->addFront({"W7", "7", 700, 700});
lista_miast->addFront({"W8", "8", 800, 800});

tablica_indeksow_miast= new node<miasto>*[lista_miast->get_nodeNumber()];
tablica_sasiedztwa= new node<polaczenia>*[lista_polaczen->get_nodeNumber()];

miasto_ptr = lista_miast->get_head();
while(miasto_ptr != nullptr) {
  tmp = lista_miast->get_elem(miasto_ptr);
  tablica_indeksow_miast[inkes] = miasto_ptr;
  miasto_ptr = lista_miast->get_next(miasto_ptr);
  std::cout << " name: "<<tmp.name
    << " id:   "<<tmp.id
    << " lati: "<<tmp.latitude
    << " long: "<<tmp.longitude
    << std::endl;
  ++inkes;
}

cout <<"\nTablica"<< endl;
for(int i = 0; i < lista_miast->get_nodeNumber(); ++i) {
  tmp = lista_miast->get_elem(tablica_indeksow_miast[i]);
  std::cout << " name: "<<tmp.name
    << " id:   "<<tmp.id
    << " lati: "<<tmp.latitude
    << " long: "<<tmp.longitude
    << std::endl;
}



/*...*/

delete lista_miast;
delete lista_polaczen;
delete[] tablica_sasiedztwa;
delete[] tablica_indeksow_miast;
lista_miast = nullptr;
lista_polaczen = nullptr;
tablica_sasiedztwa = nullptr;
tablica_indeksow_miast = nullptr;

/* END ##### GRAF ##### END */
graf GRAF;
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