#include "osm_contour.hpp"


int main(int argc, char * argv[]) {
  Cell c(argv[1]);
  ToMap *m=new ToMap;
  m->setMap(
  CContourMap *map=new CContourMap;
  map->generate_levels(0,1,3);
  printf("Attempting to contour \n");
  map->contour(m);
  map->dump();
  printf("Consolidating Vectors\n");
  map->consolidate();
  printf("\n\n\n\t\tDumping Contour Map\n");
  map->dump();
  return 0;
}
