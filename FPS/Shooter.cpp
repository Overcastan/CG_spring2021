//
// Created by Overcastan on 19.05.2021.
//

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <random>

#include "Enemy.h"
#include "Projectile.h"

int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1920, 1080, "First Person Shooter", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1920/2, 1080/2);

    // Dark blue background
    glClearColor(0.9f, 0.9f, 1.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "VertexShader.vertexshader", "FragmentShader.fragmentshader" );
    // Load the texture
    // GLuint Texture = loadDDS("uvmap.DDS");
    GLuint EnemyTexture = loadBMP_custom("Enemy_face.bmp");
    GLuint FireTexture = loadBMP_custom("fire.bmp");


    // Read our .obj file
    std::vector<glm::vec3> enemy_vertices;
    std::vector<glm::vec2> enemy_uvs;
    std::vector<glm::vec3> enemy_normals; // Won't be used at the moment.
    bool enemy_res = loadOBJ("Enemy_cube.obj", enemy_vertices, enemy_uvs, enemy_normals);

    GLuint enemy_vertexbuffer;
    GLuint enemy_vertexCount = enemy_vertices.size();
    glGenBuffers(1, &enemy_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, enemy_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, enemy_vertices.size() * sizeof(glm::vec3), &enemy_vertices[0], GL_STATIC_DRAW);

    GLuint enemy_uvbuffer;
    glGenBuffers(1, &enemy_uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, enemy_uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, enemy_uvs.size() * sizeof(glm::vec2), &enemy_uvs[0], GL_STATIC_DRAW);

    std::vector<glm::vec3> projectile_vertices;
    std::vector<glm::vec2> projectile_uvs;
    std::vector<glm::vec3> projectile_normals; // Won't be used at the moment.
    bool projectile_res = loadOBJ("sphere.obj", projectile_vertices, projectile_uvs, projectile_normals);

    GLuint projectile_vertexbuffer;
    GLuint projectile_vertexCount = projectile_vertices.size();
    glGenBuffers(1, &projectile_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, projectile_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, projectile_vertices.size() * sizeof(glm::vec3), &projectile_vertices[0], GL_STATIC_DRAW);

    GLuint projectile_uvbuffer;
    glGenBuffers(1, &projectile_uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, projectile_uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, projectile_uvs.size() * sizeof(glm::vec2), &projectile_uvs[0], GL_STATIC_DRAW);

    std::vector<Enemy> enemies;
    std::vector<Projectile> fireballs;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> len_distr(2.0, 6.0);
    std::uniform_real_distribution<> alpha_distr(0.0, 2 * pi<float>());

    double last_add_time = glfwGetTime();

    int mouseState = GLFW_RELEASE;

    do{

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        int currMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (mouseState == GLFW_RELEASE && currMouseState == GLFW_PRESS) {
            fireballs.emplace_back();
            auto &f = fireballs.back();
            f.init(programID, projectile_vertexbuffer, projectile_uvbuffer, projectile_vertexCount);
            f.setPosition(getPosition());
            f.setDirection(getDirection());
            f.setTexture(FireTexture);
        }
        mouseState = currMouseState;

        double curr_time = glfwGetTime();

        if (curr_time - last_add_time > 1 && enemies.size() < 10) {
            enemies.emplace_back();
            Enemy &enemy = enemies.back();
            enemy.init(programID, enemy_vertexbuffer, enemy_uvbuffer, enemy_vertexCount);

            const double r = len_distr(gen);
            const double phi = alpha_distr(gen);
            const double psi = alpha_distr(gen);
            glm::vec3 center = glm::vec3(
                    cos(phi) * sin(psi) * r,
                    sin(phi) * r,
                    cos(phi) * cos(psi) * r
            );

            enemy.setCenter(center);
            enemy.setTexture(EnemyTexture);
            last_add_time = curr_time;
        }

        for (size_t i = 0; i < fireballs.size();) {
            bool touch = false;
            const glm::vec3 c = fireballs[i].getCenter();
            for (size_t j = 0; j < enemies.size(); ++j) {
                if (enemies[j].isClose(c)) {
                    touch = true;
                    enemies.erase(enemies.begin() + j);
                    break;
                }
            }
            if (touch) {
                fireballs.erase(fireballs.begin() + i);
            } else {
                ++i;
            }
        }

        for (Enemy &e : enemies) {
            e.draw(MVP);
        }

        for (size_t i = 0; i < fireballs.size();) {
            if (fireballs[i].draw(MVP)) {
                ++i;
                continue;
            } else {
                fireballs.erase(fireballs.begin() + i);
            }
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &enemy_vertexbuffer);
    glDeleteBuffers(1, &enemy_uvbuffer);
    glDeleteBuffers(1, &projectile_vertexbuffer);
    glDeleteBuffers(1, &projectile_uvbuffer);

    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

