#include "osm_contour.hpp"
#include <iostream>

int main(int argc, char * argv[]) {
  ToMap m(argv[1]);
  CContourMap *map=new CContourMap;
  map->generate_levels(0,1,3);
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
