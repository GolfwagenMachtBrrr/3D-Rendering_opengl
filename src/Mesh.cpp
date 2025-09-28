//
// Created by judelhuu on 8/23/25.
//

#include "../include/Mesh.h"


void Mesh::SetPosition(const vec3& position)
{m_position = position;}

void Mesh::SetRotation(const vec3& rotation)
{m_rotation = rotation; }

void Mesh::SetScaleValue(const vec3& scale)
{m_scale = scale;}

void Mesh::SetScaleIncrement(const vec3 &scale)
{
    m_scale += scale;
}

void Mesh::Rotate(const glm::vec3 &rotation)
{
    m_rotation += rotation;
}

void Mesh::Move(const glm::vec3 &position)
{
    m_position += position;
}
