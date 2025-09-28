//
// Created by judelhuu on 8/23/25.
//

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Vertex.h"

#include <iostream>
#include <vector>
#include <string>


class Primitive {
public:
    Primitive()= default;
    ~Primitive()= default;

    void Set(const Vertex* vertexArray, const unsigned nrOfVertices, const GLuint* indexArray, const unsigned& nrOfIndices)
    {
       m_vertices.assign(vertexArray, vertexArray+nrOfVertices);
       m_indices.assign(indexArray, indexArray+nrOfIndices);
    }

    Vertex* GetVertices(){return m_vertices.data();}
    GLuint* GetIndices(){return m_indices.data();}
    unsigned GetNrOfVertices() const {return m_vertices.size();}
    unsigned GetNrOfIndices()  const {return m_indices.size();}


private:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
};

struct  Quad : public Primitive
{
    Quad()
    {
        Vertex vertices[] =
        {
            // position                       // color                        //texccords         //normals
            glm::vec3(-0.5f, 0.5f, 0.f),      glm::vec3(1.0f, 0.f, 0.f),     glm::vec2(0.f, 1.f), glm::vec3(0.0f, 0.f, 1.f),
            glm::vec3(-0.5f, -0.5f, 0.f),     glm::vec3(0.0f, 1.f, 0.f),     glm::vec2(0.f, 0.f), glm::vec3(0.0f, 0.f, 1.f),
            glm::vec3(0.5f, -0.5f, 0.f),      glm::vec3(0.0f, 0.f, 1.f),     glm::vec2(1.f, 0.f), glm::vec3(0.0f, 0.f, 1.f),

            glm::vec3(-0.5f, 0.5f, 0.f),      glm::vec3(1.0f, 0.f, 0.f),     glm::vec2(0.f, 1.f), glm::vec3(0.0f, 0.f, 1.f),
            glm::vec3(0.5f, -0.5f, 0.f),      glm::vec3(0.0f, 0.f, 1.f),     glm::vec2(1.f, 0.f), glm::vec3(0.0f, 0.f, 1.f),
            glm::vec3(0.5f, 0.5f, 0.f),       glm::vec3(1.0f, 1.f, 0.f),     glm::vec2(1.f, 1.f), glm::vec3(0.0f, 0.f, 1.f)
        };

        GLuint indices[] =
        {
            0,1,2,  // t1
            3,4,5   // t2
        };

        unsigned nrOfVerticies = sizeof(vertices)/sizeof(Vertex);
        unsigned nrOfIndices= sizeof(indices)/sizeof(GLuint);

        Set(vertices, nrOfVerticies, indices, nrOfIndices);
    }
};


struct Triangle : public Primitive
{
    Triangle()
    {
        Vertex vertices[] =
        {
            // position                       // color                        //texccords         //normals
            glm::vec3(-0.5f, 0.5f, 0.f),      glm::vec3(1.0f, 0.f, 0.f),     glm::vec2(0.f, 1.f), glm::vec3(0.0f, 0.f, 1.f),
            glm::vec3(-0.5f, -0.5f, 0.f),     glm::vec3(0.0f, 1.f, 0.f),     glm::vec2(0.f, 0.f), glm::vec3(0.0f, 0.f, 1.f),
            glm::vec3(0.5f, -0.5f, 0.f),      glm::vec3(0.0f, 0.f, 1.f),     glm::vec2(1.f, 0.f), glm::vec3(0.0f, 0.f, 1.f),

            glm::vec3(-0.5f, 0.5f, 0.f),      glm::vec3(1.0f, 0.f, 0.f),     glm::vec2(0.f, 1.f), glm::vec3(0.0f, 0.f, 1.f),
            glm::vec3(0.5f, -0.5f, 0.f),      glm::vec3(0.0f, 0.f, 1.f),     glm::vec2(1.f, 0.f), glm::vec3(0.0f, 0.f, 1.f),
            glm::vec3(0.5f, 0.5f, 0.f),       glm::vec3(1.0f, 1.f, 0.f),     glm::vec2(1.f, 1.f), glm::vec3(0.0f, 0.f, 1.f)
        };

        GLuint indices[] =
        {
            0,1,2,  // t1
        };

        unsigned nrOfVerticies = sizeof(vertices)/sizeof(Vertex);
        unsigned nrOfIndices= sizeof(indices)/sizeof(GLuint);

        Set(vertices, nrOfVerticies, indices, nrOfIndices);
    }

};

struct Pyramid : public Primitive
{
    Pyramid()
    {
        Vertex vertices[] =
        {
            // front                          // color                        //texccords           //normals
            glm::vec3(0.f, 0.5f, 0.f),         glm::vec3(1.0f, 0.f, 0.f),     glm::vec2(0.5f, 1.f), glm::vec3(0.0f, 0.f, 1.f),
            glm::vec3(-0.5f, -0.5f, 0.5f),     glm::vec3(0.0f, 1.f, 0.f),     glm::vec2(0.f, 0.f),  glm::vec3(0.0f, 0.f, 1.f),
            glm::vec3(0.5f, -0.5f, 0.5f),      glm::vec3(0.0f, 0.f, 1.f),     glm::vec2(1.f, 0.f),  glm::vec3(0.0f, 0.f, 1.f),

            // left
            glm::vec3(0.f, 0.5f, 0.f),           glm::vec3(1.0f, 1.f, 0.f),     glm::vec2(0.5f, 1.f), glm::vec3(-1.0f, 0.f, 0.f),
            glm::vec3(-0.5f, -0.5f, -0.5f),      glm::vec3(0.0f, 0.f, 1.f),     glm::vec2(0.f, 0.f),  glm::vec3(-1.0f, 0.f, 0.f),
            glm::vec3(-0.5f, -0.5f, 0.5f),       glm::vec3(0.0f, 0.f, 1.f),     glm::vec2(1.f, 0.f),  glm::vec3(-1.0f, 0.f, 0.f),

            // back
            glm::vec3(0.f, 0.5f, 0.f),          glm::vec3(1.0f, 1.f, 0.f),     glm::vec2(0.5f, 1.f), glm::vec3(0.0f, 0.f, -1.f),
            glm::vec3(0.5f, -0.5f, -0.5f),      glm::vec3(0.0f, 0.f, 1.f),     glm::vec2(0.f, 0.f),  glm::vec3(0.0f, 0.f, -1.f),
            glm::vec3(-0.5f, -0.5f, -0.5f),     glm::vec3(0.0f, 0.f, 1.f),     glm::vec2(1.f, 0.f),  glm::vec3(0.0f, 0.f, -1.f),

            // right;
            glm::vec3(0.f, 0.5f, 0.f),         glm::vec3(1.0f, 1.f, 0.f),     glm::vec2(0.5f, 1.f), glm::vec3(1.0f, 0.f, 0.f),
            glm::vec3(0.5f, -0.5f, 0.5f),      glm::vec3(0.0f, 0.f, 1.f),     glm::vec2(0.f, 0.f),  glm::vec3(1.0f, 0.f, 0.f),
            glm::vec3(0.5f, -0.5f, -0.5f),      glm::vec3(0.0f, 0.f, 1.f),    glm::vec2(1.f, 0.f),   glm::vec3(1.0f, 0.f, 0.f)
        };

        unsigned nrOfVerticies = sizeof(vertices)/sizeof(Vertex);

        Set(vertices, nrOfVerticies, nullptr, NULL);
    }

};



#endif //PRIMITIVE_H
