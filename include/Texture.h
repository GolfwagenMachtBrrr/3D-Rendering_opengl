//
// Created by judelhuu on 8/22/25.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>


class Texture
{
public:
    Texture(const char* filename, const GLenum type)
    {
        m_type = type;
        unsigned char* image  = SOIL_load_image(filename, &m_width, &m_height, nullptr, SOIL_LOAD_RGBA);

        glGenTextures(1, &m_textureID);
        glBindTexture(type,m_textureID);

        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(type, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(type);

        glActiveTexture(0);
        glBindTexture(type, 0);
        SOIL_free_image_data(image);

    }

    ~Texture()
    {
        glDeleteTextures(1,&m_textureID);
    }

    GLuint GetTextureID() const
    {
        return m_textureID;
    }

    void SetID(const char* ID) {
        m_ID = ID;
    }

    const char* GetID() {
        return  m_ID;
    }


    void Bind(const GLint textureUnit)
    {
        glActiveTexture(GL_TEXTURE0+textureUnit);
        glBindTexture(m_type, m_textureID);
    }

    void Unbind()
    {
        glActiveTexture(0);
        glBindTexture(m_type, 0);
    }

    void LoadFromFile(const char* filename)
    {
        if (m_textureID)
        {
            glDeleteTextures(1, &m_textureID);
        }

        unsigned char* image  = SOIL_load_image(filename, &m_width, &m_height, nullptr, SOIL_LOAD_RGBA);

        glGenTextures(1, &m_textureID);
        glBindTexture(m_type,m_textureID);

        glTexParameteri(m_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(m_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(m_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(m_type, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(m_type);

        glActiveTexture(0);
        glBindTexture(m_type, 0);
        SOIL_free_image_data(image);
    }


private:
    GLuint m_textureID;
    int m_height, m_width;

    unsigned int m_type;
    const char* m_ID;
};



#endif //TEXTURE_H
