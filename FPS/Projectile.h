//
// Created by Overcastan on 20.05.2021.
//

#ifndef TUTORIALS_PROJECTILE_H
#define TUTORIALS_PROJECTILE_H

#include <algorithm>
#include <utility>
#include <vector>
#include <fstream>
#include <cmath>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Projectile {
    glm::vec3 direction;
    glm::vec3 position;
    GLuint rotateID = 0;

    // coords
    size_t vertex_count = 0;
    GLuint vertexbuffer = 0;
    GLuint uvbuffer = 0;
    GLfloat angle = 0.0f;
    constexpr static GLfloat angleStep = 0.01f;
    GLfloat dist = 0.5f;
    constexpr static GLfloat distStep = 0.05f;

    // shader ID
    GLuint programID = 0;
    GLuint MatrixID = 0;

    // texture IDs
    GLuint texture = 0;
    GLuint textureID = 0;
    constexpr const static auto pi = glm::pi<GLfloat>();

public:
    Projectile() = default;

    void init(GLuint programID, GLuint vertexbuffer, GLuint uvbuffer, size_t vc);

    void setPosition(glm::vec3 pos);

    void setDirection(glm::vec3 d);

    void setTexture(GLuint texture);

    glm::vec3 getCenter();

    bool draw(const glm::mat4& MVP);
};


#endif //TUTORIALS_PROJECTILE_H
