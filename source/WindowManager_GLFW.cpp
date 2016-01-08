#include "WindowManager_GLFW.h"
#include <GLFW/glfw3.h>
#include <iostream>

bool WindowManagerGLFW::initWindow(char *title, int width, int height)
{
    if (!glfwInit ()) {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return false;
    }
    
    // uncomment these lines if on Apple OS X
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow (width,height, title, NULL, NULL);
    if (!window) {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent (window);
    
    // start GLEW extension handler

    return true;
}

bool WindowManagerGLFW::shouldClose()
{
    return glfwWindowShouldClose (window);
}
void WindowManagerGLFW::swapBuffers()
{
    // put the stuff we've been drawing onto the display
    glfwSwapBuffers (window);
}
void WindowManagerGLFW::checkEvents()
{
    // update other events like input handling
    glfwPollEvents ();
}

WindowManagerGLFW::~WindowManagerGLFW()
{
    glfwTerminate();
}