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
#ifndef _CONTOURMAP_HPP
#define _CONTOURMAP_HPP
#include <vector>
#include <fstream>

#include "ContourLevel.hpp"
#include "Map.hpp"

class ContourMap
{
   public:
      ContourMap();
      int generate_levels(double min,double max, int num);
      int add_segment(SPair t,int level);
      int dump(std::ofstream & fp);
      int dump_osm(std::ofstream &fp);
      int contour(Map *r);
      int consolidate();
      int get_n_levels() {return n_levels;}
      ContourLevel* level(int i){return((*contour_level)[i]);}
      double alt(int i){return(levels[i]);}
      ~ContourMap();
   private:
      std::vector<ContourLevel*> *contour_level;
      int n_levels;
      double *levels;      
};

#endif
