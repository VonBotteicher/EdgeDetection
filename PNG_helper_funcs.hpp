#ifndef PNG_HELPER
#define PNG_HELPER
#include <vector>
#include "png.h"
#include <png++/png.hpp>
#include <stdint.h>

enum Channel { Red, Green, Blue };

struct RGB_Vectors{
	std::vector<std::vector<uint8_t> > Red;
	std::vector<std::vector<uint8_t> > Green;
	std::vector<std::vector<uint8_t> > Blue;
};

RGB_Vectors png_to_vectors(png::image< png::rgb_pixel >);


png::image< png::rgb_pixel > vectors_to_png(RGB_Vectors);

png::image< png::rgb_pixel > grayscale_to_png(std::vector<std::vector<uint8_t> >);

png::image< png::rgb_pixel > binary_vectors_to_png(RGB_Vectors);


#endif //PNG_HELPER
