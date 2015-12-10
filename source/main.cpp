
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <stdio.h>
#include "Shader.h"
#include "ShaderProgram.h"
#include "GeometryArray.h"
#include "WindowManager_GLFW.h"
#include <iostream>

GeometryArray triangle;

int main () {
    // start GL context and O/S window using the GLFW helper library
    WindowManager *window = new WindowManagerGLFW();
    window->initWindow();
    glewExperimental = GL_TRUE;
    glewInit ();
    
    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    /* OTHER STUFF GOES HERE NEXT */
    vec3 vertices_array[] = {
        {{0.0f,  0.5f,  0.0f}},
        {{0.5f, -0.5f,  0.0f}},
        {{-0.5f, -0.5f,  0.0f}}
    };
    vec3 color_array[] = {
        {{0.0f,  1.0f,  0.0f}},
        {{1.0f, 0.0f,  0.0f}},
        {{0.0f, 0.0f,  1.0f}}
    };
    GLuint index_array[] = {
        0,1,2
    };
    
    std::vector<vec3> vertices;
    std::vector<vec3> colors;

    std::vector<GLuint> indicies;
    
    vertices.assign(vertices_array, vertices_array+ sizeof(vertices_array)/ sizeof(vertices_array[0]));
    colors.assign(color_array, color_array+ sizeof(color_array)/ sizeof(color_array[0]));

    indicies.assign(index_array, index_array+ sizeof(index_array)/ sizeof(index_array[0]));

    triangle.generateObjects();

    triangle.bind();

    triangle.addIndex(indicies, "tri");
    triangle.addAttribute(vertices, ATTRIB_POSITION, 3, TYPE_FLOAT);
    triangle.addAttribute(colors, ATTRIB_COLOR, 3, TYPE_FLOAT);

    
    triangle.loadAttributes();
    triangle.loadIndices();
    
    triangle.bind();

    
    Shader vertexShader = ShaderType::CreateShaderFromFile("share/shaders/BasicVertexShader.vs", GL_VERTEX_SHADER);
    vertexShader->compile();
    Shader fragmentShader = ShaderType::CreateShaderFromFile("share/shaders/BasicFragmentShader.fs", GL_FRAGMENT_SHADER);
    fragmentShader->compile();


    ShaderProgram * prgrm = new ShaderProgram();
    prgrm->addShader(vertexShader);
    prgrm->addShader(fragmentShader);
    prgrm->link();

    
    while (!window->shouldClose()) {
        // wipe the drawing surface clear
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        prgrm->use();
        // draw points 0-3 from the currently bound VAO with current in-use shader
        
        glDrawElements(GL_TRIANGLES, triangle.getCountForIndex("tri"), GL_UNSIGNED_INT, (void*)triangle.getStartForIndex("tri"));

        window->swapBuffers();
        window->checkEvents();
        
    }
    
    delete window;
    
    return 0;
}