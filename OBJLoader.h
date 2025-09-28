//
// Created by judelhuu on 8/27/25.
//

#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <deque>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vertex.h"
#include <Material.h>

using namespace glm;




class OBJLoader : public core::Singleton<OBJLoader>
{
public:
    std::vector<Vertex> LoadFromFile(const char* filename, std::vector<Texture*>& textures, std::vector<Material*>& materials)
    {
    // vertex portions
    std::vector<vec3> vertex_position;
    std::vector<vec3> vertex_normal;
    std::vector<vec2> vertex_texcoord;

    // face vectors
    std::vector<GLint> vertex_position_indices;
    std::vector<GLint> vertex_normal_indices;
    std::vector<GLint> vertex_texcoord_indices;

    //vertex array
    std::vector<Vertex> vertices;

    // Meshes
    std::vector<Mesh> meshes;

    std::stringstream stringstream;
    std::ifstream inFile(filename);

    std::string currentLine;
    std::string präfix;

    vec3 tmp_vec3;
    vec2 tmp_vec2;
    GLint tmp_glint = 0;


    if (!inFile.is_open())
    {
        throw std::runtime_error("ERROR::LoadOBJ::Could not open file");
    }

    while (std::getline(inFile, currentLine))
    {
        stringstream.clear();
        stringstream.str(currentLine);
        stringstream >> präfix;

        if (präfix == "#")
        {

        }
        else if (präfix == "o")
        {

        }
        else if (präfix == "s")
        {

        }
        else if (präfix == "mtllib")
        {
            // path + filename
            std::string mtlFile = currentLine;
            mtlFile.erase(mtlFile.begin(), mtlFile.begin()+7);

            LoadMtlFile(std::string(directory_objFiles+mtlFile).c_str(), textures, materials, meshes);
        }
        else if (präfix == "usemtl")
        {
            std::string materialType = currentLine;
            materialType.erase(materialType.begin(), materialType.begin()+7);
            std::cout << "usemtl says: " << materialType << std::endl;
        }
        else if (präfix == "v") // vertex position
        {
            stringstream >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vertex_position.push_back(tmp_vec3);
        }
        else if (präfix == "vt")
        {
            stringstream >> tmp_vec2.x >> tmp_vec2.y;
            vertex_texcoord.push_back(tmp_vec2);
        }
        else if (präfix == "vn") // vertex position
        {
            stringstream >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vertex_normal.push_back(tmp_vec3);
        }
        else if (präfix == "f") // vertex position
        {

            std::deque<GLint> vertex_indices_unsorted;
            std::string line = stringstream.str();

            switch ( int type = GetInputType(line, vertex_indices_unsorted))
            {
                case POSITION:
                {
                    for (auto & index : vertex_indices_unsorted)
                         vertex_position_indices.push_back(index);
                    break;
                }

                case POSITION_TEXCOORD:
                {
                    for (size_t i = 0; i < vertex_indices_unsorted.size(); i += 2)
                    {
                        vertex_position_indices.push_back(vertex_indices_unsorted[i]);
                        vertex_texcoord_indices.push_back(vertex_indices_unsorted[i + 1]);
                    }

                    break;
                }

                case POSITION_NORMAL:
                {
                    for (size_t i = 0; i < vertex_indices_unsorted.size(); i += 2)
                    {
                        vertex_position_indices.push_back(vertex_indices_unsorted[i]);
                        vertex_normal_indices.push_back(vertex_indices_unsorted[i + 1]);
                    }

                    break;
                }

                case POSITION_TEXCOORD_NORMAL:
                {
                    for (int i = 0; i < vertex_indices_unsorted.size(); i+=3)
                    {
                        vertex_position_indices.push_back(vertex_indices_unsorted[i]);
                        vertex_texcoord_indices.push_back(vertex_indices_unsorted[i+1]);
                        vertex_normal_indices.push_back(vertex_indices_unsorted[i+2]);
                    }
                    break;
                }

            }
        }
        vertices.resize(vertex_position_indices.size(), Vertex());


        for (int i = 0; i < vertices.size(); i++)
        {
            vertices[i].color = vec3(1.f,1.f,1.f);

            if (!vertex_position.empty() && !vertex_position_indices.empty())
                vertices[i].position = vertex_position[vertex_position_indices[i]-1];

            if (!vertex_texcoord.empty() && !vertex_texcoord_indices.empty() && i < vertex_texcoord_indices.size())
                    vertices[i].texcoord = vertex_texcoord[vertex_texcoord_indices[i]-1];

            if (!vertex_normal.empty() && !vertex_normal_indices.empty())
                vertices[i].normal = vertex_normal[vertex_normal_indices[i]-1];

        }
    }
    return vertices;
}
private:
    void LoadMtlFile(const char* filename, std::vector<Texture*>& textures, std::vector<Material*>& materials, std::vector<Mesh>& meshes)
    {
        std::string material_name;

        float specular_exponent = -1;

        vec3 material_ambient;
        vec3 material_diffuse;
        vec3 material_specular;

        float optical_density;
        float dissolve; //transparenz

        GLint texture_diffuse = -1;
        GLint texture_specular = -1;

        // meshes
        int nrOfMeshes = 0;

        std::stringstream sStream;
        std::ifstream mtlFile(filename);

        std::string line;
        std::string mtl_präfix;


        if (!mtlFile.is_open())
        {
            throw std::runtime_error("ERROR::LoadOBJ::Could not open file");
        }

        while (std::getline(mtlFile, line))
        {
            sStream.clear();
            sStream.str(line);
            sStream >> mtl_präfix;

            std::cout << line << std::endl;

            if (mtl_präfix == "illum" || mtl_präfix == " ")
                continue;

            if (mtl_präfix == "newmtl")
            {
                material_name = line;
                material_name.erase(material_name.begin(), material_name.begin()+7);
                nrOfMeshes++;
            }
            else if (mtl_präfix == "Ns")
            {
                line.erase(line.begin(), line.begin()+3);
                specular_exponent = std::stof(line);
            }
            else if (mtl_präfix == "Ka")
            {
                sStream >> material_ambient.x >> material_ambient.y >> material_ambient.z;
            }
            else if (mtl_präfix == "Kd")
            {
                sStream >> material_diffuse.x >> material_diffuse.y >> material_diffuse.z;
            }
            else if (mtl_präfix == "Ks")
            {
                sStream >> material_specular.x >> material_specular.y >> material_specular.z;
            }
            else if (mtl_präfix == "Ni")
            {
                line.erase(line.begin(), line.begin()+3);
                specular_exponent = std::stof(line);
            }
            else if (mtl_präfix == "d")
            {
                line.erase(line.begin(), line.begin()+2);
                specular_exponent = std::stof(line);
            }
            else if (mtl_präfix == "map_Kd")
            {

                std::string imageFile = line;
                imageFile.erase(imageFile.begin(), imageFile.begin()+7);

                // umstellung auf dynamisches hinzufügen von Textures & Materials
                // falls probleme auftreten suche hier; zukunftsjan!
                texture_diffuse = textures.size();
                textures.push_back(new Texture(std::string(directory_Images+imageFile).c_str(), texture_diffuse));

                // assuming its ze last one:
                if (texture_specular == -1) // temp
                    texture_specular = texture_diffuse;

                Material* mtl_material = new Material(material_ambient, material_diffuse, material_specular,
                                                      texture_diffuse, texture_specular);
                mtl_material->SetMaterialID(material_name); // id zuweisen

                materials.emplace_back(mtl_material);
            }

            mtl_präfix.clear();


        }

    }

    int GetInputType(std::string line, std::deque<GLint>& values) // either position/texcoord/normal or position/texcord or position//normal
    {
        // get number of sperators
        int nrOfSeperators = std::count(line.begin(), line.end(), '/');

        // get number of integers
        line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

        char* the_int_within = std::strtok(line.data(), "/ ");
        while (the_int_within)
        {
            values.push_back(atoi(the_int_within));
            the_int_within = strtok(NULL, "/ ");
        }

        if (!values.empty())
            values.pop_front();


        int nrOfIntegers = (int)values.size();

        // std::cout << "Ins: " << nrOfIntegers << std::endl;
        // std::cout << "Slashs: " << nrOfSeperators << std::endl;


        // return type (hehe)
        if (nrOfSeperators == 0)
            return POSITION;

        if (nrOfIntegers == nrOfSeperators)
            return POSITION_NORMAL;

        if (nrOfIntegers/nrOfSeperators == 2)
            return POSITION_TEXCOORD;

        if (nrOfIntegers == nrOfSeperators*1.5)
            return POSITION_TEXCOORD_NORMAL;



        throw std::runtime_error("ahhhhh");
        return -1;
    }

    enum OBJFaceType
    {
        POSITION = 0,
        POSITION_TEXCOORD = 1,
        POSITION_NORMAL = 2,
        POSITION_TEXCOORD_NORMAL = 3,
    };

    std::string directory_objFiles = "/home/judelhuu/CLionProjects/OpenGL_3D_Tutorial/OBJ-Files/";
    std::string directory_Images = "/home/judelhuu/CLionProjects/OpenGL_3D_Tutorial/Images";
};

#endif //OBJLOADER_H
