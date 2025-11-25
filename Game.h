//
// Created by judelhuu on 8/23/25.
//

#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>


#include "Game.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "GameData.h"
#include "Camera.h"
#include "Model.h"
#include "OBJLoader.h"




class Game : public core::Singleton<Game>
{
public:
    void Initialize()
    {
        InitGLFW();
        m_window = CreateWindow();
        InitGLEW();
        InitShader();
        InitTexture();
        InitMaterial();
        InitProjectionMatrix();
        InitModels();
        InitOBJModels();
    }
    void Run()
    {
        while (!glfwWindowShouldClose(m_window))
        {
            // Poll events
            glfwPollEvents();

            this->Update();
            this->Render();

            glBindVertexArray(0);
            glUseProgram(0);
            glActiveTexture(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

private:
    void UpdateKeyboardInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            Camera::GetData().Update(FORWARD);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            Camera::GetData().Update(BACKWARD);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            Camera::GetData().Update(LEFT);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            Camera::GetData().Update(RIGHT);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            Camera::GetData().GetPosition().y += 0.005f;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
           Camera::GetData().GetPosition().y -= 0.005f;
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            Camera::GetData().GetPosition().z += 0.005f;
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            Camera::GetData().GetPosition().z -= 0.005f;
        }
    }
    void UpdateMouseInput(GLFWwindow* window)
    {
        glfwGetCursorPos(m_window, &MouseInput::GetData().mouseX, &MouseInput::GetData().mouseY);

        if (MouseInput::GetData().firstMouse)
        {
            MouseInput::GetData().lastMouseX = MouseInput::GetData().mouseX;
            MouseInput::GetData().lastMouseY = MouseInput::GetData().mouseY;
            MouseInput::GetData().firstMouse = false;
        }

        // calculate offset
        MouseInput::GetData().mouseOffsetX = MouseInput::GetData().mouseX - MouseInput::GetData().lastMouseX;
        MouseInput::GetData().mouseOffsetY = MouseInput::GetData().mouseY - MouseInput::GetData().lastMouseY;

        // set last x,y
        MouseInput::GetData().lastMouseX = MouseInput::GetData().mouseX;
        MouseInput::GetData().lastMouseY = MouseInput::GetData().mouseY;
    }
    void UpdateUniforms()
    {
        // view matrix
        *m_viewMatrix = Camera::GetData().GetViewMatrix();
        m_coreProgram->SetMat4f(*m_viewMatrix, "viewMatrix");
        m_coreProgram->SetVec3f(Camera::GetData().GetPosition(), "camPosition");

        // projection matrix
        glfwGetFramebufferSize(m_window, &_frameBufferWidth, &_frameBufferHeight);
        m_projectionMatrix = new mat4(1.f);
        *m_projectionMatrix = mat4(glm::perspective(glm::radians(ViewPointData::GetData().fov),
                         static_cast<float>(_frameBufferWidth / _frameBufferHeight),
                         ViewPointData::GetData().nearPlane, ViewPointData::GetData().farPlane));

        m_coreProgram->SetMat4f(*m_projectionMatrix, "projectionMatrix");
    }
    void UpdateDt()
    {
        DeltaTime::GetData().currTime = static_cast<float>(glfwGetTime());
        DeltaTime::GetData().dt = DeltaTime::GetData().currTime - DeltaTime::GetData().lastTime;
        DeltaTime::GetData().lastTime = DeltaTime::GetData().currTime;
    }

    void Update()
    {
        // update deltatime
        UpdateDt();

        for (const auto& model : m_models)
        {
           model->Update(m_window);
        }

        // UpdateInput
        this->UpdateKeyboardInput(m_window);
        this->UpdateMouseInput(m_window);

        // Update Cameri
        Camera::GetData().Update(-1);

    }
    void Render()
    {
        // Clear buffers
        glClearColor(0.f, 0.f, 0.f, 1.0f); // Change background color to see if clearing works
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        UpdateUniforms();

        for (const auto& model : m_models)
        {
            model->Render(m_coreProgram);
        }

        // Swap buffers
        glfwSwapBuffers(m_window);
        glFlush();


        glBindVertexArray(0);
        glUseProgram(0);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void InitShader()
    {
        m_coreProgram = new Shader(_vertexShaderPath,_fragmentShaderPath);

    }
    void InitProjectionMatrix() // replace (currently holds matrix, lights, fov)
    {
       // camera
        Camera::GetData().Initialize(vec3(0.f,0.f,1.f), vec3(0.f, 1.f, 0.f));

      m_viewMatrix = new mat4(1.f);
      *m_viewMatrix = Camera::GetData().GetViewMatrix();
      // fov
     ViewPointData::GetData().fov = 120.f;
     ViewPointData::GetData().nearPlane = 0.1f;
     ViewPointData::GetData().farPlane = 1000;


      m_projectionMatrix = new mat4(1.f);
      *m_projectionMatrix = mat4(glm::perspective(glm::radians(ViewPointData::GetData().fov),
                       static_cast<float>(_frameBufferWidth / _frameBufferHeight),
                       ViewPointData::GetData().nearPlane, ViewPointData::GetData().farPlane));

      // lights
      ViewPointData::GetData().lightPosition = vec3(0.f,0.f,1.f);


      // init uniforms (mat4)
      m_coreProgram->SetMat4f(*m_viewMatrix, "viewMatrix");
      m_coreProgram->SetMat4f(*m_projectionMatrix, "projectionMatrix");

      m_coreProgram->SetVec3f(ViewPointData::GetData().lightPosition, "lightPosition");
      m_coreProgram->SetVec3f(Camera::GetData().GetPosition(), "camPosition");
    }
    void InitModels()
    {

        m_models.push_back(new Model(vec3(0.f), m_materials[0], m_textures[1], m_textures[1],
          "/home/judelhuu/CLionProjects/3D-Rendering_opengl/OBJ-Files/SnowTerrain.obj"));

        m_models[0]->SetScale(vec3(0.5f));

        std::cout << "Loaded" << std::endl;
    }
    void InitTexture()
    {
        // temporary
        m_textures.push_back(new Texture("/home/judelhuu/CLionProjects/3D-Rendering_opengl/Images/MangaGirl2.png", GL_TEXTURE_2D));
        m_textures.push_back(new Texture("/home/judelhuu/CLionProjects/3D-Rendering_opengl/Images/MangaGirl2_negative.png", GL_TEXTURE_2D));

        m_textures.push_back(new Texture("/home/judelhuu/CLionProjects/3D-Rendering_opengl/Images/container.png", GL_TEXTURE_2D));
        m_textures.push_back(new Texture("/home/judelhuu/CLionProjects/3D-Rendering_opengl/Images/container_specular.png", GL_TEXTURE_2D));

        //m_textures.push_back(new Texture("/home/judelhuu/CLionProjects/3D-Rendering_opengl/Images/686.jpg", GL_TEXTURE_2D));
        //m_textures.push_back(new Texture("/home/judelhuu/CLionProjects/3D-Rendering_opengl/Images/686.jpg", GL_TEXTURE_2D));

        // set names for later identification

        m_textures[0]->SetID("manga");
        m_textures[1]->SetID("manga_negative");

        m_textures[2]->SetID("container");
        m_textures[3]->SetID("container_negative");
    }
    void InitMaterial()
    {
       Material* material = new Material(vec3(0.1f), vec3(1.f), vec3(1.f), 0, 1);
       m_materials.emplace_back(material);
    }
    void InitOBJModels()
    {
    }

    void InitGLFW()
    {
        glfwInit();
    }
    void InitGLEW() const {
        // Initialize GLEW
        glewExperimental = GL_TRUE;
        glewInit();

        // OpenGL options
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

       glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    static GLFWwindow* CreateWindow()
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _glVersionMajor);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _glVersionMinor);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        GLFWwindow* window = glfwCreateWindow(_windowWidth, _windowHeight, _title, nullptr, nullptr);

        glfwGetFramebufferSize(window, &_frameBufferWidth, &_frameBufferHeight);
        glfwMakeContextCurrent(window);

        return window;
    }

    GLFWwindow* m_window;
    Shader* m_coreProgram;

    std::vector<Model*> m_models;
    Model* model1;
    Model* model2;

    std::vector<Texture*>  m_textures;
    std::vector<Material*> m_materials;

    mat4* m_projectionMatrix;
    mat4* m_viewMatrix;
};



#endif //GAME_H
