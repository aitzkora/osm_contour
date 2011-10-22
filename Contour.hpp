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
#ifndef __CCONTOUR_HPP__
#define __CCONTOUR_HPP__
#include <vector>
#include <fstream>

#include "Points.hpp"

class Contour
{
   public:
      
      Contour(){ contour = NULL;}
      Contour(Contour * cc); 
      ~Contour();
      
      int merge(Contour *c);
      int reverse();
      int add_vector(SPoint start, SPoint end);
      int condense(double difference = 0.000000001);
      int dump(std::ofstream & fp);
      bool closed(){return(_start==_end);}
      
      SPoint start() { return _start; }
      SPoint end() { return _end; }
      std::vector<SVector> *contour;
   private:
      SPoint _start;
      SPoint _end;
};

#endif
