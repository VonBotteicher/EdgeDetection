This is EdgeDetector, a Canny edge detection implementation for .PNGs 

 
Prerequisites
=============

- libpng (tested with libpng 1.2.46)

- zlib (tested with zlib 1.2.3.4)


General
=============

To make

	$ make all

To invoke

	$ ./EdgeDetector

The program will prompt you for a filename

	$ Enter filename like "./lena.png"
	$ Filename: ./lena.png

Color and grayscale .png's are supported.  

Outputs "gradient_mag.png" "edge_map.png" and "edge_map_binary.png" in the local 
directory. Note that gradient_mag.png and edge_map.png are intermediate images
from the red channel processor just to show the steps.  "edge_map_binary.png" is
the final output that uses all three channels. 


PNG++
=========

This project uses png++, a collection of header files that provide a C++ wrapper
for libpng.  The author is Alex Shulgin.

The project is hosted at Savannah:

	http://savannah.nongnu.org/projects/pngpp/


