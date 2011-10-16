#include "osm_contour.hpp"
#include <fstream>

ToMap::ToMap(const char * fichier_asc, 
           double lat_min, double long_min,
	   double lat_max, double long_max) {

  std::ifstream f_in(fichier_asc, std::ifstream::in);
  #ifdef DEBUG
  if (!f_in.good()) {
      std::cerr << "File " <<  fichier_asc << "not Found " 
                << std::endl; 
  }
  #endif
  std::string chaine;
  f_in >> chaine >> ncols 
       >> chaine >> nrows
       >> chaine >> xllcorner
       >> chaine >> yllcorner
       >> chaine >> cellsize;
  int NODATA ;
  f_in >> chaine >> NODATA;
  #ifdef DEBUG
  std::cout << "cols : "  << ncols << std::endl
           << "rows : "  << nrows << std::endl
           << "xcor : "  << xllcorner << std::endl
           << "ycor : "  << yllcorner << std::endl
           << "cell size : " << cellsize << std::endl
           << "NODATA : "  << NODATA << std::endl;
  #endif
  int  first_col = static_cast<int>((long_min - xllcorner) / cellsize);
  int  last_col = static_cast<int>((long_max - xllcorner) / cellsize);
  int  first_row = nrows - static_cast<int>((lat_max - yllcorner) / cellsize);
  int  last_row = nrows - static_cast<int>((lat_min - yllcorner) / cellsize);

  int  new_ncols = last_col - first_col;
  int  new_nrows = last_row - first_row;

  int  new_xllcorner = xllcorner + first_col * cellsize;
  int  new_yllcorner = yllcorner + (nrows-last_row) * cellsize;

  #ifdef DEBUG
  std::cout << "Extracting columns " << first_col << "-" 
            <<  "from rows" << first_row << "-"<< last_row << std::endl; 
  #endif  
    elevation = matrix<double>(last_row-first_row+1,last_col-first_col+1);
    for(int i = 0; i < last_row +1; i++)
    {
       double vide;
       for (int j = first_col; j < last_col; j++) {  
           if ( i >= first_row) {
	    f_in >> elevation(i-first_row,j-first_col); 
            #ifdef DEBUG
                std::cout << elevation(i-first_row,j-first_col)  
		          << " " ;
	    #endif
           }
           else {
	    f_in >> vide;
           }
       }
       #ifdef DEBUG
          std::cout <<  std::endl;
       #endif 
    }
    f_in.close();
    ncols = new_ncols;
    nrows = new_nrows;
    xllcorner = new_xllcorner;
    yllcorner = new_yllcorner;

}   

double ToMap::value(double x, double y)
{
   int i = static_cast<int>((x-xllcorner)/cellsize);
   int j = static_cast<int>((y-yllcorner)/cellsize);
   assert(i >=0);
   assert(i <= nrows);
   assert(j >=0);
   assert(j <= ncols);
   return elevation(i,j);
}

SPoint ToMap::lower_bound()
{
   return SPoint(xllcorner,yllcorner);
}

SPoint ToMap::upper_bound()
{
   return SPoint((nrows-1)*cellsize+xllcorner,
                 (ncols-1)*cellsize+yllcorner);
}
