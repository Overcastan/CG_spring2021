//
// Created by Overcastan on 20.05.2021.
//

#include <iostream>
#include "Projectile.h"

void Projectile::init(GLuint programID_, GLuint vertexbuffer_, GLuint uvbuffer_, size_t vc) {
    vertexbuffer = vertexbuffer_;
    uvbuffer = uvbuffer_;
    vertex_count = vc;
    programID = programID_;
    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    rotateID = glGetUniformLocation(programID, "rotate");
    textureID = glGetUniformLocation(programID, "myTextureSampler");
}

void Projectile::setDirection(glm::vec3 d) {
    direction = d;
}

void Projectile::setPosition(glm::vec3 pos) {
    position = pos;
}

void Projectile::setTexture(GLuint texture_) {
    texture = texture_;
}

glm::vec3 Projectile::getCenter() {
    return direction * dist + position;
}

bool Projectile::draw(const glm::mat4& MVP) {

    if ((angle += angleStep) > 2 * pi) {
        angle -= 2 * pi;
    }

    dist += distStep;

    if (dist > 30) {
        return false;
    }

    glm::vec3 c = getCenter();
    GLfloat scale = glm::sqrt(c.x * c.x + c.y * c.y + c.z * c.z);

    const glm::mat4 rotateMat = glm::translate(glm::mat4(), c) *
                                glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1)) *
                                glm::scale(glm::mat4(), glm::vec3(1 / scale, 1 / scale, 1 / scale));

    // Use our shader
    glUseProgram(programID);

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(rotateID, 1, GL_FALSE, &rotateMat[0][0]);

    // Enable attribute arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(textureID, 0);

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            nullptr             // array buffer offset
    );

    // 2nd attribute buffer : colors
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
            1,                  // attribute 1. No particular reason for 1, but must match the layout in the shader.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            nullptr             // array buffer offset
    );

    // Draw the triangle !
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);

    // Disable attribute arrays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    return true;
}