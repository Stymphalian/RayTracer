#include <iostream>
#include <vector>
#include "ObjFileReader.h"
#include "linearalgebra.h"


ObjFileReader::ObjFileReader(){}
ObjFileReader::~ObjFileReader(){}

std::vector<std::string> split(const std::string& line, const char* delims, int num_delims,bool keep_blanks = false)
{
    std::vector<std::string> tokens;
    std::string last_token = "";
    for(int i = 0;i < line.size(); ++i)
    {
        char c = line[i];

        // check for delimiters
        boolean flag = false;
        for(int j = 0; j < num_delims; ++j){
            if( c == delims[j]){
                flag = true;
                break;
            }
        }

        if( flag == true){
            // dump the last recorded token
            if( keep_blanks){
                tokens.push_back(last_token);
            }else{
                if( last_token.size() != 0){
                    // only push in non-empty sized tokens
                    tokens.push_back(last_token);
                }
            }
            last_token = "";

            continue;
        }

        // append to the next token
        last_token.push_back(c);
    }

    if(last_token.size() >= 1){
        tokens.push_back(last_token);
    }

    return tokens;
}
void ObjFileReader::parse_face(std::vector<std::string>& tokens,Obj_Model& model)
{
    if(tokens.size() < 2){return;}

    jVec3 vertex_face;
    jVec3 texture_face;
    jVec3 normal_face;
    jVec3 origin_vertex_index = 0;
    jVec3 origin_texture_index = 0;
    jVec3 origin_normal_index = 0;
    std::vector<std::string> parsed_number;
    const char delims[] = {'/'};
    const int delims_len = 1;

    // start from 1 in order to ignore the 'f' token
    bool first_index = true;
    int index = 0;
    for(int i = 1; i < tokens.size(); ++i)
    {
        parsed_number = split(tokens[i],delims,delims_len,true);

        if( parsed_number.size() >= 1){
            vertex_face[index] = std::atoi(parsed_number[0].c_str());
        }
        if( parsed_number.size() >= 2){
            if( parsed_number[1] != ""){
                texture_face[index] = std::atoi(parsed_number[1].c_str());
            }
        }
        if( parsed_number.size() >= 3){
            if( parsed_number[2] != ""){
                normal_face[index] = std::atoi(parsed_number[2].c_str());
            }
        }

        if(first_index){
            origin_vertex_index = vertex_face[0];
            origin_texture_index = texture_face[0];
            origin_normal_index = normal_face[0];
            first_index = false;
        }

        // read in as a triangle fan
        index += 1;
        if( index == 3){
            model.vertex_indices.push_back(vertex_face);
            model.texture_indices.push_back(texture_face);
            model.normal_indices.push_back(normal_face);
            index = 1;
        }
    }
}


void ObjFileReader::read(const char* filename,Obj_Model* model){
    std::ifstream file;
    file.open(filename,ios::in);
    if(file.is_open() == false){return;}

    std::string line;
    const char delims[] = {' ','\t'};
    const int num_delims = 2;

    int line_count = 0;
    while(std::getline(file,line))
    {
        // ignore comment lines
        if( line[0] == '#'){continue;}
        if( line[0] == '\n'){continue;}
        if( line.size() == 0){continue;}

        // process into tokens in order to make parsing easier.
        std::vector<std::string> tokens = split(line,delims,num_delims,false);
        if( tokens.size() < 1){
            continue;
        }

        if( tokens[0] == "v"){
            // handle vertices
            if( tokens.size() < 4){
                cout << "Invalid vertex: " << line_count << std::endl;
                return;
            }

            jVec3 vert;
            vert[0] = std::atof(tokens[1].c_str());
            vert[1] = std::atof(tokens[2].c_str());
            vert[2] = std::atof(tokens[3].c_str());

            // if there is an optional 4th parameter
            // if( tokens.size() >= 5){
            //     vert[3] = std::atof(tokens[4].c_str());
            // }else{
            //     vert[3] = 1.0f;
            // }
            model->vertices.push_back(vert);

        }else if( tokens[0] == "vt"){
            if( tokens.size() < 3){
                cout << "Invalid vertex texture: " << line_count << std::endl;
                return;
            }
            // handle texture coordinates
            jVec3 tex;
            tex[0] = std::atof(tokens[1].c_str());
            tex[1] = std::atof(tokens[2].c_str());

            // look for optional 3rd texture coordinate
            if( tokens.size() >= 4){
                tex[2] = std::atof(tokens[3].c_str());
            }else{
                tex[2] = 0.0f;
            }
            model->textures.push_back(tex);

        }else if( tokens[0] == "vn"){
            // handle all the normals
            if( tokens.size() < 4){
                cout << "Invalid normal coordinate : " << line_count << std::endl;
                return;
            }
            jVec3 normal;
            normal[0] = std::atof(tokens[1].c_str());
            normal[1] = std::atof(tokens[2].c_str());
            normal[2] = std::atof(tokens[3].c_str());
            normal.normalize();
            model->normals.push_back(normal);

        }else if( tokens[0] == "f"){
            parse_face(tokens,*model);
        }else{
            // ignore an un-handled case
        }


        line_count += 1;
    }

    file.close();
}

void ObjFileReader::test()
{
    std::cout << "Testing split" << std::endl;
    const char delims_split[]= {' ','\t'};
    std::string test_split_string = "Hello Jordan  is here to help";
    std::vector<std::string> split_rs = split(test_split_string,delims_split,2,true);
    const std::string desired_split[]= {"Hello","Jordan","","is","here","to","help"};
    for(int i = 0; i < split_rs.size(); ++i){
        if( split_rs[i] != desired_split[i]){
            std::cout << "Bad Input" << std::endl;
        }
    }
    std::cout << "Finish Testing split" << std::endl;

    std::cout << "Testing obj read" << std::endl;
    ObjFileReader::Obj_Model model;
    read("icosahedron.obj",&model);
    for(int i = 0; i < model.vertices.size(); ++i){
        std::cout << "v " << model.vertices[i][0] << " " << model.vertices[i][1] << " " << model.vertices[i][2] << std::endl;
    }
    for(int i = 0; i < model.textures.size(); ++i){
        std::cout << "vt " << model.textures[i][0] << " " << model.textures[i][1] << " " << model.textures[i][2] << std::endl;
    }
    for(int i = 0; i < model.normals.size(); ++i){
        std::cout << "vn " << model.normals[i][0] << " " << model.normals[i][1] << " " << model.normals[i][2] << std::endl;
    }
    for(int i = 0; i < model.vertex_indices.size(); ++i){
        std::cout << "f " << model.vertex_indices[i][0] << " " << model.vertex_indices[i][1] << " " << model.vertex_indices[i][2] << std::endl;
    }
    for(int i = 0; i < model.texture_indices.size(); ++i){
        std::cout << "ft " << model.texture_indices[i][0] << " " << model.texture_indices[i][1] << " " << model.texture_indices[i][2] << std::endl;
    }
    for(int i = 0; i < model.normal_indices.size(); ++i){
        std::cout << "fn " << model.normal_indices[i][0] << " " << model.normal_indices[i][1] << " " << model.normal_indices[i][2] << std::endl;
    }
    std::cout << "Finish obj read" << std::endl;


    // test the split
    // test a sample file
}
