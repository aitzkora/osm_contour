#include "osm_contour.hpp"
#include <fstream>

Cell::Cell(const char * fichier_asc, 
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
       int j = 0;
          if  (
	  f_in >> elevation(i,j) 
          #ifdef DEBUG
                std::cout << elevation(i,j)  << std::endl;
	  #endif
          j++;
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

Cell::Cell(const Cell & c) {

       elevation = c.elevation;
       ncols = c.ncols;
       nrows = c.nrows;
       xllcorner = c.xllcorner;
       yllcorner = c.yllcorner;
       cellsize = c.cellsize;
}

Cell & Cell::operator=(const Cell & c) {

       if (this == &c) {
           return *this;
       }
       else { 
           elevation = c.elevation;
           ncols = c.ncols;
           nrows = c.nrows;
           xllcorner = c.xllcorner;
           yllcorner = c.yllcorner;
           cellsize = c.cellsize;
       }
}
