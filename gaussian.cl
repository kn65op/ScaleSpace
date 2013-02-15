__kernel void  gaussian( __constant float * mask, __global float* input, __global float* output) 
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