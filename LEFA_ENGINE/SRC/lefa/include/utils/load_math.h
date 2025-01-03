/* Open LEFA Project : load_math.h
2023
*/

#ifndef _LOAD_MATH_H_
#define _LOAD_MATH_H_

/*
here is a shortcut to load the math library
*/

#ifdef LOAD_HYP /* load Hypatia C Math Lib */
 #	define HYP_STATIC  /* staic befor function */
 #	define HYPATIA_SINGLE_PRECISION_FLOATS /* Use Float / if disabled = double */
 #	define HYPATIA_IMPLEMENTATION  /* Load Full */
 #	include"3rdparty/hypatia/hypatia.h"
	
#endif

#endif /* _LOAD_MATH_H_ */
