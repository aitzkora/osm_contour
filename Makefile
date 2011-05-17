all: 
	@python setup.py build_ext --inplace

clean : 
	@rm -f contours.so contours.cpp contours.o Contours.o 
	@python setup.py clean
