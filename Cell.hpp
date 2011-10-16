#ifndef _CELL_HPP_
#define _CELL_HPP_

#include <boost/numeric/ublas/matrix.hpp>
#include <iostream>

using namespace boost::numeric::ublas;
class Cell {
public:   
   matrix<double> elevation;
   double cellsize;
   int ncols;
   int nrows;
   int xllcorner;
   int yllcorner;


   Cell(const char * fichier_asc="srtm_38_03.asc", 
        double lat_min=48.2299, 
        double long_min=6.8489,
	double lat_max=48.3343,
        double long_max=7.0514);
        
   ~Cell() {}

   Cell(const Cell & c);

   Cell & operator=(const Cell & c);
 

};
#endif
