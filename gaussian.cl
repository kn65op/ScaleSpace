__kernel void  gaussian_not_image( __constant float * mask, __global float* input, __global float* output) 
{
  int width_output = get_global_size(0); 
  int width = get_global_size(0) + 2; 

  int j = get_global_id(0); //column number
  int i = get_global_id(1) + 1; //row number
  int iout = get_global_id(1); 

  int ptr = i * width + j; 
  int ptro = iout * width_output+ j; 

  output[ptro] = 
  input[(i-1)*width + j -1] * mask[0] + 
  input[(i-1)*width + j] * mask[1] + 
  input[(i-1)*width + j+1] * mask[2] +
  input[(i)*width + j-1] * mask[3] +
  input[(i)*width + j] * mask[4] +
  input[(i)*width + j+1] * mask[5] +
  input[(i+1)*width + j-1] * mask[6] +
  input[(i+1)*width + j] * mask[7] +
  input[(i+1)*width + j+1] *mask[8] ;
  
}

const sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_MIRRORED_REPEAT | CLK_FILTER_NEAREST;


__kernel void  gaussian(__read_only image2d_t input, __write_only image2d_t output) 
{
  int width_output = get_global_size(0); 
  int width = get_global_size(0); 

  int j = get_global_id(0); //column number
  int i = get_global_id(1); //row number
  
  
  write_imagef(output, (int2)(i,j), 
  read_imagef(input, sampler, (int2)(i-1, j-1)) * 0.1 + 
  read_imagef(input, sampler, (int2)(i, j-1)) * 0.1 + 
  read_imagef(input, sampler, (int2)(i+1, j-1)) * 0.1 +
  read_imagef(input, sampler, (int2)(i-1, j)) * 0.1 +
  read_imagef(input, sampler, (int2)(i, j)) * 0.1 +
  read_imagef(input, sampler, (int2)(i+1, j)) * 0.1 +
  read_imagef(input, sampler, (int2)(i-1, j+1)) * 0.1 +
  read_imagef(input, sampler, (int2)(i, j+1)) * 0.1 +
  read_imagef(input, sampler, (int2)(i+1, j+1)) * 0.1);
  
}