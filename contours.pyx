cdef extern from "Contours.h" namespace "contours":
     cdef cppclass ToMap:
       int _m,_n
       double *_mat
       toMap(int, int, double *)
       void setMap(int, int, double *)
     cdef cppclass CContourMap:
        int n_levels
        double *levels
        int generate_levels(double ,double , int )
        void contour()
        void consolidate()

cdef extern from "numpy/arrayobject.h":
    ctypedef int intp
    ctypedef extern class numpy.ndarray [object PyArrayObject]:
        cdef char *data
        cdef int nd
        cdef intp *dimensions
        cdef intp *strides
        cdef int flags

cdef class PyToMap:
     cdef ToMap *thisptr
     #constructor 
     def __cinit__(self, ndarray pm):
           cdef double *mat = <double *>pm.data
           cdef int m = pm.dimensions[0]
           cdef int n = pm.dimensions[1]
           self.thisptr = new ToMap()
           self.thisptr.setMap(m, n, mat) 
	       
     def __dealloc__(self):
           del self.thisptr

    
