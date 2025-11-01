//
// Created by judelhuu on 8/25/25.
//

#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Material.h"
#include "OBJLoader.h"

class Model
{
public:
    /*Model(const glm::vec3& position, Material* material, Texture* orTexDif, Texture* orTexSpec)
    {
        // inti data
        m_material = material;
        m_overrideTextureDiffuse = orTexDif;
        m_overrideTextureSpecular = orTexSpec;
        m_position = position;

        m_meshes.push_back(new Mesh(new Pyramid(), vec3(0.f), vec3(0.f), vec3(1.f)));

        for (const auto& mesh : m_meshes)
        {
            mesh->Move(position);
        }

        std::cout << "model created" << std::endl;
    }*/
    Model(const glm::vec3& position, Material* material, Texture* orTexDif, Texture* orTexSpec, const char* filepath)
    {
        m_material = material;
        m_overrideTextureDiffuse = orTexDif;
        m_overrideTextureSpecular = orTexSpec;
        m_position = position;

        // tmp
            std::vector<Texture*> textures;
            textures.push_back(std::move(orTexDif));
            textures.push_back(std::move(orTexSpec));

            std::vector<Material*> materials;
            materials.push_back(std::move(material));
        // tmp

        std::vector<Vertex> mesh = OBJLoader::GetData().LoadFromFile(filepath, textures, materials);
        m_meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0 ,vec3(0.f), vec3(0.f), vec3(1.f)));


        for (const auto& mesh : m_meshes)
        {
            mesh->Move(position);
        }
    }
    //Model(const glm::vec3& position, Material* material, std::vector<Texture>& textures, const char* filepath);

    // Model(const glm::vec3& position, std::vector<Texture*>& textures, std::vector<Material*>& materials,  char* filepath)
    // {
    //     // inti data
    //     m_position = position;
    //     m_material = materials[0];
    //
    //     m_materials.assign(materials.begin(), materials.end());
    //     m_textures.assign(textures.begin(), textures.end());
    //
    //     std::vector<Vertex> mesh = OBJLoader::GetData().LoadFromFile(filepath, textures, materials);
    //     m_meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0 ,vec3(0.f), vec3(0.f), vec3(1.f)));
    //
    //
    //
    //
    //     for (const auto& mesh : m_meshes)
    //     {
    //         mesh->Move(position);
    //     }
    //
    //     std::cout << "model created" << std::endl;
    // }

    ~Model()
    {
        for (auto &i : m_meshes)
        {
            delete i;
        }

        std::cout << "Model destructed" << std::endl;
    }



    void Update(GLFWwindow* window)
    {
        for (const auto&i : m_meshes)
        {
            i->Update(window);
        }
    }

    void Render(Shader* shader)
    {

        this->UpdateUniforms();

        // send Material & use shader
        m_material->SendToShader(*shader);
        shader->Use();

        // render Meshsss
        for (const auto&j : m_meshes)
        {
            m_overrideTextureDiffuse->Bind(0);
            m_overrideTextureSpecular->Bind(1);
            j->Render(shader);
        }

    }

    void Rotate(const glm::vec3& rotation) const
    {
        for (auto& mesh : m_meshes)
        {
            mesh->Rotate(rotation);
        }
    }

    void SetScale(const vec3& scale)
    {
        for (auto& mesh : m_meshes)
        {
            mesh->SetScaleValue(scale);
        }
    }

private:

    void PrintVec3(const glm::vec3& vec)
    {
        std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
    }
    void UpdateUniforms()
    {

    }

    Material* m_material;

    Texture* m_overrideTextureDiffuse;
    Texture* m_overrideTextureSpecular;

    std::vector<Mesh*> m_meshes;

    std::vector<Material*> m_materials;
    std::vector<Texture*> m_textures;

    glm::vec3 m_position;

};



#endif //MODEL_H
