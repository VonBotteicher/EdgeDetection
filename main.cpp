
#include <string>
#include <iostream>

#include "png.h"
#include <png++/png.hpp>

#include "PNG_helper_funcs.hpp"
#include "Image_Proc_Funcs.hpp"
#include "Image_Worker.hpp"

int main(int argc, char *argv[]){
	 
	std::string filename;
	Image_Worker red_img_worker;
	Image_Worker green_img_worker;
	Image_Worker blue_img_worker;

	if(argc>1){
		std::cout << "Usage ./EdgeDetection"<< std::endl;
		std::cout << "No arguments needed."<< std::endl;
		exit(-1);
	}
    std::cout << "Enter filename like \"./lena.png\"" << std::endl;
    std::cout << "Filename: ";
    std::cin >> filename;
	std::cout << "Loading "<< filename<<"...\n";


	png::image< png::rgb_pixel > image(filename);


	RGB_Vectors rgb_vec = png_to_vectors(image);



	red_img_worker.load_image(byte_to_float_2D(rgb_vec.Red));
	green_img_worker.load_image(byte_to_float_2D(rgb_vec.Green));
	blue_img_worker.load_image(byte_to_float_2D(rgb_vec.Blue));


	std::vector<std::vector<float> > gaussian = generate_2D_gaussian(2.0, 2.0);

	red_img_worker.smooth_image(gaussian);
	green_img_worker.smooth_image(gaussian);
	blue_img_worker.smooth_image(gaussian);

	std::vector<std::vector<uint8_t> > image_blur_byte = float_to_byte_2D(red_img_worker.get_smoothed_image());
	png::image<png::rgb_pixel> image_out_blur = grayscale_to_png(image_blur_byte);

	red_img_worker.sobel_filt();
	green_img_worker.sobel_filt();
	blue_img_worker.sobel_filt();

	std::vector<std::vector<uint8_t> > gradient_mag_byte  = float_to_byte_2D(red_img_worker.get_gradient_magnitude());
	png::image<png::rgb_pixel> image_out_gradient_mag = grayscale_to_png(gradient_mag_byte);

	red_img_worker.edge_thinning();
	green_img_worker.edge_thinning();
	blue_img_worker.edge_thinning();

	std::vector<std::vector<uint8_t> > edge_thin_byte  = float_to_byte_2D(red_img_worker.get_edge_map());
	png::image<png::rgb_pixel> image_out_edge_thin = grayscale_to_png(edge_thin_byte);


	red_img_worker.hysteresis(70.0);
	green_img_worker.hysteresis(70.0);
	blue_img_worker.hysteresis(70.0);


	RGB_Vectors binary_edge_vectors = {red_img_worker.get_binary_edge_map(),
								   green_img_worker.get_binary_edge_map(),
								   blue_img_worker.get_binary_edge_map()};


	png::image<png::rgb_pixel> image_out_binary_edge = binary_vectors_to_png(binary_edge_vectors);

	image_out_gradient_mag.write("gradient_mag.png");
	image_out_edge_thin.write("edge_map.png");
	image_out_binary_edge.write("edge_map_binary.png");

	return 0;
}
