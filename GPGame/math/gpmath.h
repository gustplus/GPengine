#ifndef _GPMATH_H_
#define _GPMATH_H_

#define PI 3.14159f;
#define TO_RADIANS ((1 / 180f) * (float) PI)
#define TO_DEGREES ((1 / (float) PI) * 180)

inline int d_ceil(double var)
{
	if(var < 0)
	{
		return (int) var;
	}
	int intVar = (int)var;
	if(intVar < var)
	{
		return (intVar + 1);
	}else{
		return intVar;
	}
}

inline int d_floor(double var)
{
	if(var > 0)
	{
		return (int)var;
	}
	
	int intVar = (int)var;
	if(intVar > var){
		return (intVar - 1);
	}else{
		return intVar;
	}
}

template<class T>
inline void swap(T &a, T &b)
{
	T &tmp = a;
	a = b;
	b = tmp;
}

#endif