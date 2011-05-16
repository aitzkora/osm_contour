all: 
	@python setup.py build_ext --inplace

clean : 
	@rm -f contours.so contours.cpp
	@python setup.py clean
