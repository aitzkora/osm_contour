#ifndef _TOMAP_HPP_
#define _TOMAP_HPP_

#include "Contours.hpp"
#include "ToMap.hpp"
#include <boost/numeric/ublas/matrix.hpp>

using namespace boost::numeric::ublas;

class ToMap : public CRaster{
public:   
   matrix<double> elevation;
   double cellsize;
   int ncols;
   int nrows;
   int xllcorner;
   int yllcorner;

   ToMap(const char * fichier_asc="srtm_38_03.asc", 
        double lat_min=48.2299, 
        double long_min=6.8489,
	double lat_max=48.3343,
        double long_max=7.0514);
        
   ~ToMap() {}

   double value(double y, double x);
   
   SPoint upper_bound();
   SPoint lower_bound();
};
#endif
