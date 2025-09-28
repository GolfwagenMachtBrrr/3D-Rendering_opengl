//
// Created by judelhuu on 8/23/25.
//

#ifndef GAMEDATA_H
#define GAMEDATA_H

#include <glm/glm.hpp>

namespace GameData
{
    namespace core
    {
        // https://stackoverflow.com/questions/41328038/singleton-template-as-base-class-in-c?utm_source=chatgpt.com
        template<typename T>
        class Singleton
        {
        public:
            static T& GetData() // just 4 better naming :D
            {
                static T instance;
                return instance;
            }

        protected:
            Singleton() {}
            ~Singleton() {}
        public:
            Singleton(Singleton const&) = delete;
            Singleton& operator=(Singleton const&) = delete;
        };

    }

    namespace Config
    {
        static const char *_title = "NichtÄndern";
        static int _windowWidth = 1920;
        static int _windowHeight = 1080;
        static int _glVersionMajor = 4;
        static int _glVersionMinor = 4;

        inline int _frameBufferWidth;
        inline int _frameBufferHeight;
    }

    namespace Shaders
    {
        static const char *_vertexShaderPath =
                "/home/judelhuu/CLionProjects/OpenGL_3D_Tutorial/Shaders/vertex_core.glsl";
        static const char *_fragmentShaderPath =
                "/home/judelhuu/CLionProjects/OpenGL_3D_Tutorial/Shaders/fragment_core.glsl";
    }

    namespace util
    {
        struct ViewPointData : public core::Singleton<ViewPointData>
        {
            // sollte aufgeteilt werden sobald zweckvolle
            // fov
            float fov = 0;
            float nearPlane = 0;
            float farPlane = 0;

            // lighting
            glm::vec3 lightPosition{};
        };
        struct DeltaTime : public core::Singleton<DeltaTime>
        {
            float dt = 0;
            float currTime = 0;
            float lastTime = 0;
        };
        struct MouseInput : public core::Singleton<MouseInput>
        {
            double lastMouseX = 0;
            double mouseY= 0;
            double mouseX= 0;
            double lastMouseY= 0;
            double mouseOffsetX= 0;
            double mouseOffsetY= 0;

            bool   firstMouse= true;
        };
    }

    enum Textures
    {
        MANGA_2 = 0,
        MANGA_2_NEGATIVE = 1,
        CONTAINER = 2,
        CONTAINER_SPECULAR = 3,
        COTTAGE = 4,
        COTTAGE_DIFFUSE,
    };

    inline void PrintVec3(const glm::vec3& vec)
    {
        std::cout << vec.x << " " << vec.y << " " << vec.z << " " << std::endl;
    }
}


using namespace GameData;
using namespace GameData::Config;
using namespace GameData::Shaders;
using namespace GameData::util;


#endif //GAMEDATA_H





