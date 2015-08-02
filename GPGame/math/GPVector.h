#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>

namespace GPEngine3D{
	template<class T> class Vector2;
	template<class T>
	std::ostream& operator <<(std::ostream& out, const Vector2<T>& vector);
	template<class T>
	bool operator==(const Vector2<T> &v1, const Vector2<T> &v2);
	template<class T>
	bool operator!=(const Vector2<T> &v1, const Vector2<T> &v2);

	template <class T>
	class Vector2
	{
	public:
		friend std::ostream& operator<< <T>(std::ostream& out, const Vector2<T>& vector);
		friend bool operator== <T>(const Vector2<T> &v1, const Vector2<T> &v2);
		friend bool operator!= <T>(const Vector2<T> &v1, const Vector2<T> &v2);

		T x;
        T y;

        Vector2(T fx = 0, T fy = 0) : x(fx), y(fy){}

        Vector2(const Vector2<T> &from, const Vector2<T> &to) : x(to.x - from.x), y(to.y - from.y){}
        
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

		T lenSquared() const
		{
			return x*x+y*y;
		}

		float len() const
		{
			return sqrtf(x*x+y*y);
		}

		Vector2 normalize() const
		{
			float length = len();
			if(length>0)
			{
				x /= length;
				y /= length;
			}
			return *this;
		}

		T dist(Vector2 vector) const
		{
			float distX = x - vector.x;
			float distY = y - vector.y;
			return sqrt(distX * distX + distY * distY);
		}

		T distSquared(Vector2 vector) const
		{
			float distX = x - vector.x;
			float distY = y - vector.y;
			return distX * distX + distY * distY;
		}
	};
    
    template<class T> std::ostream& operator << (std::ostream& out, const Vector2<T>& vector)
    {
        out<<vector.x<<vector.y;
        return out;
    }
    
    template <class T>
    bool operator==(const Vector2<T> &v1, const Vector2<T> &v2)
    {
        return v1.x == v2.x && v1.y == v2.y;
    }
    
    template <class T>
    bool operator!=(const Vector2<T> &v1, const Vector2<T> &v2)
    {
        return v1.x != v2.x || v1.y != v2.y;
    }
    
    //Vector3
	template<class T> class Vector3;
	template<class T>
	std::ostream& operator <<(std::ostream& out, const Vector3<T>& vector);
	template<class T>
	bool operator==(const Vector3<T> &v1, const Vector3<T> &v2);
	template<class T>
	bool operator!=(const Vector3<T> &v1, const Vector3<T> &v2);

	template <class T> class Vector4;

	template <class T>
	class Vector3
	{
	public:
		friend std::ostream& operator<< <T>(std::ostream& out, const Vector3<T>& vector);
		friend bool operator== <T>(const Vector3<T> &v1, const Vector3<T> &v2);
		friend bool operator!= <T>(const Vector3<T> &v1, const Vector3<T> &v2);

        T x;
        T y;
        T z;
        
        Vector3(T fx = 0, T fy = 0, T fz = 0) : x(fx), y(fy), z(fz){}
        
        Vector3(const Vector2<T> &v, T fz = 0) : x(v.x), y(v.y), z(fz){}
        Vector3(const Vector4<T> &v) : x(v.x), y(v.y), z(v.z){}
        Vector3(const Vector3<T> &from, const Vector3<T> &to) : x(to.x - from.x), y(to.y - from.y), z(to.z - from.z){}
        
        ~Vector3(void){};

		const T operator[](unsigned int i) const
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

		T lenSquared() const
		{
			return x*x+y*y+z*z;
		}

		float len() const
		{
			return sqrtf(x*x+y*y+z*z);
		}

		Vector3 normalize() const
		{
			float length = len();
			if(length>0)
			{
				float inv_length = 1 / length;
				x *= inv_length;
				y *= inv_length;
				z *= inv_length;
			}
			return *this;
		}

		float dist(Vector3 vector) const
		{
			float distX = x - vector.x;
			float distY = y - vector.y;
			float distZ = z - vector.z;
			return sqrt(distX * distX + distY * distY + distZ * distZ);
		}

		T distSquared(Vector3 vector) const
		{
			float distX = x - vector.x;
			float distY = y - vector.y;
			float distZ = z - vector.z;
			return distX * distX + distY * distY + distZ * distZ;
		}
	};

	template<class T>
	std::ostream& operator << (std::ostream& out, const Vector3<T>& vector)
    {
        out<<vector.x<<vector.y<<vector.z;
        return out;
    }
    
    template <class T>
    bool operator==(const Vector3<T> &v1,const  Vector3<T> &v2)
    {
        return v1.x == v2.x && v1.y == v2.y && v1.z != v2.z;
    }
    
    template <class T>
    bool operator!=(const Vector3<T> &v1, const Vector3<T> &v2)
    {
        return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
    }


	//Vector4
	template<class T> class Vector4;
	template<class T>
	std::ostream& operator <<(std::ostream& out, const Vector4<T>& vector);
	template<class T>
	bool operator==(const Vector4<T> &v1, const Vector4<T> &v2);
	template<class T>
	bool operator!=(const Vector4<T> &v1, const Vector4<T> &v2);
	
	template <class T>
	class Vector4
	{
		friend std::ostream& operator << <T>(std::ostream& out, const Vector4<T>& vector);
		friend bool operator== <T>(const Vector4<T> &v1, const Vector4<T> &v2);
		friend bool operator!= <T>(const Vector4<T> &v1, const Vector4<T> &v2);
	public:
		T x;
        T y;
        T z;
		T w;
        
        Vector4(T fx = 0, T fy = 0, T fz = 0, T fw = 1) : x(fx), y(fy), z(fz), w(fw){};
        
        Vector4(const Vector3<T> &v, T fw = 1) : x(v.x), y(v.y), z(v.z), w(fw){};
        
        ~Vector4(void){};

		const T &operator[](unsigned int i) const
		{
			if (i == 0) return x;
			if (i == 1) return y;
			if (i == 2) return z;
			return w;
		}

		T& operator[](unsigned int i)
		{
			if (i == 0) return x;
			if (i == 1) return y;
			if (i == 2) return z;
			return w;
		}

		void operator+=(const Vector4& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
		}


		Vector4 operator+(const Vector4& v) const
		{
			return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
		}

		void operator-=(const Vector4& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
		}

		Vector4 operator-(const Vector4& v) const
		{
			return Vector3(x - v.x, y - v.y, z - v.z, w - v.w);
		}

		void operator*=(const T value)
		{
			x *= value;
			y *= value;
			z *= value;
		}

		T dotMul(const Vector4 & vector) const
		{
			return x * vector.x + y * vector.y + z * vector.z + w * vector.w;
		}

		Vector4 operator*(const T value) const
		{
			return Vector4(x * value, y * value, z * value, w);
		}

		Vector3<T> crossMul(Vector4& vector) const
		{
			return Vector3<T>(y*vector.z - z*vector.y,
						   z*vector.x - x*vector.z,
						   x*vector.y - y*vector.x);
		}

		T operator *(const Vector4 &vector) const
		{
			return x*vector.x + y*vector.y + z*vector.z + w*vector.w;
		}

		void scale(const Vector4 &vector)
		{
			x *= vector.x;
			y *= vector.y;
			z *= vector.z;
			w *= vector.w;
		}

		T lenSquared() const
		{
			return x * x + y * y + z * z + w * w;
		}

		float len() const
		{
			return sqrtf(x * x + y * y + z * z + w * w);
		}

		Vector4 &normalize()
		{
			float len = len();
			if(len>0)
			{
				float inv_length = 1 / len;
				x *= inv_length;
				y *= inv_length;
				z *= inv_length;
				w *= inv_length;
			}
			return *this;
		}

		float dist(Vector4 &vector) const
		{
			float distX = x - vector.x;
			float distY = y - vector.y;
			float distZ = z - vector.z;
			float distW = w - vector.w;
			return sqrt(distX * distX + distY * distY + distZ * distZ + distW * distW);
		}

		T distSquared(Vector4 &vector) const
		{
			float distX = x - vector.x;
			float distY = y - vector.y;
			float distZ = z - vector.z;
			float distW = w - vector.w;
			return distX * distX + distY * distY + distZ * distZ + distW * distW;
		}
	};

	template<class T>
	std::ostream& operator << (std::ostream& out, const Vector4<T>& vector)
    {
        out<<vector.x<<vector.y<<vector.z<<vector.w;
        return out;
    }
    
    template <class T>
    bool operator==(const Vector4<T> &v1, const Vector3<T> &v2)
    {
        return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
    }
    
    template <class T>
    bool operator!=(const Vector4<T> &v1, const Vector3<T> &v2)
    {
        return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z || v1.w != v2.w;
    }


	typedef Vector2<float> vec2f;
	typedef Vector2<int> vec2i;
	typedef Vector3<float> vec3f;
	typedef Vector3<int> vec3i;
	typedef Vector4<float> vec4f;
	typedef Vector4<int> vec4i;

	typedef Vector2<int> Size;
	typedef Vector2<int> Point2i;
	typedef Vector2<float> Point2f;
	typedef Vector3<float> Bound;
	typedef Vector3<float> Point3f;
    typedef Vector2<float> Vector2f;
    typedef Vector3<float> Vector3f;
    typedef Vector4<float> Vector4f;
}

#endif