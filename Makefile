CC=g++

INCPATH = -I/usr/include/libpng -I./include/

LIBS = -L/usr/lib/i386-linux-gnu -lpng

CPP_FLAGS = -std=gnu++0x

all: Edge_detector

Edge_detector: main.o Image_Worker.o PNG_helper_funcs.o Image_Proc_Funcs.o
	 $(CC) -o Edge_detector Image_Worker.o Image_Proc_Funcs.o PNG_helper_funcs.o main.o $(LIBS)

main.o: main.cpp Image_Worker.hpp PNG_helper_funcs.hpp Image_Proc_Funcs.hpp 
	$(CC) $(CPP_FLAGS) $(INCPATH) -o main.o -c main.cpp 

PNG_helper_funcs.o: PNG_helper_funcs.cpp PNG_helper_funcs.hpp
	$(CC) $(CPP_FLAGS) $(INCPATH) -o PNG_helper_funcs.o -c PNG_helper_funcs.cpp 

Image_Proc_Funcs.o: Image_Proc_Funcs.cpp Image_Proc_Funcs.hpp
	$(CC) $(CPP_FLAGS) $(INCPATH) -o Image_Proc_Funcs.o -c Image_Proc_Funcs.cpp 

Image_Worker.o: Image_Worker.cpp Image_Worker.hpp
	$(CC) $(CPP_FLAGS) $(INCPATH) -o Image_Worker.o -c Image_Worker.cpp 

clean:
	rm -f *.o
	rm -f Edge_detector
