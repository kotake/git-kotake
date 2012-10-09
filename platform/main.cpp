#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <CL/cl.h>

#define DATA_SIZE (1024)
typedef long POINTER_VALUE;

#define GetAndCHK(a) _getAndChk(a,__LINE__)

using namespace std;

/* Function headers*/
extern "C"
{
  int printDeviceInfos(int cnt, cl_device_id* clDeviceIDs);
  inline void _getAndChk(int a, int l);
}

inline void _getAndChk(int a, int l)
{
  if(a){
    cout<<"ERROR:"<< a << " LINE:" << l << endl;
    exit(0);
  }
}

int main(int argc, char** argv)
{
  char cBuffer[1024];
  cl_uint clPlatformCount = 0;

  GetAndCHK(clGetPlatformIDs(0, NULL, &clPlatformCount));
  cout << noshowbase << uppercase << right;
  cout << left << setw(40) << "Platform Count:" << right <<setw(40) << clPlatformCount << endl;
  cl_platform_id *clPlatformIDs = new cl_platform_id[clPlatformCount];
  GetAndCHK(clGetPlatformIDs(clPlatformCount, clPlatformIDs, NULL));

  for(int i = 0; i < clPlatformCount; i++)
  {
    cl_platform_id clSelectedPlatformID = clPlatformIDs[i];
    cout << left << setw(40) << "Platform ID:" << right << setw(32) << "0x" << hex << setw(8) << setfill('0') << (POINTER_VALUE)clSelectedPlatformID << setfill(' ') << endl;

    GetAndCHK(clGetPlatformInfo(clSelectedPlatformID, CL_PLATFORM_NAME, sizeof(cBuffer), cBuffer, NULL));
    cout << left << setw(40) << "CL_PLATFORM_NAME:" << right << setw(40) << cBuffer << endl;

    GetAndCHK(clGetPlatformInfo(clSelectedPlatformID, CL_PLATFORM_VENDOR, sizeof(cBuffer), cBuffer, NULL));
    cout << left << setw(40) << "CL_PLATFORM_VENDOR:" << right << setw(40) << cBuffer << endl;

    GetAndCHK(clGetPlatformInfo(clSelectedPlatformID, CL_PLATFORM_VERSION, sizeof(cBuffer), cBuffer, NULL));
    cout << left << setw(40) << "CL_PLATFORM_VERSION:" << right << setw(40) << cBuffer << endl;

    GetAndCHK(clGetPlatformInfo(clSelectedPlatformID, CL_PLATFORM_EXTENSIONS, sizeof(cBuffer), cBuffer, NULL));
    cout << left << setw(40) << "CL_PLATFORM_EXTENSIONS" << right << setw(40) << cBuffer << endl;

    cl_uint clDeviceCount = 0;
    GetAndCHK(clGetDeviceIDs(clSelectedPlatformID, CL_DEVICE_TYPE_ALL, 0, NULL, &clDeviceCount));
    cl_device_id *clDeviceIDs = new cl_device_id[clDeviceCount];
    GetAndCHK(clGetDeviceIDs(clSelectedPlatformID, CL_DEVICE_TYPE_ALL, clDeviceCount, clDeviceIDs, NULL));
    cout << endl;
    cout << left << setw(40) << "Device Count:" << right << setw(40) << clDeviceCount << endl;
    printDeviceInfos(clDeviceCount, clDeviceIDs);
  }
  return 0;
}

int printDeviceInfos(int cnt, cl_device_id* clDeviceIDs)
{
  char cBuffer[1024];
  for(int i =0; i < cnt; i++)
  {
    cl_device_id clSelectedDeviceID = clDeviceIDs[i];

    cout << left << setw(40) << "Device ID:" << right << setw(32) << "0x" << hex <<setw(8) << setfill('0') << (POINTER_VALUE)clSelectedDeviceID << setfill(' ') << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_NAME, sizeof(cBuffer), cBuffer, NULL));
    cout << left << setw(40) << "CL_DEVICE_NAME:" <<right << setw(40) <<cBuffer << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_VENDOR, sizeof(cBuffer), cBuffer, NULL));
    cout << left << setw(40) << "CL_DEVICE_VENDOR:" << right << setw(40) << cBuffer << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DRIVER_VERSION,sizeof(cBuffer), cBuffer, NULL));
    cout << left << setw(40) << "CL_DRIVER_VERSION:" << right << setw(40) << cBuffer << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_PROFILE,sizeof(cBuffer), cBuffer, NULL));
    cout << left << setw(40) << "CL_DEVICE_PROFILE:" << right << setw(40) << cBuffer << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_VERSION,sizeof(cBuffer), cBuffer, NULL));
    cout << left << setw(40) << "CL_DEVICE_VERSION:" << right << setw(40) << cBuffer << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_EXTENSIONS, sizeof(cBuffer), cBuffer, NULL));
    cout << left << setw(40) << "CL_DEVICE_EXTENSIONS:" << right << endl << left << cBuffer << endl;

    cl_platform_id clPlatformId;
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_PLATFORM, sizeof(cl_platform_id), &clPlatformId, NULL));
    cout << left << setw(40) << "CL_DEVICE_PLATFORM:" << right << setw(32) << "0x" << hex << setw(8) <<setfill('0') << (POINTER_VALUE)clPlatformId << setfill(' ') << endl;

    cl_bitfield clBitField;
    cl_uint clUInt;
    cl_ulong clUlong;
    cl_bool clBool;
    size_t sz;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_TYPE, sizeof(clBitField), &clBitField, NULL));
    cout << left << setw(40) << "CL_DEVICE_TYPE:" << endl;
    if(clBitField & CL_DEVICE_TYPE_DEFAULT) cout << right << setw(76) << "CL_DEVICE_TYPE_DEFAULT" << endl;
    if(clBitField & CL_DEVICE_TYPE_CPU) cout << right << setw(76) << "CL_DEVICE_TYPE_CPU" << endl;
    if(clBitField & CL_DEVICE_TYPE_GPU) cout << right << setw(76) << "CL_DEVICE_TYPE_GPU" << endl;
    if(clBitField & CL_DEVICE_TYPE_ACCELERATOR) cout << right << setw(76) << "CL_DEVICE_TYPE_ACCELERATOR" << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_VENDOR_ID, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_VENDOR_ID" << right << setw(32) << "0x" << hex << setw(8) <<setfill('0') << clUInt << setfill(' ') << dec << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID,CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(clUInt),&clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_COMPUTE_UNITS:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS:" << right << setw(40) << clUInt << endl;

    size_t* clWorkItemSizes = new size_t[clUInt];
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * clUInt, clWorkItemSizes, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_WORK_ITEM_SIZES:" <<  right << setw(40);
    ostringstream os;
    os << "(";
    for(int i = 0; i < clUInt; i++)
    {
      os << (unsigned long)clWorkItemSizes[i];
      if(i < clUInt - 1) os << " , ";
    }
    os << ")." << flush;
    cout << os.str() << endl;
    delete [] clWorkItemSizes;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(sz), &sz, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_WORK_GROUP_SIZE:" << right << setw(40) << (unsigned long)sz << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(sz), &sz, NULL));
    cout << left << setw(40) << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT:" << right << setw(40) << (unsigned long)sz << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_CLOCK_FREQUENCY:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_ADDRESS_BITS, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_ADDRESS_BITS:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(clUlong), &clUlong, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_MEM_ALLOC_SIZE:" << right << setw(40) << clUlong << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_IMAGE_SUPPORT, sizeof(clBool), &clBool, NULL));
    cout << boolalpha;
    cout << left << setw(40) << "CL_DEVICE_IMAGE_SUPPORT:" << right << setw(40) << (bool)clBool << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MAX_READ_IMAGE_ARGS, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_READ_IMAGE_ARGS:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MAX_WRITE_IMAGE_ARGS, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_WRITE_IMAGE_ARGS:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(sz), &sz, NULL));
    cout << left << setw(40) << "CL_DEVICE_IMAGE2D_MAX_WIDTH:" << right << setw(40) << (unsigned long)sz << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(sz), &sz, NULL));
    cout << left << setw(40) << "CL_DEVICE_IMAGE2D_MAX_HEIGHT:" << right << setw(40) << (unsigned long)sz << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(sz), &sz, NULL));
    cout << left << setw(40) << "CL_DEVICE_IMAGE3D_MAX_WIDTH:" << right << setw(40) << (unsigned long)sz << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(sz), &sz, NULL));
    cout << left << setw(40) << "CL_DEVICE_IMAGE3D_MAX_HEIGHT:" << right << setw(40) << (unsigned long)sz << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(sz), &sz, NULL));
    cout << left << setw(40) << "CL_DEVICE_IMAGE3D_MAX_DEPTH:" << right << setw(40) << (unsigned long)sz << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MAX_SAMPLERS, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_SAMPLERS:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(sz), &sz, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_PARAMETER_SIZE:" << right << setw(40) << (unsigned long)sz << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MEM_BASE_ADDR_ALIGN, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_MEM_BASE_ADDR_ALIGN:" << right << setw(40) << clUInt << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE:" << right << setw(40) << clUInt << endl;

    cl_device_fp_config clDeviceFpConfig;
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_SINGLE_FP_CONFIG, sizeof(clDeviceFpConfig), &clDeviceFpConfig, NULL));
    cout << left << setw(40) << "CL_DEVICE_SINGLE_FP_CONFIG:"  << endl;

    if(clDeviceFpConfig & CL_FP_DENORM) cout << right << setw(76) << "CL_FP_DENORM" << endl;
    if(clDeviceFpConfig & CL_FP_INF_NAN) cout << right << setw(76) << "CL_FP_INF_NAN" << endl;
    if(clDeviceFpConfig & CL_FP_ROUND_TO_NEAREST) cout << right << setw(76) << "CL_FP_ROUND_TO_NEAREST" << endl;
    if(clDeviceFpConfig & CL_FP_ROUND_TO_ZERO) cout << right << setw(76) << "CL_FP_ROUND_TO_ZERO" << endl;
    if(clDeviceFpConfig & CL_FP_ROUND_TO_INF) cout << right << setw(76) << "CL_FP_ROUND_TO_INF" << endl;
    if(clDeviceFpConfig & CL_FP_FMA) cout << right << setw(76) << "CL_FP_FMA" << endl;

    cl_device_mem_cache_type clDeviceMemCashType;
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, sizeof(clDeviceMemCashType), &clDeviceMemCashType, NULL));
    cout << left << setw(40) << "CL_DEVICE_GLOBAL_MEM_CACHE_TYPE:" << endl;
    if(clDeviceMemCashType == CL_NONE) cout << right << setw(76) << "CL_NONE" << endl;
    if(clDeviceMemCashType == CL_READ_ONLY_CACHE) cout << right << setw(76) << "CL_READ_ONLY_CACHE" << endl;
    if(clDeviceMemCashType == CL_READ_WRITE_CACHE) cout << right << setw(76) << "CL_READ_WRITE_CACHE" << endl;
    
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE:" << right << setw(40) << clUInt << endl;
    
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(clUlong), &clUlong, NULL));
    cout << left << setw(40) << "CL_DEVICE_GLOBAL_MEM_CACHE_SIZE:" << right << setw(40) << clUlong << endl;
    
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(clUlong), &clUlong, NULL));
    cout << left << setw(40) << "CL_DEVICE_GLOBAL_MEM_SIZE:" << right << setw(40) << clUlong << endl;
    
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(clUlong), &clUlong, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE:" << right << setw(40) << clUlong << endl;
    
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(clUInt), &clUInt, NULL));
    cout << left << setw(40) << "CL_DEVICE_MAX_CONSTANT_ARGS:" << right << setw(40) << clUInt << endl;

    cl_device_local_mem_type clDeviceLocalMemType;
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_LOCAL_MEM_TYPE, sizeof(clDeviceLocalMemType), &clDeviceLocalMemType, NULL));
    cout << left << setw(40) << "CL_DEVICE_LOCAL_MEM_TYPE:" << endl;
    if(clDeviceLocalMemType == CL_LOCAL) cout << right << setw(76) << "CL_LOCAL" << endl;
    if(clDeviceLocalMemType == CL_GLOBAL) cout << right << setw(76) << "CL_GLOBAL" << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(clUlong), &clUlong, NULL));
    cout << left << setw(40) << "CL_DEVICE_LOCAL_MEM_SIZE:" << right << setw(40) << clUlong << endl;
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_ERROR_CORRECTION_SUPPORT, sizeof(clBool), &clBool, NULL));
    cout << boolalpha;
    cout << left << setw(40) << "CL_DEVICE_ERROR_CORRECTION_SUPPORT:" << right << setw(40) << (bool)clBool << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_PROFILING_TIMER_RESOLUTION, sizeof(sz), &sz, NULL));
    cout << left << setw(40) << "CL_DEVICE_PROFILING_TIMER_RESOLUTION:" << right << setw(40) << (unsigned long)sz << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_ENDIAN_LITTLE, sizeof(clBool), &clBool, NULL));
    cout << left << setw(40) << "CL_DEVICE_ENDIAN_LITTLE:" << right << setw(40) << clBool << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_AVAILABLE, sizeof(clBool), &clBool, NULL));
    cout << left << setw(40) << "CL_DEVICE_AVAILABLE:" << right << setw(40) << clBool << endl;

    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_COMPILER_AVAILABLE, sizeof(clBool), &clBool, NULL));
    cout << left << setw(40) << "CL_DEVICE_COMPILER_AVAILABLE:" << right << setw(40) << clBool << endl;

    cl_device_exec_capabilities clDeviceExecCapabilities;
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_EXECUTION_CAPABILITIES, sizeof(clDeviceExecCapabilities), &clDeviceExecCapabilities, NULL));
    cout << left << setw(40) << "CL_DEVICE_EXECUTION_CAPABILITIES:" << endl;
    if(clDeviceExecCapabilities & CL_EXEC_KERNEL) cout << right << setw(76) << "CL_EXEC_KERNEL" << endl;
    if(clDeviceExecCapabilities & CL_EXEC_NATIVE_KERNEL) cout << right << setw(76) << "CL_EXEC_NATIVE_KERNEL" << endl;

    cl_command_queue_properties clCommandQueueProperties;
    GetAndCHK(clGetDeviceInfo(clSelectedDeviceID, CL_DEVICE_QUEUE_PROPERTIES, sizeof(clCommandQueueProperties), &clCommandQueueProperties, NULL));
    cout << left << setw(40) << "CL_DEVICE_QUEUE_PROPERTIES" << endl;
    if(clCommandQueueProperties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) cout << right << setw(76) << "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE" << endl;
    if(clCommandQueueProperties & CL_QUEUE_PROFILING_ENABLE) cout << right << setw(76) << "CL_QUEUE_PROFILING_ENABLE" << endl;
  }
  return 0;
}
