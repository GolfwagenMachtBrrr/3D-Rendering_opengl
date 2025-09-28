//
// Created by judelhuu on 8/23/25.
//

#ifndef CAMERA_H
#define CAMERA_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "GameData.h"

enum Directions
{
    FORWARD = 0,
    BACKWARD,
    LEFT,
    RIGHT
};

struct CameraData : public GameData::core::Singleton<CameraData>
{

};


//https://learnopengl.com/Getting-started/Camera
class Camera : public GameData::core::Singleton<Camera>
{
public:
     void Initialize(const glm::vec3& position, const glm::vec3& worldUp)
     {
         m_viewMatrix = glm::mat4(1.f);

         m_position = position;
         m_worldUp = worldUp;

         m_right = glm::vec4(0.f);
         m_up = worldUp;

         m_pitch = 0.f;
         m_yaw = -90.f;
         m_roll = 0.f;

         this->UpdateCameraVector();
     }

    void Update(const int direction)
    {
         this->Move(direction);
         this->UpdateMouseInput();
    }

    void UpdateCameraVector()
    {
         m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
         m_front.y = sin(glm::radians(m_pitch));
         m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

         m_front = glm::normalize(m_front);

         m_right = glm::normalize(glm::cross(m_front, m_worldUp));
         m_up = glm::normalize(glm::cross(m_right, m_front));

    }

 private:
    void Move(const int direction)
    {
        switch (direction)
        {
            case FORWARD:
                m_position += m_front * movementSpeed * DeltaTime::GetData().dt;
                break;
            case BACKWARD:
                m_position -= m_front * movementSpeed * DeltaTime::GetData().dt;
                break;
            case LEFT:
                m_position -= m_right * movementSpeed * DeltaTime::GetData().dt;
                break;
            case RIGHT:
                m_position += m_right * movementSpeed * DeltaTime::GetData().dt;
                break;
            default:
                break;
        }
    }

    void UpdateMouseInput()
    {
        // Update pitch, yaw, roll
        m_yaw += static_cast<GLfloat>(MouseInput::GetData().mouseOffsetX * sensitivity * DeltaTime::GetData().dt);
        m_pitch -= static_cast<GLfloat>(MouseInput::GetData().mouseOffsetY * sensitivity * DeltaTime::GetData().dt);


        if (m_pitch > 80.f)
        {
            m_pitch = 80;
        }

        if (m_pitch < -80.f)
        {
            m_pitch = -80.f;
        }

        if (m_yaw > 360 || m_yaw < -360)
        {
            m_yaw = 0;
        }
    }

    void UpdateInput(const int direction)
    {
        this->Move(direction);
        this->UpdateMouseInput();
    }

    glm::mat4 m_viewMatrix;

    glm::vec3 m_worldUp;
    glm::vec3 m_position;

    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;


    GLfloat movementSpeed = 1.f;
    GLfloat sensitivity = 4.f;

    GLfloat m_pitch = 0;
    GLfloat m_yaw = -90;
    GLfloat m_roll = 0;


public:
    Camera() {}
    ~Camera() {}


    glm::mat4& GetViewMatrix();

    glm::vec3& GetPosition();
    glm::vec3& GetWorldUpDir();
    //glm::vec3& GetCameraFront();

    void SetPositon(const glm::vec3& position);
    void SetWorldUpDir(const glm::vec3& worldUp);
    //void SetCameraFront(const glm::vec3& direction);
};



#endif //CAMERA_H
