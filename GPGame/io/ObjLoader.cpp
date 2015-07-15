#include "stdafx.h"
#include "ObjLoader.h"
#include "gp.h"
#include <sstream>
#include <iostream>
#include <fstream>

using std::ifstream;
using std::cout;
using std::string;
using std::vector;

US_NS_GP;

ObjLoader::ObjLoader(void)
{
}


ObjLoader::~ObjLoader(void)
{
}

bool ObjLoader::Load(const char* fileName/*, VertexBuffer *buffer*/)
{
	ifstream inFile(fileName, std::ios::in);
	if(!inFile){
		cout<<"can't open file"<<std::endl;
		return false;
	}

	vector<Point3f> points;
	vector<Point2f> texcoords;
	vector<Point3f> normals;
	vector<int> verIndice;
	vector<int> texcIndice;
	vector<int> normalIndice;;
	int attribNum = 0;

	int face = 0;

	string line;
	string symbol;
	while(!inFile.eof())
	{
		std::getline(inFile,line);
		trimString(line);
		if(line.length()>0 && line.at(0) != '#')
		{
			std::istringstream stringStream(line);
			stringStream>>symbol;

			if("v" == symbol){
				float x, y, z;
				stringStream>>x>>y>>z;
				points.push_back(Point3f(x, y, z));
			}

			if("vt" == symbol){
				float u, v;
				stringStream>>u>>v;
				texcoords.push_back(Point2f(u, v));
			}

			if("vn" == symbol){
				float x, y, z;
				stringStream>>x>>y>>z;
				normals.push_back(Point3f(x, y, z));
			}
			if("f" == symbol){
				++face;
				//size_t slash0, lsash1;
				while(stringStream.good())
				{
					int pIndex = -1, nIndex = -1 , tcIndex = -1;
					string value;
					stringStream>>value;
					vector<string> result;
					int num = SplitString(value, "/", result);
					if(attribNum == 0)
					{
						attribNum = num;
					}
					if(num != attribNum)
					{
						return false;
					}
					switch(attribNum)
					{
					case 3:
						nIndex = GetRealIndex(ParseToLongInt(result[2]));
					case 2:
						tcIndex = GetRealIndex(ParseToLongInt(result[1]));
					case 1:
						pIndex = GetRealIndex(ParseToLongInt(result[0]));
					}

					if(pIndex != -1)
						verIndice.push_back(pIndex);
					else
						return false;
					if(tcIndex != -1)
						texcIndice.push_back(tcIndex);
					else
						return false;
					if(nIndex != -1)
						normalIndice.push_back(nIndex);
					else
						return false;
				}
			}
		}
	}

	float *normalBuffer;
	float *texcoordBuffer;
	float *vetexBuffer;
	size_t size;
	switch(attribNum)
	{
	case 3:
		normalBuffer = GP_NEW_ARRAY(float, 3*face*3);
		size = normalIndice.size();
		for(size_t i = 0;i < size; ++i)
		{
			int offset = 3*i;
			normalBuffer[offset] = normals[normalIndice[i]].x;
			normalBuffer[offset+1] = normals[normalIndice[i]].y;
			normalBuffer[offset+2] = normals[normalIndice[i]].z;
		}
            GP_DELETE_ARRAY(normalBuffer);
	case 2:
		texcoordBuffer = GP_NEW_ARRAY(float, 3*face*2);
		size = texcIndice.size();
		for(int i = 0;i < size; ++i)
		{
			int offset = 2*i;
			texcoordBuffer[offset] = texcoords[texcIndice[i]].x;
			texcoordBuffer[offset+1] = texcoords[texcIndice[i]].y;
		}
        GP_DELETE_ARRAY(texcoordBuffer);
	case 1:
		vetexBuffer = GP_NEW_ARRAY(float, 3*face*3);
		size = verIndice.size();
		for(int i = 0;i < size; ++i)
		{
			int offset = 3*i;
			vetexBuffer[offset] = points[verIndice[i]].x;
			vetexBuffer[offset+1] = points[verIndice[i]].y;
			vetexBuffer[offset+2] = points[verIndice[i]].z;
		}
        GP_DELETE_ARRAY(vetexBuffer);
	}
    return true;
}

void ObjLoader::trimString(string &str) {
	const char * whiteSpace = " \t\n\r";
	size_t location;
	location = str.find_first_not_of(whiteSpace);
	str.erase(0,location);
	location = str.find_last_not_of(whiteSpace);
	str.erase(location + 1);
}

int ObjLoader::SplitString(const string &value, const string &split, vector<string> &result)
{
	result.clear();
	int size = 0;
	size_t pos;
	size_t offset = 0;
	size_t length = 0;
	pos = value.find(split);
	while( string::npos != pos )
	{
		length = pos - offset;
		result.push_back(value.substr(offset, length));
		offset = ++pos;
		pos = value.find(split, offset);
		++size;
	}
	++size;
	result.push_back(value.substr(offset, value.size()));
	return size;
}

int ObjLoader::GetRealIndex(int index)
{
	return --index;
}

int  ObjLoader::ParseToLongInt(const std::string &value)
{
	return atoi(value.c_str());
}

void ObjLoader::LoadWithStride(std::vector<Point3f> & arr, int stride)
{

}
