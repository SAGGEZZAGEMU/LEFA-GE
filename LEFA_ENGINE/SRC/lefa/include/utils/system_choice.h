/*Open LEFA Project : system_choice.h
2023 Sild Team*/

#ifndef _SYSTEM_CHOICE_H_
#define _SYSTEM_CHOICE_H_



/* COMPILATION */
/* system file extension */
#ifdef _WIN32
#       define SYS_STATIC_LIB  ".lib"
#       define SYS_DYNAMIC_LIB ".dll"
#       define SYS_EXEC_FORMAT ".exe"
/**/
#       define SYS_STATIC_LIB_UP  ".LIB"
#       define SYS_DYNAMIC_LIB_UP ".DLL"
#       define SYS_EXEC_FORMAT_UP  ".EXE"
/*
*/
#elif defined _LINUX || _BSD
#       define SYS_STATIC_LIB  ".a"
#       define SYS_DYNAMIC_LIB ".so"
#       define SYS_EXEC_FORMAT ".elf"
/**/
#       define SYS_STATIC_LIB_UP  ".A"
#       define SYS_DYNAMIC_LIB_UP ".SO"
#       define SYS_EXEC_FORMAT_UP  ".ELF"
/*
*/
#elif defined _APPLE
#       define SYS_STATIC_LIB  ".a"
#       define SYS_DYNAMIC_LIB ".dylib"
#       define SYS_EXEC_FORMAT ".app"
/**/
#       define SYS_STATIC_LIB_UP  ".A"
#       define SYS_DYNAMIC_LIB_UP ".DYLIB"
#       define SYS_EXEC_FORMAT_UP  ".APP"
#endif

#define STATICLIBRARY SYS_STATIC_LIB SYS_STATIC_LIB_UP
#define DYNAMICLIBRARY SYS_DYNAMIC_LIB SYS_DYNAMIC_LIB_UP
#define EXECUTABLE SYS_EXEC_FORMAT SYS_EXEC_FORMAT_UP


#ifdef _WIN32
 #ifdef __cplusplus  /* C++ Windows Library */
  #define LE_APIEXPORT(ret, name, args) extern "C" __declspec(dllexport) ret name args
  #define LE_APIIMPORT(ret, name, args) extern "C" __declspec(dllimport) ret name args
 #else  /* C Windows Library */
  #define LE_APIEXPORT(ret, name, args) __declspec(dllexport) ret name args
  #define LE_APIIMPORT(ret, name, args) __declspec(dllimport) ret name args
 #endif
#else
 #ifdef __cplusplus /* C++ Another Sys Library */
  #define LE_APIEXPORT(ret, name, args) extern "C" ret name args
  #define LE_APIIMPORT(ret, name, args) extern "C" ret name args
#else   /* C Another Sys Library */
  #define LE_APIEXPORT(ret, name, args) ret name args
  #define LE_APIIMPORT(ret, name, args) ret name args
#endif
#endif

#ifdef LIB_EXPORT /* if called, IMPORT */
 #define LE_APICALL(ret, name, args) LE_APIIMPORT(ret, name, args) /* IMPORT */
#else
 #define LE_APICALL(ret, name, args) LE_APIEXPORT(ret, name, args) /* EXPORT */
#endif

#if __GNUC__ >= 4
 #define LE_SYS_ATTR_CONST __attribute__((const))
 #define LE_SYS_ATTR_FORMAT(i, j) __attribute__((format (printf, i, j)))
 #define LE_SYS_ATTR_NORETURN __attribute__((noreturn))
 #define LE_SYS_ATTR_SENTINEL __attribute__((sentinel))
#else
 #define LE_SYS_ATTR_CONST 0
 #define LE_SYS_ATTR_FORMAT(i, j)
 #define LE_SYS_ATTR_NORETURN 0
 #define LE_SYS_ATTR_SENTINEL 0
#endif


#endif /* _SYSTEM_CHOICE_H_ */
