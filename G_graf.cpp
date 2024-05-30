#include "G_graf.h"

graf::graf() {
  lista_miast = new lista<miasto>;
  lista_polaczen = new lista<polaczenia>;

  try {load();}
  catch (const std::exception& e) {cerr<<"Exception: "<<e.what()<<endl;} /*[3.]*/
  //load();
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

void graf::load() {
  json data_json;
  ifstream plik("kujawsko-pomorskie.json", std::ios::in); /*[2.]*/
  
  if (!plik) {
    throw std::runtime_error /*[3.]*/
      ("Can not open file.\n           G_graf.cpp -> load()");
    return;
  }
  try { data_json = json::parse(plik); } /*[3.]*/
  catch (json::parse_error& e) {
    cerr << "Parse error: " << e.what() << endl;
    return;
  }
  plik.close();
  
  for (const auto& city : data_json["cities"]) {
    lista_miast->addFront({
      city["name"].get<string>(),
      city["id"].get<string>(),
      city["latitude"].get<double>(),
      city["longitude"].get<double>()
    });
  }

  miasto tmp;
  node<miasto>* miasto_ptr = lista_miast->get_head();
  cout << lista_miast->get_nodeNumber() << endl;
  while(miasto_ptr != nullptr) {
    tmp = lista_miast->get_elem(miasto_ptr);
    miasto_ptr = lista_miast->get_next(miasto_ptr);
    std::cout << " name: "<<tmp.name
      /* << " id:   "<<tmp.id
      << " lati: "<<tmp.latitude
      << " long: "<<tmp.longitude */
      << std::endl;
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

