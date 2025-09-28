#version 440

struct Material
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  sampler2D diffuseTexture;
  sampler2D specularTexture;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

vec3 calculateAmbient(Material material)
{
    return material.ambient;
}

vec3 calculateDiffuse(Material material)
{
    vec3 posToLightDirVec = normalize(lightPosition - vs_position);
    float diffuse = clamp(dot(posToLightDirVec,vs_normal),0,1);
    vec3 diffuseFinal = material.diffuse * diffuse;

    return diffuseFinal;
}

vec3 calculateSpecular(Material material)
{
    vec3 lightToPosDirVec = normalize(vs_position - lightPosition);
    vec3 reflectiveVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
    vec3 posToViewDirVec = normalize(cameraPosition - vs_position);
    float specularConstant = pow(max(dot(posToViewDirVec, reflectiveVec),0),100);
    vec3 specularFinal = material.specular * specularConstant + texture(material.specularTexture, vs_texcoord).xyz;

    return specularFinal;
}


void main()
{
    //Ambient light
    vec3 ambientFinal = calculateAmbient(material);

    // Diffuse light
    vec3 diffuseFinal = calculateDiffuse(material);

    // specular light
    vec3 specularFinal = calculateSpecular(material);


    // Attenuation


    // final
    fs_color = texture(material.diffuseTexture, vs_texcoord)
    * (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal,1.f));

    fs_color = (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal,1.f));
}


//fs_color = texture(material.diffuseTexture, vs_texcoord) + texture(material.specularTexture, vs_texcoord)
//* (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal,1.f));