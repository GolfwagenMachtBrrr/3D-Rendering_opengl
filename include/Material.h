//
// Created by judelhuu on 8/23/25.
//

#ifndef MATERIAL_H
#define MATERIAL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "Shader.h"
#include "Texture.h"

using namespace glm;

class Material {
public:
    Material(const vec3 ambient, const vec3 diffuse, const vec3 specular, const GLint diffuseTexture, const GLint specularTexture)
    {
        m_ambient = ambient;
        m_diffuse = diffuse;
        m_specular= specular;
        m_diffuseTexture = diffuseTexture;
        m_specularTexture = specularTexture;
    }

    Material()
    {

    }

    ~Material()
    {

    }

    void SetID(std::string& ID)
    {
        m_ID = ID;
    }
    std::string& GetID()
    {
        return m_ID;
    }


    void PrintData() // debug
    {
        std::cout << "Ambient: " << std::endl;
        PrintVec3(m_ambient);

        std::cout << "diffuse: " << std::endl;
        PrintVec3(m_diffuse);

        std::cout << "specular: " << std::endl;
        PrintVec3(m_specular);

        std::cout << "Texture IDs" << std::endl;
        std::cout << "diffuseText: " << m_diffuseTexture << std::endl;
        std::cout << "specularText: " << m_specularTexture << std::endl;
    }

    void PrintVec3(const glm::vec3& vec)
    {
        std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
    }

    void SendToShader(Shader& program)
    {
        program.SetVec3f(m_ambient, "material.ambient");
        program.SetVec3f(m_diffuse, "material.diffuse");
        program.SetVec3f(m_specular, "material.specular");

        program.Set1i(m_diffuseTexture, "material.diffuseTexture");
        program.Set1i(m_specularTexture, "material.specularTexture");
    }

    void SetBoundTextureID (std::string& boundTexID) {
        m_BoundTextureID = boundTexID;
    }

    std::string& GetBoundTextureID() {
        return m_BoundTextureID;
    }

private:
    std::string m_ID;
    std::string m_BoundTextureID;

    vec3 m_ambient;
    vec3 m_diffuse;
    vec3 m_specular;

    GLint m_diffuseTexture;
    GLint m_specularTexture;
};



#endif //MATERIAL_H
