#ifndef IMAGE_WORKER
#define IMAGE_WORKER

#include <vector>
#include <math.h>
#include <stdint.h>

struct Sobel_Output{
	std::vector<std::vector<float> > edge_x;
	std::vector<std::vector<float> > edge_y;
	std::vector<std::vector<float> > gradient_mag;
	std::vector<std::vector<float> > gradient_theta;
};

struct Edge_Thinning_Output{
	std::vector<std::vector<float> > theta_0;
	std::vector<std::vector<float> > theta_45;
	std::vector<std::vector<float> > theta_neg_45;
	std::vector<std::vector<float> > theta_90;
	std::vector<std::vector<float> > edge_thin;
};

class Image_Worker{

public:
	Image_Worker();
	~Image_Worker();
	std::vector<std::vector<float> > conv2(std::vector<std::vector<float> > image_in, 
                                      std::vector<std::vector<float> > kernel);

	void sobel_filt();
	Edge_Thinning_Output edge_thinning();
	void hysteresis(float threshold);

	void load_image(std::vector<std::vector<float> > image_in);
	void smooth_image(std::vector<std::vector<float> > kernel);
	std::vector<std::vector<float> > get_smoothed_image() const;
	std::vector<std::vector<float> > get_gradient_magnitude() const;
	std::vector<std::vector<float> > get_edge_map() const;
	std::vector<std::vector<uint8_t> > get_binary_edge_map() const ;
private:
	 // can't initialize like ...sobel_x { { -1.0f, 0.0f, 1.0f},....
	// I guess g++ 4.6.3 doesn't have this C++11 feature;
	std::vector<std::vector<float> > sobel_x;
	std::vector<std::vector<float> > sobel_y;
	std::vector<std::vector<float> > input_image;
	std::vector<std::vector<float> > smoothed_image;
	std::vector<std::vector<uint8_t> > binary_edge_map;
   
	void pad_image(std::vector<std::vector<float> > image_in, int row_pad, int col_pad);
	std::vector<std::vector<float> > padded_image;

	Sobel_Output sobel_output;
	Edge_Thinning_Output edge_thinning_output;
	
	
};


#endif //IMAGE_WORKER
