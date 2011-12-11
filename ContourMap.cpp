#include <algorithm>
#include <iomanip>
#include "ContourMap.hpp"

#define xsect(p1,p2) (h[p2]*xh[p1]-h[p1]*xh[p2])/(h[p2]-h[p1])
#define ysect(p1,p2) (h[p2]*yh[p1]-h[p1]*yh[p2])/(h[p2]-h[p1])

int ContourMap::contour(Map *r)
{
/*
   this routine is coppied almost verbatim form Nicholas Yue's C++ implememtation 
   of Paul bourkes CONREC routine. for deatails on the theory and implementation
   of this routine visit http://local.wasp.uwa.edu.au/~pbourke/papers/conrec/
      
   quick summary of the changes made
   - all the data passed in to the function is in a CRaster object and accessed
     thru a method (double value(double x, double y)). This class can be subclassed
     to calculate values based on a formula or retrieve values from a table.
   - as retrieval of values is now in the class supplied by the user we don't care
     if the data is regularly spaced or not, we assume that the class takes care of
     these details.
   - upper and lower bounds are similarly provided but the upper_bound and lower_bound
     methods in the class.
   - it is not assumed that this data is being immediatly output, rather it is all stored
     in a structure for later processing/display.
   
   yet to be done
   - realisticly we should replace the i and j indices with iterators, decouple this
     function from any sort of assumption entirely. this would require being able to 
     retrieve x and y values from the iterator as well as the value. something like 
     returning a pointer to a structure that contains the x and y values of the given 
     point as well as the value. As this is not really required in my application it 
     may or may not be done.
   
//=============================================================================
//
//     CONREC is a contouring subroutine for rectangularily spaced data.
//
//     It emits calls to a line drawing subroutine supplied by the user
//     which draws a contour map corresponding to real*4data on a randomly
//     spaced rectangular grid. The coordinates emitted are in the same
//     units given in the x() and y() arrays.
//
//     Any number of contour levels may be specified but they must be
//     in order of increasing value.
//
//     As this code is ported from FORTRAN-77, please be very careful of the
//     various indices like ilb,iub,jlb and jub, remeber that C/C++ indices
//     starts from zero (0)
//
//===========================================================================*/

   int m1,m2,m3,case_value;
   double dmin,dmax,x1,x2,y1,y2;
   register int i,j,k,m;
   double h[5];
   int sh[5];
   double xh[5],yh[5];
  //===========================================================================
  // The indexing of im and jm should be noted as it has to start from zero
  // unlike the fortran counter part
  //===========================================================================
   int im[4] = {0,1,1,0},jm[4]={0,0,1,1};
  //===========================================================================
  // Note that castab is arranged differently from the FORTRAN code because
  // Fortran and C/C++ arrays are transposed of each other, in this case
  // it is more tricky as castab is in 3 dimension
  //===========================================================================
   int castab[3][3][3] =
   {
      {
         {0,0,8},{0,2,5},{7,6,9}
      },
      {
         {0,3,4},{1,3,1},{4,3,0}
      },
      {
         {9,6,7},{5,2,0},{8,0,0}
      }
   };
   for (j=((int)r->upper_bound().x-1);j>=(int)r->lower_bound().x;j--) {
      for (i=(int)r->lower_bound().y;i<=(int)r->upper_bound().y-1;i++) {
         double temp1,temp2;
         temp1 = std::min(r->value(i,j),r->value(i,j+1));
         temp2 = std::min(r->value(i+1,j),r->value(i+1,j+1));
         dmin = std::min(temp1,temp2);
         temp1 = std::max(r->value(i,j),r->value(i,j+1));
         temp2 = std::max(r->value(i+1,j),r->value(i+1,j+1));
         dmax = std::max(temp1,temp2);
         if (dmax>=levels[0]&&dmin<=levels[n_levels-1]) {
            for (k=0;k<n_levels;k++) {
               if (levels[k]>=dmin&&levels[k]<=dmax) {
                  for (m=4;m>=0;m--) {
                     if (m>0) {
		//=============================================================
		// The indexing of im and jm should be noted as it has to
		// start from zero
		//=============================================================
                        h[m] = r->value(i+im[m-1],j+jm[m-1])-levels[k];
                        xh[m] = r->y_array[i+im[m-1]];
                        yh[m] = r->x_array[j+jm[m-1]];
                     } else {
                        h[0] = 0.25*(h[1]+h[2]+h[3]+h[4]);
                        xh[0]=0.5*(r->y_array[i]+r->y_array[i+1]);
                        yh[0]=0.5*(r->x_array[j]+r->x_array[j+1]);
                     }
                     if (h[m]>0.0) {
                        sh[m] = 1;
                     } else if (h[m]<0.0) {
                        sh[m] = -1;
                     } else
                        sh[m] = 0;
                  }
	    //=================================================================
                  //
	    // Note: at this stage the relative heights of the corners and the
	    // centre are in the h array, and the corresponding coordinates are
	    // in the xh and yh arrays. The centre of the box is indexed by 0
	    // and the 4 corners by 1 to 4 as shown below.
	    // Each triangle is then indexed by the parameter m, and the 3
	    // vertices of each triangle are indexed by parameters m1,m2,and
	    // m3.
	    // It is assumed that the centre of the box is always vertex 2
	    // though this isimportant only when all 3 vertices lie exactly on
	    // the same contour level, in which case only the side of the box
	    // is drawn.
                  //
                  //
	    //      vertex 4 +-------------------+ vertex 3
	    //               | \               / |
	    //               |   \    m-3    /   |
	    //               |     \       /     |
	    //               |       \   /       |
	    //               |  m=2    X   m=2   |       the centre is vertex 0
	    //               |       /   \       |
	    //               |     /       \     |
	    //               |   /    m=1    \   |
	    //               | /               \ |
	    //      vertex 1 +-------------------+ vertex 2
                  //
                  //
                  //
	    //               Scan each triangle in the box
                  //
	    //=================================================================
                  for (m=1;m<=4;m++) {
                     m1 = m;
                     m2 = 0;
                     if (m!=4)
                        m3 = m+1;
                     else
                        m3 = 1;
                     case_value = castab[sh[m1]+1][sh[m2]+1][sh[m3]+1];
                     if (case_value!=0) {
                        switch (case_value) {
		  //===========================================================
		  //     Case 1 - Line between vertices 1 and 2
		  //===========================================================
                           case 1:
                              x1=xh[m1];
                              y1=yh[m1];
                              x2=xh[m2];
                              y2=yh[m2];
                              break;
		  //===========================================================
		  //     Case 2 - Line between vertices 2 and 3
		  //===========================================================
                           case 2:
                              x1=xh[m2];
                              y1=yh[m2];
                              x2=xh[m3];
                              y2=yh[m3];
                              break;
		  //===========================================================
		  //     Case 3 - Line between vertices 3 and 1
		  //===========================================================
                           case 3:
                              x1=xh[m3];
                              y1=yh[m3];
                              x2=xh[m1];
                              y2=yh[m1];
                              break;
		  //===========================================================
		  //     Case 4 - Line between vertex 1 and side 2-3
		  //===========================================================
                           case 4:
                              x1=xh[m1];
                              y1=yh[m1];
                              x2=xsect(m2,m3);
                              y2=ysect(m2,m3);
                              break;
		  //===========================================================
		  //     Case 5 - Line between vertex 2 and side 3-1
		  //===========================================================
                           case 5:
                              x1=xh[m2];
                              y1=yh[m2];
                              x2=xsect(m3,m1);
                              y2=ysect(m3,m1);
                              break;
		  //===========================================================
		  //     Case 6 - Line between vertex 3 and side 1-2
		  //===========================================================
                           case 6:
                              x1=xh[m3];
                              y1=yh[m3];
                              x2=xsect(m1,m2);
                              y2=ysect(m1,m2);
                              break;
		  //===========================================================
		  //     Case 7 - Line between sides 1-2 and 2-3
		  //===========================================================
                           case 7:
                              x1=xsect(m1,m2);
                              y1=ysect(m1,m2);
                              x2=xsect(m2,m3);
                              y2=ysect(m2,m3);
                              break;
		  //===========================================================
		  //     Case 8 - Line between sides 2-3 and 3-1
		  //===========================================================
                           case 8:
                              x1=xsect(m2,m3);
                              y1=ysect(m2,m3);
                              x2=xsect(m3,m1);
                              y2=ysect(m3,m1);
                              break;
		  //===========================================================
		  //     Case 9 - Line between sides 3-1 and 1-2
		  //===========================================================
                           case 9:
                              x1=xsect(m3,m1);
                              y1=ysect(m3,m1);
                              x2=xsect(m1,m2);
                              y2=ysect(m1,m2);
                              break;
                           default:
                              break;
                        }
		//=============================================================
		// Put your processing code here and comment out the printf
		//=============================================================
                        add_segment(SPair(SPoint(x1,y1),SPoint(x2,y2)),k);
                     }
                  }
               }
            }
         }
      }
   }
   return 0;
}

int ContourMap::generate_levels(double min, double max, int num)
{
   double step=(max-min)/(num-1);
   if(levels) delete levels;
   levels=new double[num];
   n_levels=num;
   for(int i=0;i<num;i++)
   {
      levels[i]=min+step*i;
   }
   return num;
}

ContourMap::ContourMap()
{
   levels=NULL;
   n_levels=0;
   contour_level=NULL;
}

int ContourMap::add_segment(SPair t, int level)
{
// ensure that the object hierarchy has been allocated
   if(!contour_level) contour_level=new std::vector<ContourLevel*>(n_levels);
   if(!(*contour_level)[level]) 
      (*contour_level)[level]=new ContourLevel;
   if(!(*contour_level)[level]->raw)
      (*contour_level)[level]->raw=new std::vector<SPair>;
// push the value onto the end of the vector
   (*contour_level)[level]->raw->push_back(t);
   return(0);
}

int ContourMap::dump(std::ofstream & fp)
{
   //sort the raw vectors if they exist
   std::vector<ContourLevel*>::iterator it=contour_level->begin();
   int l=0;
   while(it!=contour_level->end())
   {
      fp << "Contour data at level " << l << "[" << levels[l] << "]" << std::endl;
      if(*it) (*it)->dump(fp);
      it++;l++;
   }
   return(0);
}

int ContourMap::dump_osm(std::ofstream & fp)
{
   fp << "<?xml version='1.0' encoding='UTF-8'?>" << std::endl
      << "<osm version='0.5' generator='osm_contour'>"
      << "<bound box='-90,-180,90,180' origin='mkcntr'/>"
      << std::endl;
   
   // writing all the nodes in a primary pass
   for (std::vector<ContourLevel*>::iterator i  = contour_level->begin(); 
                                              i != contour_level->end(); i++)
   {
      if (*i && (*i)->contour_lines ) { 
      for(std::vector<Contour*>::iterator j  = (*i)->contour_lines->begin();
                                           j != (*i)->contour_lines->end(); ++j) {
          if (*j && (*j)->contour) { 
             double px = (*j)->start().x;
	     double py = (*j)->start().y;
	     for (std::vector<SVector>::iterator k  =  (*j)->contour->begin() ;
	                                         k !=  (*j)->contour->end() ; ++k) {
		 fp << "<node id ='" << std::setfill('0') << std::setw(3) << distance( contour_level->begin(), i) 
		            << ""    << std::setfill('0') << std::setw(3) << distance( (*i)->contour_lines->begin(), j)
			    << ""    << std::setfill('0') << std::setw(3) << distance( (*j)->contour->begin(), k)
			    << "' ";
	         px += k->dx;
		 py += k->dy;
		 fp << " lon='" << std::fixed << std::setprecision(5) << px << "'"
	            << " lat='" << std::fixed << std::setprecision(5) << py 
		    << "' user='aitzkora' visible='true' timestamp='0001-01-01T00:00:00'"
		    << " />" << std::endl;
		    
              } //fin for k
           } // fin *j != NULL
	 } // fin  for j    
      } // fin *i!=NULL
   } // fin for i
   
   
   // writing all the way in a primary pass
   for (std::vector<ContourLevel*>::iterator i  = contour_level->begin(); 
                                              i != contour_level->end(); i++)
   {
      if (*i && (*i)->contour_lines ) { 
      for(std::vector<Contour*>::iterator j  = (*i)->contour_lines->begin();
                                           j != (*i)->contour_lines->end(); ++j) {
          if (*j && (*j)->contour) { 
	     fp << "<way id='" 
	        << std::setfill('0')  << std::setw(3) << distance( contour_level->begin(), i)
                << std::setfill('0')  << std::setw(3) << distance( (*i)->contour_lines->begin(), j)
	        << "'"  << " timestamp='0001-01-01T00:00:00'>"
		<< std::endl;
	     for (std::vector<SVector>::iterator k  =  (*j)->contour->begin() ;
	                                         k !=  (*j)->contour->end() ; ++k) {
		 fp << "<nd ref ='" << std::setfill('0')  << std::setw(3) << distance( contour_level->begin(), i) 
		            << ""  << std::setfill('0')  << std::setw(3) << distance( (*i)->contour_lines->begin(), j)
			    << ""  << std::setfill('0') << std::setw(3) << distance( (*j)->contour->begin(), k)
			    << "'/>" << std::endl;
		    
              } //fin for k
            fp << "<tag k='contour' v='elevation' />" << std::endl;
	    fp << "<tag k='ele' v='" << levels[distance( contour_level->begin(), i)]
	       << "' />" << std::endl;

	    fp << "</way>" << std::endl;

	   } // fin *j != NULL
	 } // fin  for j    
      } // fin *i!=NULL
   } // fin for i
   fp << "</osm>" << std::endl; 
   return(0);
}
int ContourMap::consolidate()
{
   //sort the raw vectors if they exist
   std::vector<ContourLevel*>::iterator it=contour_level->begin();
   while(it!=contour_level->end())
   {
      if(*it) (*it)->consolidate();
      it++;
   }
   return(0);
}

ContourMap::~ContourMap()
{
   if(levels) delete levels;
   if(contour_level)
   {
      std::vector<ContourLevel*>::iterator it=contour_level->begin();
      while(it!=contour_level->end())
      {
         delete(*it);
         it=contour_level->erase(it);
      }
      contour_level->clear();
      delete contour_level;
   }
}

