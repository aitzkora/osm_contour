#include <cstring>
#include "osm_contour.hpp"

Cell::Cell(const char * fichier_asc, 
           double lat_min, double long_min,
	   double lat_max, double long_max) {

  f_in = std::ifstream(fichier_asc, ifstream::in);
  #ifdef DEBUG
  if (!f_in.good()) {
      std::cerr << "File " <<  fichier_asc << "not Found " 
                << std::endl; 
  }
  #endif
  
  f_in >> ncols ;
  f_in >> nrows ;
  f_in >> xllcorner ;
  f_in >> yllcorner ;
  f_in >> cellsize;
  int NODATA ;
  f_in >> NODATA;
#ifdef DEBUG
  std::cout << "cols : "  << ncols << std::endl
           << "rows : "  << nrows << std::endl
           << "xcor : "  << xllcorner << std::endl
           << "ycor : "  << yllcorner << std::endl
           << "cell size : " << cellsize << std::endl
           << "NODATA : "  << nodata << std::endl;
#endif
  int  first_col = static_cast<int>((long_min - xllcorner) / cellsize);
  int  last_col = static_cast<int>((long_max - xllcorner) / cellsize);
  int  first_row = nrows - static_cast<int>((lat_max - yllcorner) / cellsize);
  int  last_row = nrows - static_cast<int>((lat_min - yllcorner) / cellsize);

  int  new_ncols = last_col - first_col
  int  new_nrows = last_row - first_row

  int  new_xllcorner = xllcorner + first_col * cellsize
  int  new_yllcorner = yllcorner + (nrows-last_row) * cellsize

#ifdef DEBUG
  std::cout << "Extracting columns " << first_col << "-" 
            <<  "from rows" << first_row << "-"<< last_row << std::end; 
#endif  
    elevation = matrix_double(last_row-first_row+1,last_col-first_col+1);
    char ligne[5*10000];
    for(int i = 0; i < last_row +1; i++)
    {
       int j = 0;
       f_in.getline(ligne,5*100000);
       char * fin, *deb;
       deb = ligne;
       val = strtod(deb,&fin);
       j = 1;
       while (*fin != '\n' && (*fin != '\0' ) ) {
          deb = fin;
	  elevation[i,j] = strtod(deb, &fin);
          #ifdef DEBUG
                std::cout << elevation[i,j]  << std::endl;
	  #endif
          j++;
       }
       #ifdef DEBUG
          std::cout <<  std::endl;
       #endif 
    }
    f_in.close()
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
       yllcorner = cyllcorner;
       cellsize = c.cellsize;
}

Cell & Cell::operator(const Cell & c) {

       if (this == &c) {
           return *this;
       }
       else { 
           elevation = c.elevation;
           ncols = c.ncols;
           nrows = c.nrows;
           xllcorner = c.xllcorner;
           yllcorner = cyllcorner;
           cellsize = c.cellsize;
       }
}
