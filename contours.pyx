cdef extern from "Contours.h" namespace "contours":
     cdef cppclass ToMap:
       int _m,_n
       double *_mat
       toMap(double *, int , int)
     cdef cppclass CContourMap:
        int n_levels
        double *levels
        int generate_levels(double ,double , int )
        void contour()
        void consolidate()

