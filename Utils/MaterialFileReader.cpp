#include <map>
#include <string>
#include <stdio.h> // sscanf
#include <iostream> // ifstream
#include "MaterialFileReader.h"

MaterialFileReader::MaterialFileReader(){}
MaterialFileReader::~MaterialFileReader(){}

bool MaterialFileReader::read(const char* filename,std::map<std::string,Material>& output){
    std::ifstream fin;
    fin.open(filename,ios::in);
    if(!fin.is_open()){
        std::cout << "Failed to open materials file: '" << filename << "'" << std::endl;
        return false;
    }

    std::string line;
    while(std::getline(fin,line))
    {
        if(line.size() == 0){continue;}
        if(line[0] == '#'){continue;}
        if(line[0] == '\n'){continue;}

        Material m;
        char material_name[50];

        sscanf(line.c_str(),"%[^,],%lf,%lf,%lf, %lf,%lf,%lf, %lf,%lf,%lf, %lf,%lf,%lf, %f, %f, %f,%f, %lf,%lf,%lf",
            material_name,
            &m.color[0],&m.color[1],&m.color[2],
            &m.ambient[0],&m.ambient[1],&m.ambient[2],
            &m.diffuse[0],&m.diffuse[1],&m.diffuse[2],
            &m.specular[0],&m.specular[1],&m.specular[2],

            &m.shininess,
            &m.reflection,
            &m.refraction,&m.refractionIndex,
            &m.refractionAttenuation[0],&m.refractionAttenuation[1],&m.refractionAttenuation[2]
            );

        std::string mat_name(material_name);
        if( output.count(mat_name) != 0){
            std::cout << "Conflict for material name '" << mat_name << "'" << std::endl;
        }
        output[mat_name] = m;
    }
    fin.close();
    return true;
}

void MaterialFileReader::test(){
    std::map<std::string,Material> output;
    read("test.csv",output);

    std::map<std::string,Material>::iterator it;
    for(it = output.begin();it != output.end(); ++it){
        printf("%s,%lf,%lf,%lf, %lf,%lf,%lf, %lf,%lf,%lf, %lf,%lf,%lf, %f, %f, %f,%f, %lf,%lf,%lf\n",
            it->first.c_str(),
            it->second.color[0],it->second.color[1],it->second.color[2],
            it->second.ambient[0],it->second.ambient[1],it->second.ambient[2],
            it->second.diffuse[0],it->second.diffuse[1],it->second.diffuse[2],
            it->second.specular[0],it->second.specular[1],it->second.specular[2],

            it->second.shininess,
            it->second.reflection,
            it->second.refraction,it->second.refractionIndex,
            it->second.refractionAttenuation[0],it->second.refractionAttenuation[1],it->second.refractionAttenuation[2]
        );
    }
}
