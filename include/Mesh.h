//
// Created by judelhuu on 8/23/25.
//

#ifndef MESH_H
#define MESH_H

#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Vertex.h"
#include "Primitive.h"

#include <iostream>
#include <vector>

using namespace glm;

class Mesh
{
public:
    Mesh(Primitive* primitive, const vec3& position, const vec3& rotation, const vec3& scale)
    {
        // init
        m_origin = m_position = position;
        m_rotation = rotation;
        m_scale = scale;

        // copy vertex data
        m_nrOfVertices = primitive->GetNrOfVertices();
        m_vertexArray = new Vertex[m_nrOfVertices];
        for (int i = 0; i<m_nrOfVertices; i++)
        {
            m_vertexArray[i] = primitive->GetVertices()[i];
        }

        // copy indices
        m_nrOfIndices = primitive->GetNrOfIndices();
        m_indexArray = new GLuint[m_nrOfIndices];
        for (int i = 0; i<m_nrOfIndices; i++)
        {
            m_indexArray[i] = primitive->GetIndices()[i];
        }



        InitBuffers();
    }

    Mesh(const Vertex *vertexArray, const unsigned nrOfVertices, const GLuint *indexArray, const unsigned &nrOfIndices, const vec3& position, const vec3& rotation, const vec3& scale)
    {
        m_position = position;
        m_rotation = rotation;
        m_scale = scale;

        // copy vertex data
        m_nrOfVertices = nrOfVertices;
        m_vertexArray = new Vertex[m_nrOfVertices];
        for (int i = 0; i<m_nrOfVertices; i++)
        {
            m_vertexArray[i] = vertexArray[i];
        }

        // copy indices
        m_nrOfIndices = nrOfIndices;
        m_indexArray = new GLuint[m_nrOfIndices];
        for (int i = 0; i<m_nrOfIndices; i++)
        {
            m_indexArray[i] = indexArray[i];
        }

        InitBuffers();
    }

    ~Mesh()
    {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        if (m_nrOfIndices > 0)
        {
            glDeleteBuffers(1, &m_EBO);
        }
        delete[] m_vertexArray;
        delete[] m_indexArray;
    }

    Mesh(const Mesh& other) // copy
    {
        m_position = other.m_position;
        m_rotation = other.m_rotation;
        m_scale = other.m_scale;

        m_nrOfVertices = other.m_nrOfVertices;
        m_vertexArray = new Vertex[m_nrOfVertices];
        for (int i = 0; i<m_nrOfVertices; i++)
        {
            m_vertexArray[i] = other.m_vertexArray[i];
        }

        m_nrOfIndices = other.m_nrOfIndices;
        m_indexArray = new GLuint[m_nrOfIndices];
        for (int i = 0; i<m_nrOfIndices; i++)
        {
            m_indexArray[i] = other.m_indexArray[i];
        }

        InitBuffers();
    }

    void Update(GLFWwindow* window, bool inputEnabled = true)
    {
    }

    void Render(Shader* shader)
    {
        UpdateModelMatrix(shader);
        UpdateUniforms(shader);
        shader->Use();

        glBindVertexArray(m_VAO); // Bind vertex array object

        if (m_nrOfIndices == 0)
        {
            glDrawArrays(GL_TRIANGLES, 0, m_nrOfVertices);
        }
        else
        {
            glDrawElements(GL_TRIANGLES, m_nrOfIndices, GL_UNSIGNED_INT, 0); // Draw quads
        }

        glBindVertexArray(0);
        glUseProgram(0);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void SetPosition(const vec3& position);
    void SetRotation(const vec3& rotation);

    void SetScaleValue(const vec3& scale);
    void SetScaleIncrement(const vec3& scale);

    void Rotate(const glm::vec3& rotation);
    void Move(const glm::vec3& position);

    vec3& GetPosition()
    {
        return m_position;
    }

    // Experimental
    void FillTextureCoordinates() const
    {
        for (int i = 0; i < m_nrOfVertices; i++)
        {
            m_vertexArray[i].texcoord = glm::vec2(rand()%10+1.f, rand()%10+1.f);
        }
    }

    void FillNormal() const
    {
        for (int i = 0; i < m_nrOfVertices; i++)
        {
            m_vertexArray[i].normal = glm::vec3(rand()%10+1.f, rand()%10+1.f, rand()%10+1.f);
        }
    }




private:

    void UpdateUniforms(Shader* shader) const
    {
        shader->SetMat4f(m_modelMatrix, "modelMatrix");
    }
    void UpdateModelMatrix(Shader* shader)
    {
        m_modelMatrix = glm::mat4(1.f);
        m_modelMatrix = glm::translate(m_modelMatrix, m_origin);
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.x), glm::vec3(1.f, 0.f,0.f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.y), glm::vec3(0.f, 1.f,0.f));
        m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_rotation.z), glm::vec3(1.f, 0.f,1.f));
        m_modelMatrix = glm::translate(m_modelMatrix, m_position - m_origin);
        m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
        shader->SetMat4f(m_modelMatrix,"modelMatrix");
    }

    void InitBuffers()
    {

        // VAO - Vertex Array Object
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        // VBO - Vertex Buffer Object
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_nrOfVertices * sizeof(Vertex), m_vertexArray, GL_STATIC_DRAW);

        // EBO - Element Buffer Object
        if (m_nrOfIndices > 0)
        {
            glGenBuffers(1, &m_EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_nrOfIndices * sizeof(GLuint), m_indexArray, GL_STATIC_DRAW);

        }
        // Set vertex attribute pointers

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

        // Texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
        glEnableVertexAttribArray(2);

        //normal (light)
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(3);

        // Unbind VAO
        glBindVertexArray(0);
    }


    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;

    vec3 m_position;
    vec3 m_origin;
    vec3 m_rotation;
    vec3 m_scale; 
    mat4 m_modelMatrix;

    unsigned m_nrOfVertices;
    unsigned m_nrOfIndices;
    Vertex* m_vertexArray;
    GLuint* m_indexArray;
};



#endif //MESH_H
