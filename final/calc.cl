//#define MEM_SIZE (2000)

__kernel void
calc(__global float *outx,
     __global float *outy,
     __global bool *outcheck )
{

  #define MaxCalcNum 10000

  //int width = 2, height = 2, j = 0;
  //int out[MEM_SIZE*MEM_SIZE];

  #pragma OPENCL EXTENSION cl_khr_fp64:enable
  //int index_x = get_global_id(0);
  //int index_y = get_global_id(1);
  int index_x = get_group_id(0);
  int index_y = get_local_id(0);

  //if(index_x > MEM_SIZE)index_x = 1;
  //if(index_y > MEM_SIZE)index_y = 1;
  double prevX = 0.0f;
  double prevY = 0.0f;

  for(int i = 0; i < MaxCalcNum ; i++)
    {
      double newX = (prevX * prevX) - (prevY * prevY) + outx[index_x];
      double newY = 2.0f * prevX * prevY + outy[index_y];

      if ((newX * newX) + (newY * newY) > 4.0f)
      {
        outcheck[index_x * get_local_size(0) + index_y] = false;
        break;
      }
      else
      {
        prevX = newX;
        prevY = newY;
      }
    }
}
