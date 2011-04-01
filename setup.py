# To compile:
#      python setup.py build_ext --inplace

from distutils.core import setup, Extension
from Cython.Distutils import build_ext
import numpy.distutils.misc_util

include_dirs = numpy.distutils.misc_util.get_numpy_include_dirs()

setup (name = "contour_OSM",
       version = "0.1",

       ext_modules = [Extension('contour',
                                ['conrec.pyx'],
                                include_dirs=include_dirs)],

       cmdclass = {'build_ext': build_ext}
       )

