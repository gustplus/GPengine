#pragma once

#include "GPVertexBuffer.h"
#include <string>
#include <vector>
#include "gpmath.h"
#include "GPVector.h"

namespace GPEngine3D
{
	class ObjLoader
	{
	public:
		ObjLoader(void);
		~ObjLoader(void);

		static bool Load(const char* fileName/*, VertexBuffer *buffer*/);

		static void trimString(std::string &);

		static int SplitString(const std::string &, const std::string &split, std::vector<std::string> &);

		static int GetRealIndex(int index);

		static int ParseToLongInt(const std::string &);

		static void LoadWithStride(std::vector<Point3f> &, int stride);
	};
}
