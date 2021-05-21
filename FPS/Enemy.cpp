//
// Created by Overcastan on 20.05.2021.
//

#include "Enemy.h"

void Enemy::init(GLuint programID_, GLuint vertexbuffer_, GLuint uvbuffer_, size_t vc) {
    vertexbuffer = vertexbuffer_;
    uvbuffer = uvbuffer_;
    vertex_count = vc;
    programID = programID_;
    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    rotateID = glGetUniformLocation(programID, "rotate");
    textureID = glGetUniformLocation(programID, "myTextureSampler");
}

void Enemy::setCenter(glm::vec3 c) {
    center = c;
}

void Enemy::setTexture(GLuint texture_) {
    texture = texture_;
}

bool Enemy::isClose(glm::vec3 v) {
    glm::vec3 dist = v - center;
    GLfloat len = glm::sqrt(dist.x * dist.x + dist.y * dist.y + dist.z * dist.z);
    return (len <= 0.66);
}

void Enemy::draw(const glm::mat4& MVP) {

    if ((angle += step) > 2 * pi) {
        angle -= 2 * pi;
    }

    GLfloat scale = 1;
    if (center.x != 0 || center.y != 0 || center.z != 0) {
        scale = 1 / glm::sqrt(center.x * center.x + center.y * center.y + center.z * center.z);
    }

    const glm::mat4 rotateMat = glm::translate(glm::mat4(), center) *
                                glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 0, 1)) *
                                glm::scale(glm::mat4(), glm::vec3(scale, scale, scale));

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
}