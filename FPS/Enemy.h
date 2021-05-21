//
// Created by Overcastan on 20.05.2021.
//

#ifndef TUTORIALS_ENEMY_H
#define TUTORIALS_ENEMY_H

#include <algorithm>
#include <utility>
#include <vector>
#include <fstream>
#include <cmath>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Enemy {
    // center
    glm::vec3 center;
    GLuint rotateID = 0;

    // coords
    size_t vertex_count = 0;
    GLuint vertexbuffer = 0;
    GLuint uvbuffer = 0;
    GLfloat angle = 0.0f;
    constexpr static GLfloat step = 0.002f;

    // shader ID
    GLuint programID = 0;
    GLuint MatrixID = 0;

    // texture IDs
    GLuint texture = 0;
    GLuint textureID = 0;
    constexpr const static auto pi = glm::pi<GLfloat>();

public:
    Enemy() = default;

    void init(GLuint programID, GLuint vertexbuffer, GLuint uvbuffer, size_t vc);

    void setCenter(glm::vec3 c);

    void setTexture(GLuint texture);

    bool isClose(glm::vec3 center);

    void draw(const glm::mat4& MVP);
};


#endif //TUTORIALS_ENEMY_H
