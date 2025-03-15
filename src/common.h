#ifndef RINDOW_MATLIB_COMMON_H_
#define RINDOW_MATLIB_COMMON_H_

#include "rindow_matlib_config.h"

#include <stdint.h>

#ifdef _MSC_VER
#include <windows.h> // For Windows
#elif defined(__APPLE__)
#include <sys/sysctl.h> // For macOS
#include <mach/mach.h> // For macOS
#else
#include <sys/sysinfo.h> // For Linux and other Unix-like systems
#endif
#ifdef _OPENMP
#include <omp.h>
#endif

#include <stdlib.h>
#ifndef _OPENMP
  #ifdef _MSC_VER
    #include <sysinfoapi.h>  // Include Windows-specific header for system information
  #elif defined(__APPLE__)
    #include <sys/sysctl.h>  // Include sysctl.h for system information
    #include <pthread.h>     // Include pthread.h for POSIX threads
  #else
    #include <sys/sysinfo.h>  // Include sysinfo.h for system information (Linux)
    #include <pthread.h>      // Include pthread.h for POSIX threads
  #endif
    #include <memory.h>
#endif
#ifdef __cplusplus
  #include <exception>
#endif

#ifdef __cplusplus
#define RINDOW_BEGIN_CLEAR_EXCEPTION \
    try { 
#define RINDOW_END_CLEAR_EXCEPTION \
    } catch(std::exception &e) { \
        const char *msg = e.what(); \
        rindow_matlib_common_console("matlib error: %s\n",msg); \
    } catch (...) { \
        rindow_matlib_common_console("matlib error: unknown error\n"); \
    }
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifndef _OPENMP
void *s_sum_kernel(void *varg);
void *d_sum_kernel(void *varg);
#endif // _OPENMP

void rindow_matlib_common_console(const char *format, ...);
int32_t rindow_matlib_common_copy_ex(int32_t dtype,int32_t n,void* source,int32_t incSource,void* dest,int32_t incDest);
int32_t rindow_matlib_common_add_ex(int32_t dtype,int32_t n,void* source,int32_t incSource,void* dest,int32_t incDest);

int32_t rindow_matlib_common_get_integer(int32_t dtype, void *buffer, int32_t incWidth,int32_t index, int64_t *value);
int32_t rindow_matlib_common_set_integer(int32_t dtype, void *buffer, int32_t incWidth,int32_t index, int64_t value);
int32_t rindow_matlib_common_get_float(int32_t dtype, void *buffer, int32_t incWidth,int32_t index, double *value);
int32_t rindow_matlib_common_set_float(int32_t dtype, void *buffer, int32_t incWidth,int32_t index, double value);

void rindow_matlib_common_s_add(int32_t n, float *x, float *y);
void rindow_matlib_common_d_add(int32_t n, double *x, double *y);
void rindow_matlib_common_i_add(int32_t n, int32_t dtype, void *x, void *y);
float rindow_matlib_common_s_sum_sb(int32_t n, float *x, int32_t incX);
double rindow_matlib_common_d_sum_sb(int32_t n, double *x, int32_t incX);
float rindow_matlib_common_s_max(int32_t n, float *x, int32_t incX);
double rindow_matlib_common_d_max(int32_t n, double *x, int32_t incX);
int32_t rindow_matlib_common_s_argmax(int32_t n, float *x, int32_t incX);
int32_t rindow_matlib_common_d_argmax(int32_t n, double *x, int32_t incX);

#if _MSC_VER
#define RINDOW_MATLIB_COMMON_RAND_MAX 0x7fffffff
#else
#define RINDOW_MATLIB_COMMON_RAND_MAX RAND_MAX
#endif
int32_t rindow_matlib_common_rand();
void rindow_matlib_common_srand(int32_t seed);

#ifdef __cplusplus
} // extern "C"
#endif

// RINDOW_MATLIB_COMMON_H_
#endif
