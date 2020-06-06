#ifndef IMAGE_PROC_FUNCS
#define IMAGE_PROC_FUNCS

#include <vector>
#include <math.h>
#include <stdint.h>

#define PI 3.14159
#define E  2.71828

std::vector<std::vector<float> > generate_2D_gaussian(float sigma1,float sigma2);

std::vector<std::vector<uint8_t> > float_to_byte_2D(std::vector<std::vector<float> > img_in);

std::vector<std::vector<float> > byte_to_float_2D(std::vector<std::vector<uint8_t> > img_in);




#endif //IMAGE_PROC_FUNCS
