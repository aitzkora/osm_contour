# import stdio function
from libc.stdio cimport * 
# import std::vector
cdef extern from "<vector>" namespace "std":
    cdef cppclass vector[T]:
        cppclass iterator:
            T operator*()
            iterator operator++()
            bint operator==(iterator)
            bint operator!=(iterator)
        vector()
        void push_back(T&)
        T& operator[](int)
        T& at(int)
        iterator begin()
        iterator end()
# import Contour classes
cdef extern from "Contours.h" namespace "contours":
     cdef cppclass ToMap:
       int _m,_n
       double *_mat
       toMap(int, int, double *)
       void setMap(int, int, double *)
       double value(double, double)
     cdef cppclass CContourMap:
        CContourMap()
        int n_levels
        double *levels
        int generate_levels(double ,double , int )
        void contour(ToMap *)
        void consolidate()
        void dump(FILE *fp)
     cdef cppclass SVector:
        double x
        double y
     cdef cppclass SPoint:
         double x
         double y
     cdef cppclass SPair:
         SPoint p1
         SPoint p2
     cdef cppclass CContour:
         vector[SVector]  *contour	 
     cdef cppclass CContourLevel:
         vector[SPair] *raw
         vector[CContour] *contour_lines
          

cdef extern from "numpy/arrayobject.h":
    ctypedef int intp
    ctypedef extern class numpy.ndarray [object PyArrayObject]:
        cdef char *data
        cdef int nd
        cdef intp *dimensions
        cdef intp *strides
        cdef int flags

cdef class PyToMap:
     cdef ToMap *thisMap
     cdef CContourMap *thisContour
     #constructor 
     def __cinit__(self, ndarray pm, double min_elev,
                                     double max_elev,
				     int num):
           cdef double *mat = <double *>pm.data
           cdef int m = pm.dimensions[0]
           cdef int n = pm.dimensions[1]
           self.thisMap = new ToMap()
           self.thisMap.setMap(m, n, mat) 
           self.thisContour = new CContourMap ()
           self.thisContour.generate_levels(min_elev, max_elev, num)
           self.thisContour.contour(self.thisMap)
           self.thisContour.consolidate()
	       
     def __dealloc__(self):
           del self.thisContour 
           del self.thisMap
     def __getitem__(self, index):
           return self.thisMap.value(index[0],index[1])
     def dump_osm(self, filename):
         cdef char * name = filename
         cdef FILE * fp=fopen(name,"w")
         self.thisContour.dump(fp)
         fclose(fp)

