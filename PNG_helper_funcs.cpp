#include "PNG_helper_funcs.hpp"


RGB_Vectors png_to_vectors(png::image< png::rgb_pixel > image){
	RGB_Vectors rgb_out;
	unsigned int width = image.get_width();
	unsigned int height = image.get_height();
	rgb_out.Red.resize(height, std::vector<uint8_t>(width));
	rgb_out.Green.resize(height, std::vector<uint8_t>(width));
	rgb_out.Blue.resize(height, std::vector<uint8_t>(width));
	for(int row = 0; row<height; row++){
		for(int col = 0; col<width; col++){
			png::rgb_pixel pix = image.get_pixel(col,row);
			rgb_out.Red[row][col] = pix.red;
			rgb_out.Green[row][col] = pix.green;
			rgb_out.Blue[row][col] = pix.blue;
		}	 

	}	 
	return rgb_out; //will use move constructor
} 


png::image< png::rgb_pixel > vectors_to_png(RGB_Vectors rgb_vecs){
	int rows = rgb_vecs.Red.size();
	int cols = rgb_vecs.Red[0].size();

	png::image< png::rgb_pixel > image(cols, rows);

	for( int row = 0; row < rows; row++){
		for(int col = 0; col < cols; col++){
			image[row][col] = png::rgb_pixel(rgb_vecs.Red[row][col],
											 rgb_vecs.Green[row][col],
											 rgb_vecs.Blue[row][col]);
		}
	}
	return image;
}

png::image< png::rgb_pixel > grayscale_to_png(std::vector<std::vector<uint8_t> > gray){
	int rows = gray.size();
	int cols = gray[0].size();

	png::image< png::rgb_pixel > image(cols, rows);

	for( int row = 0; row < rows; row++){
		for(int col = 0; col < cols; col++){
			image[row][col] = png::rgb_pixel(gray[row][col],
											 gray[row][col],
											 gray[row][col]);
		}
	}
	return image;
}


png::image< png::rgb_pixel > binary_vectors_to_png(RGB_Vectors rgb_vecs){

	int rows = rgb_vecs.Red.size();
	int cols = rgb_vecs.Red[0].size();
	uint8_t value;

	png::image< png::rgb_pixel > image(cols, rows);

		for( int row = 0; row < rows; row++){
		for(int col = 0; col < cols; col++){
			value = (rgb_vecs.Red[row][col] == 255 | 
					rgb_vecs.Green[row][col] == 255 |
					rgb_vecs.Blue[row][col] == 255) ? 255 : 0; 
			image[row][col] = png::rgb_pixel(value,
											 value,
											 value);
		}
	}
	return image;
}
