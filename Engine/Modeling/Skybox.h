#ifndef SKYBOX_H
#define SKYBOX_H

#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include "../Utils/CommonValues.h"
#include "./Mesh.h"
#include "./Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Skybox
{
public:
    Skybox();
    Skybox(std::string skyboxLocation);

    void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

    ~Skybox();

private:
    Mesh* skyMesh;
    Shader* skyShader;

    GLuint textureId;
    GLuint uniformProjection, uniformView;
};

#endif