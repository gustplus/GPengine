#pragma once
#include <math.h>
#include <iostream>

namespace GPEngine3D{

	template <class T>
	class Vector2
	{
	public:
		friend std::iostream& operator << (std::iostream& out, Vector2<T>& vector);
		friend bool operator==(Vector2<T> &v1, Vector2<T> &v2);
		friend bool operator!=(Vector2<T> &v1, Vector2<T> &v2);

		T x;
        T y;

        Vector2(T fx = 0, T fy = 0) : x(fx), y(fy){}
        
        ~Vector2(void){}

		T operator[](unsigned int i) const
		{
			if (i == 0) return x;
			return y;
		}

		T& operator[](unsigned int i)
		{
			if (i == 0) return x;
			return y;
		}

		void operator+=(const Vector2& v)
		{
			x += v.x;
			y += v.y;
		}


		Vector2 operator+(const Vector2& v) const
		{
			return Vector2(x+v.x, y+v.y);
		}

		T operator-=(const Vector2& v)
		{
			x -= v.x;
			y -= v.y;
		}

		Vector2 operator-(const Vector2& v) const
		{
			return Vector2(x-v.x, y-v.y);
		}

		void operator*=(const float value)
		{
			x *= value;
			y *= value;
		}

		Vector2 operator*(const T value) const
		{
			return Vector2(x*value, y*value);
		}

		T dotMul(const Vector2 & vector) const
		{
			return x*vector.x + y*vector.y;
		}

		T operator *(const Vector2 &vector) const
		{
			return x*vector.x + y*vector.y;
		}

		void scale(const Vector2 &vector)
		{
			x *= vector.x;
			y *= vector.y;
		}

		T lenSquared()
		{
			return x*x+y*y;
		}

		float len()
		{
			return sqrtf(x*x+y*y);
		}

		Vector2 normalize()
		{
			float length = len();
			if(length>0)
			{
				x /= length;
				y /= length;
			}
			return *this;
		}

		T dist(Vector2 vector)
		{
			float distX = x - vector.x;
			float distY = y - vector.y;
			return sqrt(distX * distX + distY * distY);
		}

		T distSquared(Vector2 vector)
		{
			float distX = x - vector.x;
			float distY = y - vector.y;
			return distX * distX + distY * distY;
		}
	};
    
    template<class T> std::iostream& operator << (std::iostream& out, Vector2<T>& vector)
    {
        out<<vector.x<<vector.y;
        return out;
    }
    
    template <class T>
    bool operator==(Vector2<T> &v1, Vector2<T> &v2)
    {
        return v1.x == v2.x && v1.y == v2.y;
    }
    
    template <class T>
    bool operator!=(Vector2<T> &v1, Vector2<T> &v2)
    {
        return v1.x != v2.x || v1.y != v2.y;
    }
    
    //Vector3
	template <class T>
	class Vector3
	{
	public:
		friend std::iostream& operator << (std::iostream& out, Vector3<T>& vector);
		friend bool operator==(Vector3<T> &v1, Vector3<T> &v2);
		friend bool operator!=(Vector3<T> &v1, Vector3<T> &v2);

        T x;
        T y;
        T z;
        
        Vector3(T fx = 0, T fy = 0, T fz = 0) : x(fx), y(fy), z(fz){}
        
        Vector3(Vector2<T> &v, T fz = 0) : x(v.x), y(v.y), z(fz){}
        
        ~Vector3(void){}

		T operator[](unsigned int i) const
		{
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		T& operator[](unsigned int i)
		{
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		void operator+=(const Vector3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
		}


		Vector3 operator+(const Vector3& v) const
		{
			return Vector3(x+v.x, y+v.y, z+v.z);
		}

		void operator-=(const Vector3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
		}

		Vector3 operator-(const Vector3& v) const
		{
			return Vector3(x-v.x, y-v.y, z-v.z);
		}

		void operator*=(const T value)
		{
			x *= value;
			y *= value;
			z *= value;
		}

		T dotMul(const Vector3 & vector) const
		{
			return x*vector.x + y*vector.y + z*vector.z;
		}

		Vector3 operator*(const T value) const
		{
			return Vector3(x*value, y*value, z*value);
		}

		Vector3 crossMul(Vector3& vector) const
		{
			return Vector3(y*vector.z-z*vector.y,
				z*vector.x-x*vector.z,
				x*vector.y-y*vector.x);
		}

		T operator *(const Vector3 &vector) const
		{
			return x*vector.x + y*vector.y + z*vector.z;
		}

		void scale(const Vector3 &vector)
		{
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
		}

		T lenSquared()
		{
			return x*x+y*y+z*z;
		}

		float len()
		{
			return sqrtf(x*x+y*y+z*z);
		}

		Vector3 normalize()
		{
			float length = len();
			if(length>0)
			{
				x /= length;
				y /= length;
				z /= length;
			}
			return *this;
		}

		float dist(Vector3 vector)
		{
			float distX = x - vector.x;
			float distY = y - vector.y;
			float distZ = z - vector.z;
			return sqrt(distX * distX + distY * distY + distZ * distZ);
		}

		T distSquared(Vector3 vector)
		{
			float distX = x - vector.x;
			float distY = y - vector.y;
			float distZ = z - vector.z;
			return distX * distX + distY * distY + distZ * distZ;
		}
	};

	template<class T>
	std::iostream& operator << (std::iostream& out, Vector3<T>& vector)
    {
        out<<vector.x<<vector.y<<vector.z;
        return out;
    }
    
    template <class T>
    bool operator==(Vector3<T> &v1, Vector3<T> &v2)
    {
        return v1.x == v2.x && v1.y == v2.y && v1.z != v2.z;
    }
    
    template <class T>
    bool operator!=(Vector3<T> &v1, Vector3<T> &v2)
    {
        return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
    }


	typedef Vector2<float> vec2f;
	typedef Vector2<int> vec2i;
	typedef Vector2<int> Size;
	typedef Vector2<int> Point2i;
	typedef Vector2<float> Point2f;
	typedef Vector3<float> Bound;
    typedef Vector3<float> vec3f;
	typedef Vector3<float> Point3f;
    typedef Vector2<float> Vector2f;
    typedef Vector3<float> Vector3f;
}

