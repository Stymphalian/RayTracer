#ifndef _MATERIAL_FACTORY_H_
#define _MATERIAL_FACTORY_H_

#include "Material.h"

class MaterialFactory{
public:
    enum MATERIAL_E {
        WOOD = 0,
        PINE,
        CHARCOAL,
        BRONZE,
        SILVER,
        GLASS,
        WHITE_LIGHT,
        NUM_MATERIALS
    };

    MaterialFactory();
    virtual ~MaterialFactory();
    Material get(MATERIAL_E type);
};

#endif
