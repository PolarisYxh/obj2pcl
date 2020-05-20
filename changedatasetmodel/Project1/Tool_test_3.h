//
// MATLAB Compiler: 6.4 (R2017a)
// Date: Tue Apr 14 16:30:23 2020
// Arguments:
// "-B""macro_default""-W""cpplib:Tool_test_3""-T""link:lib""-d""D:\0000\Tool_te
// st_3\for_testing""-v""D:\0000\Tool_test_3.m"
//

#ifndef __Tool_test_3_h
#define __Tool_test_3_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_Tool_test_3
#define PUBLIC_Tool_test_3_C_API __global
#else
#define PUBLIC_Tool_test_3_C_API /* No import statement needed. */
#endif

#define LIB_Tool_test_3_C_API PUBLIC_Tool_test_3_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_Tool_test_3
#define PUBLIC_Tool_test_3_C_API __declspec(dllexport)
#else
#define PUBLIC_Tool_test_3_C_API __declspec(dllimport)
#endif

#define LIB_Tool_test_3_C_API PUBLIC_Tool_test_3_C_API


#else

#define LIB_Tool_test_3_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_Tool_test_3_C_API 
#define LIB_Tool_test_3_C_API /* No special import/export declaration */
#endif

extern LIB_Tool_test_3_C_API 
bool MW_CALL_CONV Tool_test_3InitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_Tool_test_3_C_API 
bool MW_CALL_CONV Tool_test_3Initialize(void);

extern LIB_Tool_test_3_C_API 
void MW_CALL_CONV Tool_test_3Terminate(void);



extern LIB_Tool_test_3_C_API 
void MW_CALL_CONV Tool_test_3PrintStackTrace(void);

extern LIB_Tool_test_3_C_API 
bool MW_CALL_CONV mlxTool_test_3(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_Tool_test_3
#define PUBLIC_Tool_test_3_CPP_API __declspec(dllexport)
#else
#define PUBLIC_Tool_test_3_CPP_API __declspec(dllimport)
#endif

#define LIB_Tool_test_3_CPP_API PUBLIC_Tool_test_3_CPP_API

#else

#if !defined(LIB_Tool_test_3_CPP_API)
#if defined(LIB_Tool_test_3_C_API)
#define LIB_Tool_test_3_CPP_API LIB_Tool_test_3_C_API
#else
#define LIB_Tool_test_3_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_Tool_test_3_CPP_API void MW_CALL_CONV Tool_test_3(const mwArray& root_path, const mwArray& ani_range, const mwArray& ani_count_ori, const mwArray& photo_count, const mwArray& dof_name);

#endif
#endif
