cmake_minimum_required(VERSION 3.31)
project(OpenGL_3D_Tutorial)

set(CMAKE_CXX_STANDARD 20)


find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(soil2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)

add_executable(OpenGL_3D_Tutorial main.cpp
        src/Shader.cpp
        include/Shader.h
        src/Texture.cpp
        include/Texture.h
        src/Material.cpp
        include/Material.h
        include/Vertex.h
        src/Mesh.cpp
        include/Mesh.h
        src/Primitive.cpp
        include/Primitive.h
        Game.cpp
        Game.h
        GameData.h
        Camera.cpp
        Camera.h
        Model.cpp
        Model.h
        OBJLoader.h
)


target_include_directories(OpenGL_3D_Tutorial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(OpenGL_3D_Tutorial PRIVATE glfw)
target_link_libraries(OpenGL_3D_Tutorial PRIVATE glm::glm)
target_link_libraries(OpenGL_3D_Tutorial PRIVATE soil2)
target_link_libraries(OpenGL_3D_Tutorial PRIVATE GLEW::GLEW)
