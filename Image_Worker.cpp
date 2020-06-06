
#include "Image_Worker.hpp"
#include "Image_Proc_Funcs.hpp"
#include <iostream>


Image_Worker::Image_Worker(){

	sobel_x.resize(3,std::vector<float>(3));
	sobel_y.resize(3,std::vector<float>(3));
	sobel_x[0][0] = -1.0f; sobel_x[0][1] = 0.0f; sobel_x[0][2] = 1.0f;
	sobel_x[1][0] = -2.0f; sobel_x[1][1] = 0.0f; sobel_x[1][2] = 2.0f;
	sobel_x[2][0] = -1.0f; sobel_x[2][1] = 0.0f; sobel_x[2][2] = 1.0f;

	sobel_y[0][0] = -1.0f; sobel_y[0][1] = -2.0f; sobel_y[0][2] = -1.0f;
	sobel_y[1][0] = 0.0f; sobel_y[1][1] = 0.0f; sobel_y[1][2] = 0.0f;
	sobel_y[2][0] = 1.0f; sobel_y[2][1] = 2.0f; sobel_y[2][2] = 1.0f;
	
}
Image_Worker::~Image_Worker(){

}


void Image_Worker::load_image(std::vector<std::vector<float> > image_in){
	this->input_image.insert(this->input_image.end(),image_in.begin(),image_in.end());   //using "this->" for clarity
}

void Image_Worker::smooth_image(std::vector<std::vector<float> > kernel){
	smoothed_image = conv2(input_image, kernel);
}

std::vector<std::vector<float> > Image_Worker::get_smoothed_image() const {
	return smoothed_image;
}

std::vector<std::vector<float> > Image_Worker::get_gradient_magnitude() const {
	return sobel_output.gradient_mag;
}

std::vector<std::vector<float> > Image_Worker::get_edge_map() const {
	return edge_thinning_output.edge_thin;
}

std::vector<std::vector<uint8_t> > Image_Worker::get_binary_edge_map() const {
	return binary_edge_map;
}

void Image_Worker::pad_image(std::vector<std::vector<float> > image_in, int row_pad, int col_pad)
{
	int rows = image_in.size();
	int cols = image_in[0].size();

	
	padded_image.resize(rows+2*row_pad,std::vector<float>(cols+2*col_pad));
	for(int row = 0; row < (rows + 2*row_pad); row++){
		for(int col = 0; col < (cols + 2*col_pad); col++){
			if(row < row_pad){
				padded_image[row][col] = image_in[0][std::max(0,std::min(cols,col-col_pad))]; //repeat edge values
			}else if(row >= rows+row_pad){
				padded_image[row][col] = image_in[rows-1][std::max(0,std::min(cols,col-col_pad))];//repeat edge values
			}else if(col < col_pad){
				padded_image[row][col] = image_in[std::max(0,std::min(rows,row-row_pad))][0];//repeat edge values
			}else if(col >= cols+col_pad){
				padded_image[row][col] = image_in[std::max(0,std::min(rows,row-row_pad))][cols-1];//repeat edge values
			}else{ //fill inside with image
				//std::cout<< "row = "<<row<<" col = "<<col<<std::endl;
				padded_image[row][col] = image_in[row-row_pad][col-col_pad];
			}
		}
	}
}

std::vector<std::vector<float> > Image_Worker::conv2(std::vector<std::vector<float> > image_in, 
                                      std::vector<std::vector<float> > kernel)
{
	float sum = 0;

	int rows = image_in.size();
	int cols = image_in[0].size();

	int k_rows = kernel.size();
	int k_cols = kernel[0].size();

	std::vector<std::vector<float> > img_out;
	img_out.resize(rows, std::vector<float>(cols));

	int kernel_height = kernel.size();
	int kernel_width = kernel[0].size();

	int half_kernel_height = (int)kernel_height/2; // round down  
	int half_kernel_width = (int)kernel_width/2; // round down

	pad_image(image_in,k_rows-1,k_cols-1);

	for( int row = 0; row < rows; row++){
		for( int col = 0; col < cols; col++){
			for(int k_row = 0; k_row < k_rows; k_row++){
				for(int k_col = 0; k_col < k_cols; k_col++){
					 sum += kernel[k_row][k_col]*this->padded_image[row+k_row][col+k_col];
				}
			}
			img_out[row][col] = sum; 
			sum = 0;
		}	
	}
	return img_out;
}


void Image_Worker::sobel_filt()
{
	int rows = smoothed_image.size();
	int cols = smoothed_image[0].size();	

	//Sobel_Output sobel_struct;
	sobel_output.edge_x.resize(rows,std::vector<float>(cols));
	sobel_output.edge_y.resize(rows,std::vector<float>(cols));
	sobel_output.gradient_mag.resize(rows,std::vector<float>(cols));
	sobel_output.gradient_theta.resize(rows,std::vector<float>(cols));

	sobel_output.edge_x = conv2(smoothed_image,sobel_x);
	sobel_output.edge_y = conv2(smoothed_image,sobel_y);

	for(int row = 0; row < rows; row++){
		for(int col = 0; col < cols; col++){
			sobel_output.gradient_mag[row][col] = sqrt(pow(sobel_output.edge_x[row][col],2)+pow(sobel_output.edge_y[row][col],2));
			sobel_output.gradient_theta[row][col] = atan(sobel_output.edge_y[row][col]/sobel_output.edge_x[row][col]);
		}
	}
}

Edge_Thinning_Output Image_Worker::edge_thinning(){
	int rows = sobel_output.gradient_theta.size();
	int cols = sobel_output.gradient_theta[0].size();	

	float r = 0.0;
	float q = 0.0;

	//Edge_Thinning_Output thinning_struct;
	edge_thinning_output.theta_0.resize(rows,std::vector<float>(cols,0.0));
	edge_thinning_output.theta_45.resize(rows,std::vector<float>(cols,0.0));
	edge_thinning_output.theta_neg_45.resize(rows,std::vector<float>(cols,0.0));
	edge_thinning_output.theta_90.resize(rows,std::vector<float>(cols,0.0));
	edge_thinning_output.edge_thin.resize(rows,std::vector<float>(cols,0.0));

	for(int row = 1; row <  rows-1; row++){ //start and end 1 away from the edge
		for(int col = 1; col <  cols-1; col++){
			//place the angle of the gradient into 4 bins, and compare gradient
			//magnitude values 1 pixel away along the gradient
			if((sobel_output.gradient_theta[row][col]>=(-PI/8.0)) && (sobel_output.gradient_theta[row][col]<(PI/8.0))){
				edge_thinning_output.theta_0[row][col] = 255.0;
				r = sobel_output.gradient_mag[row][col+1];
				q = sobel_output.gradient_mag[row][col-1];
			}else if((sobel_output.gradient_theta[row][col]>=(-PI*(3.0/8.0))) && (sobel_output.gradient_theta[row][col]<(-PI/8.0))){
				edge_thinning_output.theta_45[row][col] = 255.0;
				r = sobel_output.gradient_mag[row-1][col+1];
				q = sobel_output.gradient_mag[row+1][col-1];
			}else if((sobel_output.gradient_theta[row][col]<(PI*(3.0/8.0))) && (sobel_output.gradient_theta[row][col]>=(PI/8.0))){
				edge_thinning_output.theta_neg_45[row][col] = 255.0;
				r = sobel_output.gradient_mag[row+1][col+1];
				q = sobel_output.gradient_mag[row-1][col-1];
			}else if((sobel_output.gradient_theta[row][col]<(-PI*(3.0/8.0))) || (sobel_output.gradient_theta[row][col]>=(PI*(3.0/8.0)))){
				edge_thinning_output.theta_90[row][col] = 255.0;
				r = sobel_output.gradient_mag[row+1][col];
				q = sobel_output.gradient_mag[row-1][col];
			}else{// no gradient
				r = 0.0;
				q = 0.0;
			}

			if(sobel_output.gradient_mag[row][col]>=r && sobel_output.gradient_mag[row][col]>=q){
				//if there are no larger neighboring gradient values 
				edge_thinning_output.edge_thin[row][col] = sobel_output.gradient_mag[row][col];
			}else{//should already be zero, but just for clarity
				edge_thinning_output.edge_thin[row][col] = 0.0;
			} 
		}
	}
	return edge_thinning_output;
}

void Image_Worker::hysteresis(float threshold){

	int rows = edge_thinning_output.edge_thin.size();
	int cols = edge_thinning_output.edge_thin[0].size();
	
	binary_edge_map.resize(rows,std::vector<uint8_t>(cols,0));

	for(int row = 0; row < rows; row++){
		for(int col = 0; col < cols; col++){
			binary_edge_map[row][col] = (edge_thinning_output.edge_thin[row][col] > threshold) ? 255 : 0; 
		}
	}
}











