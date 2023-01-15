#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <chrono>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimatedSprite.h"

using namespace std;


GLfloat point[] = {
    -100.f, -100.f, 0.0f, // top right
    -100.f, 100.f, 0.0f,  // bottom right
    100.f, 100.f, 0.0f,   // top left
    -100.f, -100.f, 0.0f, // bottom right
    100.f, -100.f, 0.0f,  // bottom left
    100.f, 100.f, 0.0f,   // top left

    

};

GLfloat colors[] = {
    0.3f, 0.9f, 0.1f,
    0.3f, 0.8f, 0.3f,
    0.3f, 0.9f, 0.5f,
};

GLfloat texCoord[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

glm::ivec2 g_windowSize(640, 480);

bool isEagle = false;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    g_windowSize.x = width;
    g_windowSize.y = height;
    glViewport(0, 0, width, height);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        isEagle = !isEagle;
    }
}

int main(int argc, char** argv)
{
    if (!glfwInit())
    {
        cout << "glfwInit failed!" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y, "PoRtHoLe", nullptr, nullptr);
    if (!pWindow)
    {
        cout << "glfwCreateWindow failed!" << endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);

   
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL())
    {
        cout << "Can't load GLAD!" << endl;
    }

    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
    cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;

    glClearColor(0.5, 0.9, 0.2, 0.5);

    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            cerr << "Can't create shader program: " << "DefaultShader" << endl;
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            cerr << "Can't create shader program: " << "SpriteShader" << endl;
            return -1;
        }

        auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/S9OtX.png");

        vector<string> subTexturesNames = {
            "Lava1",
            "Lava2",
            "Lava3",
            
        };

        auto pTextureAtlas = resourceManager.loatTextureAtlas("DefaultTextureAtlas", "res/textures/S9OtX.png", move(subTexturesNames), 20, 20);

        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "stand");
        pSprite->setPosition(glm::vec2(500, 500));

        auto pAnimatedSprite = resourceManager.loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "stand");
        pAnimatedSprite->setPosition(glm::vec2(300, 100));
        vector<pair<string, uint64_t>> LavaState;
        LavaState.emplace_back(make_pair<string, uint64_t>("Lava1", 100000000));
        LavaState.emplace_back(make_pair<string, uint64_t>("Lava2", 100000000));
        LavaState.emplace_back(make_pair<string, uint64_t>("Lava3", 100000000));


        pAnimatedSprite->insertState("LavaState", move(LavaState));

        pAnimatedSprite->setState("LavaState");

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texCoord_vbo = 0;
        glGenBuffers(1, &texCoord_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));

        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_windowSize.x), 0.f, static_cast<float>(g_windowSize.y), -300.f, 300.f);

        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        auto lastTime = chrono::high_resolution_clock::now();

        while (!glfwWindowShouldClose(pWindow))
        {

            auto currentTime = chrono::high_resolution_clock::now();
            uint64_t duration = chrono::duration_cast<chrono::nanoseconds>(currentTime - lastTime).count();
            lastTime = currentTime;
            pAnimatedSprite->update(duration);

            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            pSprite->render();

            pAnimatedSprite->render();

            glfwSwapBuffers(pWindow);

            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}