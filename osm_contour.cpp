#include "osm_contour.hpp"
#include <iostream>

int main(int argc, char * argv[]) {
  ToMap m(argv[1]);
  CContourMap *map=new CContourMap;
  int min_ele = -500;
  int max_ele = 9000;
  int step = 25;
  int num = (max_ele-min_ele)/step;
  
  map->generate_levels(min_ele,max_ele,num);
  std::cout << "Attempting to contour" << std::endl;
  map->contour(&m);
  
  std::ofstream fp1("contours.osm");
  map->dump(fp1);
  fp1.close();
  
  std::cout << "Consolidating Vectors" << std::endl;
  map->consolidate();
  
  std::cout << std::endl <<  "Dumping Contour Map" << std::endl;
  std::ofstream fp2("contours2.osm");
  map->dump(fp2);
  fp2.close();
  return 0;
}
