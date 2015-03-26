#include "MaterialFactory.h"
#include "Material.h"

MaterialFactory::MaterialFactory(){}
MaterialFactory::~MaterialFactory(){}

Material MaterialFactory::get(MATERIAL_E type){
    if( type == MaterialFactory::WOOD){
        Material m;
        return m;
    }

    return Material();
}
