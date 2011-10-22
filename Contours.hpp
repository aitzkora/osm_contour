/***************************************************************************
 *   Copyright (C) 2007 by Bjorn Harpe,,,   *
 *   bjorn@Ouelong.com   *
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
#ifndef CONTOURS_H
#define CONTOURS_H
#include <vector>
#include <algorithm>
#include <fstream>

#include "Points.hpp"
#include "Map.hpp"

class CContour
{
   public:
      CContour(){contour=NULL;}
      CContour(CContour * cc); 
      ~CContour();
      int merge(CContour *c);
      int reverse();
      int add_vector(SPoint start,SPoint end);
      int condense(double difference = 0.000000001);
      int dump(std::ofstream & fp);
      bool closed(){return(_start==_end);}
      SPoint start(){return(_start);}
      SPoint end(){return(_end);}
      std::vector<SVector> *contour;
   private:
      SPoint _start,_end;
};

class CContourLevel
{
   public:
     CContourLevel(){contour_lines=NULL;raw=NULL;};
     int dump(std::ofstream & fp);
     int merge();
     int consolidate();
     std::vector<CContour*> *contour_lines;
     std::vector<SPair> *raw;
     ~CContourLevel();
}; 

class CContourMap
{
   public:
      CContourMap();
      int generate_levels(double min,double max, int num);
      int add_segment(SPair t,int level);
      int dump(std::ofstream & fp);
      int dump_osm(std::ofstream &fp);
      int contour(Map *r);
      int consolidate();
      int get_n_levels() {return n_levels;}
      CContourLevel* level(int i){return((*contour_level)[i]);}
      double alt(int i){return(levels[i]);}
      ~CContourMap();
   private:
      std::vector<CContourLevel*> *contour_level;
      int n_levels;
      double *levels;      
};

#endif
