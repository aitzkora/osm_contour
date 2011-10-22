#include "osm_contour.hpp"

//the ContourLevel class contains all the contour data for any given contour 
//level. initially this data is stored in point to point format in the raw
//vector, however functions exist to combine these vectors into groups (Contour)
//representing lines.

int ContourLevel::dump(std::ofstream  & fp)
{
// iterate thru the vector dumping values to STDOUT as we go
// this function is intended for debugging purposes only
   fp << "======================================================================" 
      << std::endl;
   if(raw)
   {
      fp << "Raw vector data" << std::endl <<  std::endl;
      std::vector<SPair>::iterator it;
      it=raw->begin();
      while(it!=raw->end())
      {
         SPair t=*it;
         fp << "(" << t.p1.x << "," << t.p1.y << ") " 
	    << "(" << t.p2.x << "," << t.p2.y << ")"
	    << std::endl;
         it++;
      }  
   }
   if(contour_lines)
   {
      fp << "Processed contour lines" << std::endl
                                      << std::endl;
      std::vector<Contour*>::iterator it=contour_lines->begin();
      int c=1;
      while(it!=contour_lines->end())
      {
         fp << "Contour line " << c << ":" << std::endl;
         (*it)->dump(fp);
         c++;it++;
      }
   }
   fp << "======================================================================" 
      << std::endl;
   return(0);
}

int ContourLevel::consolidate()
{
   std::vector<SPair>::iterator it;
   Contour *contour;
   int c=0;
   if(!raw) return(0);
   if (!contour_lines) contour_lines=new std::vector<Contour*>;
   std::sort(raw->begin(),raw->end());
   while(!raw->empty())
   {
      c++;
      it=raw->begin();
      contour=new Contour();
      contour->add_vector((*it).p1,(*it).p2);
      it=raw->erase(it);
      while(it!=raw->end())
      {
         if((*it).p1==contour->end())
         {
            contour->add_vector((*it).p1,(*it).p2);
            raw->erase(it);
            it=raw->begin();
         }
         else it++;
      }
      contour_lines->push_back(contour);
   }
   delete raw;raw=NULL;
   c-=merge();
   std::vector<Contour*>::iterator cit=contour_lines->begin();
   while(cit!=contour_lines->end())
   {
      (*cit)->condense();
      cit++;
   }
   return(c);
}

int ContourLevel::merge()
{
   std::vector<Contour*>::iterator it,jt;
   int c=0;
   if(contour_lines->size()<2) return(0);
   it=contour_lines->begin();
   /*
   using two iterators we walk through the entire vector testing to
   see if some combination of the start and end points match. If we
   find matching points the two vectorsrs are merged. since when we go
   thru the vector once we are garanteed that all vectors that can 
   connect to that oone have been merged we only have to merge the
   vector less the processed nodes at the begining. every merge does 
   force jt back to the beginning of the search tho since a merge will
   change either the start or the end of the vector
   */
   while(it!=contour_lines->end())
   {
      jt=it+1;
      while(jt!=contour_lines->end())
      {
         /*
         if the end of *it matches the start ot *jt we can copy
         *jt to the end of *it and remove jt, the erase funtion 
         does us a favour and increments the iterator to the 
         next element so we continue to test the next element
         */
         if((*it)->end()==(*jt)->start())
         {
            (*it)->merge(*jt);
            delete(*jt);
            contour_lines->erase(jt);
            jt=it+1;
            c++;
         }
         /*
         similarily if the end of *jt matches the start ot *it we can copy
         *it to the end of *jt and remove it,replacing it with jt, we then 
         neet to update it to point at the just inserted record.
         */
         else if((*jt)->end()==(*it)->start())
         {
            (*jt)->merge(*it);
            delete(*it);
            (*it)=(*jt);
            contour_lines->erase(jt);
            jt=it+1;
            c++;
         }
         /*
         if both segments end at the same point we reverse one and merge
         it to the other, then remove the one we merged.
         */
         else if((*it)->end()==((*jt)->end()))
         {
            (*jt)->reverse();
            (*it)->merge(*jt);
            delete(*jt);
            contour_lines->erase(jt);
            jt=it+1;
            c++;
         }
         /*
         if both segments start at the same point reverse it, then merge
         it with jt, removing jt and reseting jt to the start of the search
         sequence
         */
         else if((*it)->start()==((*jt)->start()))
         {
            (*it)->reverse();
            (*it)->merge(*jt);
            delete(*jt);
            jt=contour_lines->erase(jt);
            c++;
         }         
         else 
         {
            jt++;
         }
      }
      it++;
   }
   return(c);
}

ContourLevel::~ContourLevel()
{
   if(raw)
   {
      raw->clear();
      delete raw;
   }
   
   if(contour_lines)
   {
      std::vector<Contour*>::iterator it=contour_lines->begin();
      while(it!=contour_lines->end())
      {
         delete (*it);
         it=contour_lines->erase(it);
      }
      contour_lines->clear();
      delete contour_lines;
   }
}

