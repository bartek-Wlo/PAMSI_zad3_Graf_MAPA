#ifndef G_STRUCKT_H
#define G_STRUCKT_H
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
  char road_type;
  unsigned int distance;
};

#endif