#include "MaterialFactory.h"
#include "Material.h"

MaterialFactory::MaterialFactory(){}
MaterialFactory::~MaterialFactory(){}

Material MaterialFactory::get(MATERIAL_E type){
    // void set(jVec3 color,
    //      jVec3 ambient,jVec3 diffuse, jVec3 specular,
    //      float shininess,float reflection,float refraction,float refractionIndex);

    Material m;
    switch(type){
        case(MaterialFactory::WOOD):
            m.set(jVec3(0.6,0,0),
                jVec3(0.1,0.1,0.1),jVec3(0.65,0.65,0.65),jVec3(0.4,0.4,0.4),
                24.0,0.3,
                0.0,0.0,jVec3(0,0,0));
            break;

        case(MaterialFactory::PINE):
            m.set(jVec3(0,0.68,0.14),
                jVec3(0.1,0.1,0.1),jVec3(0.65,0.65,0.65), jVec3(0.4,0.4,0.4),
                16.0,0.0,
                0.0,0.0,jVec3(0,0,0));
            break;

        case(MaterialFactory::CHARCOAL):
            m.set(jVec3(0.12,0.05,0.08),
                jVec3(0.1,0.1,0.1),jVec3(0.65,0.65,0.65), jVec3(0.4,0.4,0.4),
                0.0,0.0,
                0.0,0.0,jVec3(0,0,0));
            break;

        case(MaterialFactory::BRONZE):
            m.set(jVec3(0.8,0.8,0.8),
                jVec3(0.1,0.1,0.1),jVec3(0.65,0.65,0.65), jVec3(0.4,0.4,0.4),
                32.0,0.78,
                // 32.0,0.0,
                0.0,0.0,jVec3(0,0,0));
            break;

        case(MaterialFactory::SILVER):
            m.set(jVec3(0,0,0),
                jVec3(0.05,0.05,0.05),jVec3(0.1,0.1,0.1), jVec3(0.6,0.6,0.6),
                32.0,0.4,
                0.0,0.0,jVec3(0,0,0));
            break;

        case(MaterialFactory::GLASS):
            m.set(jVec3(0,0.08,0.12),
                jVec3(0.05,0.05,0.05),jVec3(0.-5,0.05,0.05), jVec3(0,0,0),
                3.0,0.28,
                0.8,1.40,jVec3(0.4,0.4,0.3));
            break;

        case(MaterialFactory::WHITE_LIGHT):
            m.set(jVec3(1.0,1.0,1.0),
                jVec3(0.1,0.1,0.1),jVec3(0.1,0.1,0.1), jVec3(0.8,0.2,0.3),
                0.3,0.0,
                0.0,0.0,jVec3(0,0,0));
            break;
        default:
            break;
    }
    return m;
}
