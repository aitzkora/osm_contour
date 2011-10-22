#include "osm_contour.hpp"
#include <iostream>

int main(int argc, char * argv[]) {
  Map m(argv[1],48.2998,6.935,48.312,6.950);
  ContourMap *map=new ContourMap;
  int min_ele = 300;
  int max_ele = 700;
  int step = 25;
  int num = (max_ele-min_ele)/step;
  
  map->generate_levels(min_ele,max_ele,num);
  std::cout << "Attempting to contour" << std::endl;
  map->contour(&m);
  std::ofstream fp1("contours.txt");
  map->dump(fp1);
  fp1.close();
  
  std::cout << "Consolidating Vectors" << std::endl;
  map->consolidate();
  
  std::ofstream osm("contours.osm");
  map->dump_osm(osm);
  osm.close();
  std::cout << std::endl <<  "Dumping Contour Map" << std::endl;
  std::ofstream fp2("contours2.txt");
  map->dump(fp2);
  fp2.close();
  return 0;
}
