#include "gpmath.h"

namespace GPEngine3D{
	byte log2Map[513] =  
	{
		0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3, 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4, 
		5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5, 
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6, 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6, 
		6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6, 6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6, 
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 
		7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, 7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
		
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
		8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
	};

	int_32 Math::ceil(double var)
	{
		if(var < 0)
		{
			return (int_32)var;
		}
		int_32 intVar = (int_32)var;
		if(intVar < var)
		{
			return (intVar + 1);
		}else{
			return intVar;
		}
	}

	int_32 Math::floor(double var)
	{
		if(var > 0)
		{
			return (int_32)var;
		}
		
		int_32 intVar = (int_32)var;
		if(intVar > var){
			return (intVar - 1);
		}else{
			return intVar;
		}
	}

	double Math::sin(double p_x) {
		return ::sin(p_x);
	}

	double Math::cos(double p_x) {
		return ::cos(p_x);
	}

	double Math::tan(double p_x) {
		return ::tan(p_x);
	}

	double Math::sinh(double p_x) {
		return ::sinh(p_x);
	}

	double Math::cosh(double p_x) {
		return ::cosh(p_x);
	}

	double Math::tanh(double p_x) {
		return ::tanh(p_x);
	}

	double Math::asin(double p_x) {
	return ::asin(p_x);
	}

	double Math::acos(double p_x) {
		return ::acos(p_x);
	}

	double Math::atan(double p_x) {
		return ::atan(p_x);
	}

	double Math::atan2(double p_y, double p_x) {
		return ::atan2(p_y,p_x);
	}

	int Math::random() {
		return (::rand() % 255);
	}
}