#ifndef __GPEngine__GPFIleUtil__
#define __GPEngine__GPFIleUtil__

#include <string>
#include <set>

#include "gp.h"
#include "stdafx.h"

namespace GPEngine3D {
    class FileUtil
    {
    public:
        DEFINE_SINGLETON(FileUtil)
        ~FileUtil();
        
        void initWithRootPath(const std::string &rootPath);
        
        void addFileSearchPath(const std::string &filePath);
        
        std::string getFullPathOfFile(const std::string &file);
        
        bool isFileExist(const std::string &file);
        
        bool isAbsolutePath(const std::string &path);
        
    private:
        
        std::set<std::string> searchPaths;
        
        std::string rootPath;
    };
}


#endif /* defined(__GPEngine__GPFIleUtil__) */


