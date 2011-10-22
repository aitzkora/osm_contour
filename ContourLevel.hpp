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
#ifndef _CONTOURSLEVEL_HPP
#define _CONTOURSLEVEL_HPP
#include <vector>
#include <fstream>

#include "Points.hpp"
#include "Contour.hpp"

class ContourLevel
{
   public:
     ContourLevel(){ contour_lines=NULL;
                     raw=NULL;};
     ~ContourLevel();
     
     int dump(std::ofstream & fp);
     int merge();
     int consolidate();
     std::vector<Contour*> *contour_lines;
     std::vector<SPair> *raw;
}; 


#endif
