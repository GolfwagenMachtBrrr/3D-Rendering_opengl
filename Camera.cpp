//
// Created by judelhuu on 8/23/25.
//

#include "Camera.h"
glm::mat4 & Camera::GetViewMatrix() {

    this->UpdateCameraVector();
    m_viewMatrix = glm::lookAt(m_position, m_position+m_front, m_up);

    return m_viewMatrix;
}
glm::vec3 & Camera::GetPosition() {return m_position;}
glm::vec3 & Camera::GetWorldUpDir() {return m_worldUp;}


void Camera::SetPositon(const glm::vec3 &position)
{m_position = position;}

void Camera::SetWorldUpDir(const glm::vec3 &worldUp)
{m_worldUp = worldUp;}

