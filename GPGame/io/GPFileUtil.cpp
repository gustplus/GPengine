//
//  GPFileUtil.cpp
//  GPEngine
//
//  Created by shizhan on 14-9-14.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#include "GPFileUtil.h"

#include <stdio.h>

namespace GPEngine3D {
    IMP_SINGLETON(FileUtil)
    
    FileUtil::FileUtil():searchPaths(std::set<std::string>()), rootPath("")
    {
        searchPaths.insert(rootPath);
		TCHAR szPath[MAX_PATH];
		if( GetModuleFileName( NULL, szPath, MAX_PATH ) )
		{
			 printf("GetModuleFileName failed %s\n", szPath);
		}
    }
    
    FileUtil::~FileUtil(){
        
    }
    
    void FileUtil::addFileSearchPath(const std::string &filePath)
    {
        if (isAbsolutePath(filePath))
        {
            this->searchPaths.insert(filePath);
        }else{
            this->searchPaths.insert(rootPath + "/" + filePath);
        }
    }
    
    bool FileUtil::isFileExist(const std::string &path)
    {
#ifdef GP_PLATFORM_WIN32
		FILE *file = nullptr;
		fopen_s(&file, path.c_str(), "r");
#else
		FILE *file = fopen(path.c_str(), "r");
#endif
        if (file) {
            return true;
        }
        return false;
    }
    
    bool FileUtil::isAbsolutePath(const std::string &path)
    {
        return path.c_str()[0] == '/';
    }
    
    std::string FileUtil::getFullPathOfFile(const std::string &file)
    {
        if (isAbsolutePath(file)) {
            return file;
        }
        std::set<std::string>::iterator it = searchPaths.begin();
        for (; it != searchPaths.end(); ++it) {
            std::string path = *it;
            path = path + file;
            if (isFileExist(path)) {
                return path;
            }
        }
        return "";
    }
    
    
   
}

