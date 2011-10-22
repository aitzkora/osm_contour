/***************************************************************************
 *   Copyright (C) 2007 by Bjorn Harpe,,,   *
 *   bjorn@ouelong.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

// based on the work of Paul Bourke and Nicholas Yue
#include "osm_contour.hpp"
#include <iomanip>
#include <algorithm>
#include <fstream>


/*
===============================================================================
the Contour class stores actual individual contour lines in a vector,addition
to the vector is handled by the add_vector function for individual vector 
components. in the case that one contour needs to be coppied to the end of 
another contour there is a merge function that will copy the second onto the
end of the first. individual accessors are provided for the start and end points
and the actual vector is publicly available.
*/

int Contour::add_vector(SPoint p1, SPoint p2)
{

// move the vector to the orgin   
   SVector v;
   v.dx=p2.x-p1.x;
   v.dy=p2.y-p1.y;
// if the contour vector does not exist create it   
// and set the starting point for this contour   
   if(!contour)
   {
      contour=new std::vector<SVector>;
      _start=p1;
   }
// insert the new vector to the end of the contour
// and update the end point for this contour   
   contour->push_back(v);
   _end=p2;
   return(0);
}

int Contour::reverse()
{
// swap the start and end points
   SPoint t=_end;
   _end=_start;
   _start=t;
// iterate thru the entire vector and reverse each individual element
// inserting them into a new vector as we go
   std::vector<SVector> *tmp=new std::vector<SVector>;
   std::vector<SVector>::iterator it=contour->begin();
   while(it!=contour->end())
   {
      (*it).dx*=-1;
      (*it).dy*=-1;
      tmp->insert(tmp->begin(),(*it));
      it++;
   }
// swap the old contour vector with the new reversed one we just generated
   delete contour;
   contour=tmp;
   return (0);
}

int Contour::merge(Contour *c)
{
   this->contour->insert(this->contour->end(),c->contour->begin(),c->contour->end());
   this->_end=c->_end;
   return(0);
} 

Contour::Contour(Contour *c)
{
   _start = c->_start;
   _end = c->_end;
   contour = new std::vector<SVector>(c->contour->size());
   std::copy(c->contour->begin(),c->contour->end(),
             contour->begin());
} 

int Contour::dump(std::ofstream & fp)
{
   fp << " Start: [" << _start.x << "," << _start.y << "]" << std::endl
      << " End: [" << _end.x << "," << _end.y << "]" << std::endl  
      << " Components>" << std::endl;
   std::vector<SVector>::iterator cit=contour->begin();
   int c=1;
   SPoint p=_start;
   while(cit!=contour->end())
   {
      p.x+=(*cit).dx;
      p.y+=(*cit).dy;
      fp << "  " << "{" <<(*cit).dx << "," << (*cit).dy << "}"
         << " " << "[" << p.x << "," << p.y << "]" << std::endl;
      c++,cit++;
   }
   return(0);
}

int Contour::condense(double difference)
{
   /*
   at this time we potentially have multiple SVectors in the contour vector that
   are colinear and could be condensed into one SVector with, to determine if two
   successive vectors are colinear we take each vector and divide the y component 
   of the vector by the x component, giving us the slope. we pass in a difference
   if the difference between th two slopes is less than the difference that we 
   pass in and since we already know that both segments share a common point they 
   can obviously be condensed. in the sample code on this page this is evident it 
   the bounding rectangle. another possibility is modifying the code to allow 
   point intersections on the plane. In this instance we may have multiple 
   identical vectors with no magnitude that can be reduced to a single data point.
   */
   
   std::vector<SVector>::iterator it,jt;
   double m1,m2;
   it=contour->begin();
   jt=it+1;
   while(jt!=contour->end())
   {
      if(((*jt).dx)&&((*it).dx))
      {
         m1=(*jt).dy/(*jt).dx;
         m2=(*it).dy/(*jt).dx;
      }
      else if(((*jt).dy)&&((*it).dy))
      {
         m1=(*jt).dx/(*jt).dy;
         m2=(*it).dx/(*jt).dy;
      }
      else
      {
         it++;jt++;
         continue;
      }
      if ((m1-m2<difference)&&(m2-m1<difference))
      {
         (*it).dy+=(*jt).dy;
         (*it).dx+=(*jt).dx;
         jt=contour->erase(jt);
      }
      else
      {
         it++;jt++;
      }
   }
   return(0);
}

Contour::~Contour()
{
   this->contour->clear();
   delete this->contour;
}
