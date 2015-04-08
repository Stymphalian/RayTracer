#ifndef MATERIAL_FILE_READER_H_
#define MATERIAL_FILE_READER_H_

#include <map>
#include <string>
#include "../Materials/Material.h"

class MaterialFileReader{
public:
    MaterialFileReader();
    virtual ~MaterialFileReader();

    bool read(const char* filename,std::map<std::string,Material>& output);
    void test();
};

#endif
