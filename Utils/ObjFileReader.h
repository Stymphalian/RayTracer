#ifndef OBJ_FILE_READER_H_
#define OBJ_FILE_READER_H_

#include "linearalgebra.h"
#include <string>
#include <vector>

class ObjFileReader {
public:
    class Obj_Model {
    public:
        // std::vector<jVec4> vertices;
        std::vector<jVec3> vertices;
        std::vector<jVec3> textures;
        std::vector<jVec3> normals;

        std::vector<jVec3> vertex_indices;
        std::vector<jVec3> texture_indices;
        std::vector<jVec3> normal_indices;
    };

    ObjFileReader();
    virtual ~ObjFileReader();

    // the filename of the obj to read.
    // model is the model to fill out.
    void read(const char* filename,Obj_Model* model);
    void parse_face(std::vector<std::string>& tokens,Obj_Model& model);
    void test();
};

#endif
