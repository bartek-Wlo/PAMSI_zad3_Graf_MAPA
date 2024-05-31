#ifndef G_STRUCKT_H
#define G_STRUCKT_H
#include "G_lista.h"
#include <string>

struct miasto {
  std::string name;
  std::string id;
  double latitude;
  double longitude;
};

struct polaczenia{
  std::string city_1;
  std::string city_2;
  std::string road_name;
  std::string road_type;
  double distance;
  node<polaczenia>* next_pol = nullptr;
};

#endif