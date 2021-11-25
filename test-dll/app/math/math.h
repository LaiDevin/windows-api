#pragma once

#ifdef MATHLIBRARY_EXPORTS
#define MATH_API __declspec(dllexport)
#else
#define MATH_API __declspec(dllimport)
#endif 


#ifdef __cplusplus
extern "C" {
#endif 
	MATH_API int _stdcall add(int a, int b);
#ifdef __cplusplus
}
#endif // __cplusplus
