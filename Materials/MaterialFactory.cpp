#include <map>
#include <string>
#include "MaterialFactory.h"
#include "../Utils/MaterialFileReader.h"
#include "Material.h"

static const char* MATERIALS_FILENAME = "data/materials.csv";

MaterialFactory::MaterialFactory(){
    refresh();
}

MaterialFactory& MaterialFactory::getInstance(){
    static MaterialFactory instance;
    return instance;
}

void MaterialFactory::refresh(){
    _materials.clear();
    MaterialFileReader reader;
    reader.read(MATERIALS_FILENAME,_materials);
}

Material MaterialFactory::get(std::string type){
    return _materials[type];
}
