#ifndef _MATERIAL_FACTORY_H_
#define _MATERIAL_FACTORY_H_

#include <string> // std::string
#include <map>
#include "Material.h"

class MaterialFactory{
public:
    // static
    static MaterialFactory& getInstance();

    // public api
    Material get(std::string type);
    void refresh();

protected:
    // internal sturcture holding all the materials
    std::map<std::string,Material> _materials;

private:
    MaterialFactory();
    MaterialFactory(const MaterialFactory& other);
    void operator=(MaterialFactory const&);
};

#endif
