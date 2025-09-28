//
// Created by judelhuu on 8/22/25.
//

#include "../include/Shader.h"

void Shader::Use() const
{
    glUseProgram(m_id);
}

void Shader::Unuse()
{
    glUseProgram(0);
}

void Shader::Set1i(GLint value, const GLchar *name)
{
    Use();
    glUniform1i(glGetUniformLocation(m_id, name),value);
    Unuse();
}

void Shader::Set1f(GLfloat value, const GLchar *name)
{
    Use();
    glUniform1f(glGetUniformLocation(m_id, name), value);
    Unuse();
}

void Shader::SetVec2f(glm::vec2 value, const GLchar *name)
{
    Use();
    glUniform2fv(glGetUniformLocation(m_id, name), 1, value_ptr(value));
    Unuse();
}

void Shader::SetVec3f(glm::vec3 value, const GLchar *name)
{
    Use();
    glUniform3fv(glGetUniformLocation(m_id, name), 1, value_ptr(value));
    Unuse();
}

void Shader::SetVec4f(glm::vec4 value, const GLchar *name)
{
    Use();
    glUniform4fv(glGetUniformLocation(m_id, name), 1, value_ptr(value));
    Unuse();
}

void Shader::SetMat3f(glm::mat3 matrix, const GLchar *name, GLboolean transpose)
{
    Use();
    glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, transpose, glm::value_ptr(matrix));
    Unuse();
}

void Shader::SetMat4f(glm::mat4 matrix, const GLchar *name, GLboolean transpose)
{
    Use();
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, transpose, glm::value_ptr(matrix));
    Unuse();
}
