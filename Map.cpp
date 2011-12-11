#include "osm_contour.hpp"
#include <fstream>

Map::Map(const char * fichier_asc, 
           double lat_min, double long_min,
	   double lat_max, double long_max):x_array(std::vector<double>(10)),
                                            y_array(std::vector<double>(10)) 
	   {

  std::ifstream f_in(fichier_asc, std::ifstream::in);
  #ifndef NDEBUG
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
  #ifndef NDEBUG
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

  #ifndef NDEBUG
  std::cout << "Extracting columns " << first_col << "-" << last_col 
            <<  " and rows " << first_row << "-"<< last_row << std::endl; 
  #endif  
  elevation = matrix<double>(new_nrows+1,new_ncols+1);
  for(int i = 0; i < last_row +1; i++)
  {
     double vide;
     for (int j = first_col; j < last_col; j++) {  
         if ( i >= first_row) {
          f_in >> elevation(i-first_row,j-first_col); 
          //#ifndef NDEBUG
          //    std::cout << elevation(i-first_row,j-first_col)  
      	  //        << " " ;
          //#endif
         }
         else {
          f_in >> vide;
         }
     }
     //#ifndef NDEBUG
     //   std::cout <<  std::endl;
     //#endif 
  }
  f_in.close();
  ncols = new_ncols;
  nrows = new_nrows;
  xllcorner = new_xllcorner;
  yllcorner = new_yllcorner;
 
  x_array.resize(ncols,0);
  y_array.resize(nrows,0);
  for(int i = 0; i < ncols; ++i) {
       x_array[i] = i * cellsize + xllcorner;
  }
    
  for(int j = 0; j < nrows; ++j) {
       y_array[j] = (nrows-j) * cellsize + yllcorner;
  }

}   

double Map::value(int y, int x)
{
   assert(y >=0);
   assert(y < nrows);
   assert(x >=0);
   assert(x < ncols);
   return elevation(y,x);
}

SPoint Map::lower_bound()
{
   return SPoint(0,0);
}

SPoint Map::upper_bound()
{
   return SPoint((ncols-1),(nrows-1));
}
