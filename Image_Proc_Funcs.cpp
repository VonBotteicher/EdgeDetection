#include "Image_Proc_Funcs.hpp"
#include <iostream>

std::vector<std::vector<float> > generate_2D_gaussian(float sigma1, float sigma2)
{
	int rows = (int)sigma1*3.0; //round down
	int cols = (int)sigma2*3.0; //round down
	int mid_row = rows/2; //round down
	int mid_col = cols/2; //round down
	std::vector<std::vector<float> > gaussian;
	gaussian.resize(rows,std::vector<float>(cols));

	float normal = 1.0/(2.0 * PI * sigma1 *sigma2);
	for(int row = 0; row < rows; row++){
		for(int col = 0; col < cols; col++){
			gaussian[row][col] = normal * pow(E,-1*((pow(row-mid_row,2)/(2*pow(sigma1,2))) + (pow(col-mid_col,2)/(2*pow(sigma2,2)))));
		}
	}
	return gaussian;
}

std::vector<std::vector<uint8_t> > float_to_byte_2D(std::vector<std::vector<float> > img_in)
{
	int rows = img_in.size();
	int cols = img_in[0].size();

	std::vector<std::vector<uint8_t> > img_out;

	img_out.resize(rows,std::vector<uint8_t>(cols));

	for(int row = 0; row < rows; row++){
		for(int col = 0; col < cols; col++){
			img_out[row][col] = (uint8_t)std::max(0.0F,std::min(255.0F,img_in[row][col]));
		}
	}
	return img_out;
}

std::vector<std::vector<float> > byte_to_float_2D(std::vector<std::vector<uint8_t> > img_in)
{
	int rows = img_in.size();
	int cols = img_in[0].size();

	std::vector<std::vector<float> > img_out;

	img_out.resize(rows,std::vector<float>(cols));

	for(int row = 0; row < rows; row++){
		for(int col = 0; col < cols; col++){
			img_out[row][col] = (float)img_in[row][col];
		}
	}
	return img_out;
}
