//
// Created by judelhuu on 8/22/25.
//

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../GameData.h"

class Shader
{
public:

    Shader(const std::string& vertexFile, const std::string& fragmentFile, const std::string& geometryFile = "")
    {
        GLuint vertexShader = 0;
        GLuint fragmentShader = 0;
        GLuint geometryShader = 0;

        vertexShader = LoadShader(GL_VERTEX_SHADER, vertexFile);
        fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fragmentFile);

        // if (geometryFile != "")
        //     geometryShader = LoadShader(GL_GEOMETRY_SHADER, geometryFile);

        LinkProgram(vertexShader, fragmentShader, geometryShader);

        // cleanup
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(geometryShader);

    }
    ~Shader()
    {
        glDeleteProgram(m_id);
    }


    void Use() const;
    void Unuse();
    void Set1i(GLint value, const GLchar* name);
    void Set1f(GLfloat value, const GLchar* name);
    void SetVec2f(glm::vec2 value, const GLchar* name);
    void SetVec3f(glm::vec3 value, const GLchar* name);
    void SetVec4f(glm::vec4 value, const GLchar* name);
    void SetMat3f(glm::mat3 matrix, const GLchar* name, GLboolean transpose = GL_FALSE);
    void SetMat4f(glm::mat4 matrix, const GLchar* name, GLboolean transpose = GL_FALSE);

private:
    GLuint m_id;

    std::string LoadShaderSource(const std::string& filename)
    {
        std::string tmp = "";
        std::string src = "";

        std::ifstream in_file;

        in_file.open(filename);

        if (in_file.is_open())
        {
            while (std::getline(in_file, tmp))
            {
                src += tmp + "\n";
            }
        }
        else
        {
            std::cerr << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FILE: " << filename << std::endl;
        }

        in_file.close();

        // auto fill the correct version :/
        std::string versionNr
            = std::to_string(_glVersionMajor)
            + std::to_string(_glVersionMinor)
            + "0";
        //std::cout << src.replace(src.find("#version"), 12, "#version " + versionNr) << std::endl;


        return src;
    }

    GLuint LoadShader(GLenum type, const std::string& filename)
    {
        char infoLog[512];
        GLint success;

        GLuint shader = glCreateShader(type);
        std::string src_src = LoadShaderSource(filename); // homo gl
        const GLchar* src = src_src.c_str();
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << filename << std::endl;
            std::cerr << infoLog << std::endl;
            glDeleteShader(shader);
            return false;
        }

        return shader;
    }

    void LinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader)
    {
        char infoLog[512];
        GLint success;

        m_id = glCreateProgram();

        if (vertexShader)
            glAttachShader(m_id, vertexShader);

        if (geometryShader)
            glAttachShader(m_id, geometryShader);

        if (fragmentShader)
            glAttachShader(m_id, fragmentShader);

        glLinkProgram(m_id);

        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_id, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << std::endl;
            std::cerr << infoLog << std::endl;
        }

        glUseProgram(0);
    }

};



#endif //SHADER_H
