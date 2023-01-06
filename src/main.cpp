#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

int g_windowsSize_X = 640;
int g_windowsSize_Y = 480;
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    int g_windowsSize_X = width;
    int g_windowsSize_Y = height;
    cout<<"width"<<width<<endl;
    cout<<"height"<<height<<endl;
    glViewport(0, 0, g_windowsSize_X, g_windowsSize_Y );
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scanCode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
        cout<<"I always come back "<<endl;
    }
}

int main(void)
{


    if (!glfwInit())
    {
        cout<<"glfwInit failed!"<<endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(g_windowsSize_X, g_windowsSize_Y, "Porthole", NULL, NULL);
    if (!pWindow)
    {
        cout<<"glfwCreateWindow failed!"<<endl;
        glfwTerminate();
        return -1;
    }
    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow,glfwKeyCallback);
    glfwMakeContextCurrent(pWindow);
    if(!gladLoadGL())
    {
        cout<<"Cant load GLAD"<<endl;
        return -1;
    }

    cout<<"Renderer:"<<glGetString(GL_RENDERER)<<endl;
    cout<<"OpenGL version: "<<glGetString(GL_VERSION)<<endl;
    glClearColor(1, 0.6, 0.5, 0.2);

    while (!glfwWindowShouldClose(pWindow))
    {  
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}