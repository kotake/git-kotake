#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<CL/cl.h>
#include<string>
#include<time.h>

#define MAX_SOURCE_SIZE (0x100000)
#define MEM_SIZE (CL_DEVICE_MAX_WORK_ITEM_SIZES)

using namespace std;



//test
int main()
{
  //char string[MEM_SIZE];

  FILE *fp;
  char fileName[] = "./calc.cl";
  char *source_str;
  size_t source_size;

  fp = fopen(fileName, "r");
  if(!fp)
  {
    cout << "file failed" <<endl;
    exit(1);
  }
  source_str = (char *)malloc(MAX_SOURCE_SIZE);
  source_size = fread(source_str,1,MAX_SOURCE_SIZE,fp);
  fclose(fp);

  cl_int status;
  cl_platform_id platforms[2];
  cl_uint num_platforms;
  status = clGetPlatformIDs(2, platforms, &num_platforms);
  //cout << status << endl;

  cl_device_id devices;
  cl_uint num_devices;
  cl_context context;
  status = clGetDeviceIDs(platforms[1], CL_DEVICE_TYPE_GPU, 4, &devices, &num_devices);
  //cout << status << endl;

  context = clCreateContext(NULL, 4, &devices, NULL, NULL, &status);
  //cout << status << endl;

  cl_command_queue queue;
  queue = clCreateCommandQueue(context, devices, 0, &status);
  //cout <<"queue:"<<status << endl;

  cl_program program;
  program = clCreateProgramWithSource(context, 1, (const char**)&source_str, &source_size, &status);
  //cout << status << endl;

  status = clBuildProgram(program, num_devices, &devices, NULL, NULL, NULL);
  cout << status << endl;

  //*////////////////////////

  size_t logsize;
  status = clGetProgramBuildInfo(program,devices,CL_PROGRAM_BUILD_LOG,0,NULL,&logsize);
  if(status == CL_SUCCESS)
  {
    char *logbuffer;
    logbuffer = new char[logsize + 1];
    if(logbuffer == NULL)
    {
      cout << "memory allocation failed."<<endl;
      //return;
    }

    status = clGetProgramBuildInfo(program,devices,CL_PROGRAM_BUILD_LOG,logsize,logbuffer,NULL);
    if(status == CL_SUCCESS)
    {
      logbuffer[logsize]='\0';
      cout << "build log" << endl;
      cout << logbuffer << endl;
    }
    delete [] logbuffer;
  }

  //*////////////////////////

  cl_kernel kernel;
  kernel = clCreateKernel(program, "calc", &status);
  //cout << status << endl;

  float Outx[MEM_SIZE];
  float Outy[MEM_SIZE];
  //bool OutCheck[MEM_SIZE][MEM_SIZE];
  bool *OutCheck = new bool[MEM_SIZE*MEM_SIZE];
  Outx[0] = -2;
  Outy[0] = -2;
  for(int i= 0; i < MEM_SIZE-1 ;i++)
  { 
    Outx[i+1] = Outx[i] + (4.0 / MEM_SIZE);
    Outy[i+1] = Outy[i] + (4.0 / MEM_SIZE);

    for(int j = 0; j < MEM_SIZE ;j++)
    {
      OutCheck[i*MEM_SIZE+j] = true;
    }
  }

  cl_mem memoutx, memouty, memoutcheck;
  memoutx = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(float), NULL, &status);
  //cout <<status<<endl;
  memouty = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(float), NULL, &status);
  //cout <<status<<endl;
  memoutcheck = clCreateBuffer(context, CL_MEM_READ_WRITE, MEM_SIZE * MEM_SIZE * sizeof(bool), NULL, &status);
  //cout <<status<<endl;

  status = clEnqueueWriteBuffer( queue, memoutx, CL_TRUE, 0, MEM_SIZE * sizeof( float ), Outx, 0, NULL, NULL );
  //cout << status << endl;
  status = clEnqueueWriteBuffer( queue, memouty, CL_TRUE, 0, MEM_SIZE * sizeof( float ), Outy, 0, NULL, NULL );
  //cout << status << endl;
  status = clEnqueueWriteBuffer( queue, memoutcheck, CL_TRUE, 0, MEM_SIZE * MEM_SIZE * sizeof( bool ), OutCheck, 0, NULL, NULL );
  //cout << status << endl;

  status = clSetKernelArg(kernel, 0,sizeof(cl_mem),(void *)&memoutx);
  status = clSetKernelArg(kernel, 1,sizeof(cl_mem),(void *)&memouty);
  status = clSetKernelArg(kernel, 2,sizeof(cl_mem),(void *)&memoutcheck);
  //cout <<"カーネルに渡す引数セット："<<status<<endl;
  
  //status = clEnqueueTask(queue, kernel, 0, NULL, NULL);
  size_t global_work_size[] = {MEM_SIZE * MEM_SIZE};
  size_t local_work_size[]  = {MEM_SIZE};
  
  
  clock_t t1, t2;
  t1 = clock();
  status = clEnqueueNDRangeKernel(queue, kernel, 1, local_work_size, global_work_size, NULL, 0, NULL, NULL);
  t2 = clock();
  if( status < 0){
    cout <<"実行："<<status<<endl;
    cout << CL_DEVICE_MAX_WORK_GROUP_SIZE << endl;
    cout << CL_DEVICE_MAX_WORK_ITEM_SIZES << endl;
    exit(0);
  }

  //status = clEnqueueReadBuffer(queue, memoutx, CL_TRUE, 0, MEM_SIZE * sizeof(float), Out, 0, NULL, NULL);
  //status = clEnqueueReadBuffer(queue, memouty, CL_TRUE, 0, MEM_SIZE * sizeof(float), Out, 0, NULL, NULL);
  status = clEnqueueReadBuffer(queue, memoutcheck, CL_TRUE, 0, MEM_SIZE * MEM_SIZE * sizeof(bool), OutCheck, 0, NULL, NULL);
  //cout <<"結果取り出し："<<status<<endl;

  for(int i = 0;i < MEM_SIZE; i++)
  {
    for(int j = 0;j < MEM_SIZE; j++)
    {
      if(OutCheck[i*MEM_SIZE+j] == true){
        cout << Outx[i]<< " " << Outy[j] << endl;
      }
    }
  }

  //cout <<"time = "<< (double)(t2 - t1) / CLOCKS_PER_SEC << endl;

  delete OutCheck; 

  clReleaseMemObject(memoutx);
  clReleaseMemObject(memouty);
  clReleaseMemObject(memoutcheck);
  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseCommandQueue(queue);
  clReleaseContext(context);
}

