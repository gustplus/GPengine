#ifndef _GPMATH_H_
#define _GPMATH_H_

#include <math.h>
#include "types.h"
#include "stdafx.h"

const float PI = 3.14159f;
#define TO_RADIANS (PI / 180.0f)
#define TO_DEGREES (180.0f / PI)
namespace GPEngine3D{
	class Math{
	public:
		static int_32 ceil(double var);

		static int_32 floor(double var);

		static double sin(double p_x);

		static double cos(double p_x);

		static double tan(double p_x);

		static double sinh(double p_x);

		static double cosh(double p_x);

		static double tanh(double p_x);

		static double asin(double p_x);

		static double acos(double p_x);

		static double atan(double p_x);

		static double atan2(double p_y, double p_x);
	};

	extern byte log2Map[513];
}

#endif