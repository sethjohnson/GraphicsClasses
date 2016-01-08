
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <stdio.h>
#include "Shader.h"
#include "ShaderProgram.h"
#include "GeometryArray.h"
#include "WindowManager_GLFW.h"
#include <iostream>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

GeometryArray triangle;
GeometryArray quad;
GeometryArray cube;

int main () {
    // start GL context and O/S window using the GLFW helper library
    WindowManager *window = new WindowManagerGLFW();
    window->initWindow("Title", 800,800);
    glewExperimental = GL_TRUE;
    glewInit ();
    
    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    //    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
    glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
    glClampColor(GL_CLAMP_VERTEX_COLOR, GL_FALSE);
    glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
    
    /* OTHER STUFF GOES HERE NEXT */
    vec3 triangle_vertices_array[] = {
        {0.0f,  0.5f,  0.0f},
        {0.5f, -0.5f,  0.0f},
        {-0.5f, -0.5f,  0.0f}
    };
    vec3 triangle_color_array[] = {
        {0.2f,  0.5f,  0.2f},
        {0.5f, 0.2f,  0.2f},
        {0.5f, 0.5f,  0.5f}
    };
    GLuint triangle_index_array[] = {
        0,1,2
    };
    
    
    
    glm::vec3 v[] = {
        glm::vec3(0.5f, 0.5f, 0.5f ),
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f,-0.5f, 0.5f),
        glm::vec3(0.5f,-0.5f, 0.5f),
        glm::vec3(0.5f,-0.5f,-0.5f),
        glm::vec3(0.5f, 0.5f,-0.5f),
        glm::vec3(-0.5f, 0.5f,-0.5f),
        glm::vec3(-0.5f,-0.5f,-0.5f)
    };
    glm::vec3 n[] = {
        glm::vec3(0,0,1),
        glm::vec3(1,0,0),
        glm::vec3(0,1,0),
        glm::vec3(-1,0,0),
        glm::vec3(0,-1,0),
        glm::vec3(0,0,-1)
    };
    glm::vec3 c[] = {
        glm::vec3(0.5,0.5,0.5),
        glm::vec3(0.5,0.5,1),
        glm::vec3(0.5,0.6,0.5),
        glm::vec3(0.5, 0.75, 1),
        glm::vec3(1, 0.6, 0.5),
        glm::vec3(1,0.5,1),
        glm::vec3(1, 0.6, 0.5),
        glm::vec3(1, 0.6, 1)
    };
    
    glm::vec3 vertices_array[]  = {
        v[0], v[1], v[2], v[3],
        v[0], v[3], v[4], v[5],
        v[0], v[5], v[6], v[1],
        v[1], v[6], v[7], v[2],
        v[7], v[4], v[3], v[2],
        v[4], v[7], v[6], v[5]
    };
    
    glm::vec3 normals_array[] = {
        n[0], n[0], n[0], n[0],
        n[1], n[1], n[1], n[1],
        n[2], n[2], n[2], n[2],
        n[3], n[3], n[3], n[3],
        n[4], n[4], n[4], n[4],
        n[5], n[5], n[5], n[5]
    };
    
    glm::vec3 colors_array[] = {
#if 0
        c[0], c[1], c[2], c[3],
        c[0], c[3], c[4], c[5],
        c[0], c[5], c[6], c[1],
        c[1], c[6], c[7], c[2],
        c[7], c[4], c[3], c[2],
        c[4], c[7], c[6], c[5]
#elif 1
        c[0], c[0], c[0], c[0],
        c[1], c[1], c[1], c[1],
        c[2], c[2], c[2], c[2],
        c[3], c[3], c[3], c[3],
        c[4], c[4], c[4], c[4],
        c[5], c[5], c[5], c[5]
#else
        c[7], c[7], c[7], c[7],
        c[7], c[7], c[7], c[7],
        c[7], c[7], c[7], c[7],
        c[7], c[7], c[7], c[7],
        c[7], c[7], c[7], c[7],
        c[7], c[7], c[7], c[7]
        
#endif
    };
    
    GLuint indices_array[] = {
        0, 1, 2, // front
        2, 3, 0,
        4, 5, 6, // right
        6, 7, 4,
        8, 9, 10, // top
        10, 11, 8,
        12, 13, 14, // left
        14, 15, 12,
        16, 17, 18, // bottom
        18, 19, 16,
        20, 21, 22, // back
        22, 23, 20
    };
    
    
    vec3 quad_vertices_array[] = {
        {-1.0f,  -1.f,  0.0f},
        {-1.0f, 1.f,  0.0f},
        { 1.0f, 1.f,  0.0f},
        { 1.0f, -1.f,  0.0f}
    };
    
    vec2 quad_tex_array[] = {
        {0.0f,  0.f},
        {0.0f, 1.f},
        { 1.0f, 1.f},
        { 1.0f, 0.0f}
    };
    GLuint quad_index_array[] = {
        0,1,2,2,3,0
    };
    
    std::vector<vec3> triangle_vertices;
    std::vector<vec3> triangle_colors;
    
    std::vector<GLuint> triangle_indicies;
    
    triangle_vertices.assign(triangle_vertices_array, triangle_vertices_array+ sizeof(triangle_vertices_array)/ sizeof(triangle_vertices_array[0]));
    triangle_colors.assign(triangle_color_array, triangle_color_array+ sizeof(triangle_color_array)/ sizeof(triangle_color_array[0]));
    
    triangle_indicies.assign(triangle_index_array, triangle_index_array+ sizeof(triangle_index_array)/ sizeof(triangle_index_array[0]));
    
    
    
    triangle.generateObjects();
    
    triangle.bind();
    
    triangle.addIndex(triangle_indicies, "tri");
    triangle.addAttribute(triangle_vertices, ATTRIB_POSITION, 3, TYPE_FLOAT);
    triangle.addAttribute(triangle_colors, ATTRIB_COLOR, 3, TYPE_FLOAT);
    
    
    triangle.loadAttributes();
    triangle.loadIndices();
    
    triangle.bind();
    
    
    
    std::vector<vec3> quad_vertices;
    std::vector<vec2> quad_tex;
    
    std::vector<GLuint> quad_indicies;
    
    quad_vertices.assign(quad_vertices_array, quad_vertices_array+ sizeof(quad_vertices_array)/ sizeof(quad_vertices_array[0]));
    quad_tex.assign(quad_tex_array, quad_tex_array+ sizeof(quad_tex_array)/ sizeof(quad_tex_array[0]));
    
    quad_indicies.assign(quad_index_array, quad_index_array+ sizeof(quad_index_array)/ sizeof(quad_index_array[0]));
    
    
    
    quad.generateObjects();
    
    quad.bind();
    
    quad.addIndex(quad_indicies, "quad");
    quad.addAttribute(quad_vertices, ATTRIB_POSITION, 3, TYPE_FLOAT);
    quad.addAttribute(quad_tex, ATTRIB_TEXCOORD, 2, TYPE_FLOAT);
    
    
    quad.loadAttributes();
    quad.loadIndices();
    
    quad.bind();
    
    
    std::vector<vec3> cube_vertices;
    std::vector<vec3> cube_color;
    
    std::vector<GLuint> cube_indicies;
    
    cube_vertices.assign((vec3*)vertices_array, (vec3*)vertices_array+ sizeof(vertices_array)/ sizeof(vertices_array[0]));
    cube_color.assign((vec3*)colors_array, (vec3*)colors_array+ sizeof(colors_array)/ sizeof(colors_array[0]));
    
    cube_indicies.assign(indices_array, indices_array+ sizeof(indices_array)/ sizeof(indices_array[0]));
    
    
    
    cube.generateObjects();
    
    cube.bind();
    
    cube.addIndex(cube_indicies, "cube");
    cube.addAttribute(cube_vertices, ATTRIB_POSITION, 3, TYPE_FLOAT);
    cube.addAttribute(cube_color, ATTRIB_COLOR, 3, TYPE_FLOAT);
    
    
    cube.loadAttributes();
    cube.loadIndices();
    
    cube.bind();
    
    
    
    
    std::map<std::string, Shader> shaders;
    
    shaders["BasicVertexShader.vs"] = ShaderType::CreateShaderFromFile("share/shaders/BasicVertexShader.vs", GL_VERTEX_SHADER);
    shaders["BasicVertexShader.fs"] = ShaderType::CreateShaderFromFile("share/shaders/BasicFragmentShader.fs", GL_FRAGMENT_SHADER);
    shaders["TextureQuad.vs"] = ShaderType::CreateShaderFromFile("share/shaders/TextureQuad.vs", GL_VERTEX_SHADER);
    shaders["TextureQuad.fs"] = ShaderType::CreateShaderFromFile("share/shaders/TextureQuad.fs", GL_FRAGMENT_SHADER);
    shaders["Mesh.vs"] = ShaderType::CreateShaderFromFile("share/shaders/MeshVertexShader.vs", GL_VERTEX_SHADER);
    shaders["Mesh.fs"] = ShaderType::CreateShaderFromFile("share/shaders/MeshFragmentShader.fs", GL_FRAGMENT_SHADER);
    
    shaders["XYZMap.vs"] = ShaderType::CreateShaderFromFile("share/shaders/MeshXYZMap.vs", GL_VERTEX_SHADER);
    shaders["XYZMap.fs"] = ShaderType::CreateShaderFromFile("share/shaders/MeshXYZMap.fs", GL_FRAGMENT_SHADER);
    
    
    for (auto &s : shaders) {
        s.second->compile();
    }
    
    
    ShaderProgram * prgrm = new ShaderProgram();
    prgrm->addShader(shaders["BasicVertexShader.vs"]);
    prgrm->addShader(shaders["BasicVertexShader.fs"]);
    prgrm->link();
    
    ShaderProgram * texprgm = new ShaderProgram();
    texprgm->addShader(shaders["TextureQuad.vs"]);
    texprgm->addShader(shaders["TextureQuad.fs"]);
    texprgm->link();
    
    ShaderProgram * mapProgram = new ShaderProgram();
    mapProgram->addShader(shaders["XYZMap.vs"]);
    mapProgram->addShader(shaders["XYZMap.fs"]);
    mapProgram->link();
    
    GLint dims[4] = {0};
    glGetIntegerv(GL_VIEWPORT, dims);
    GLint fbWidth = dims[2];
    GLint fbHeight = dims[3];
    
    GLint renderWidth = fbWidth*1;
    GLint renderHeight = fbHeight*1;
    
    
    
    
    
    GLuint BackSideBuffer = 0;
    glGenFramebuffers(1, &BackSideBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, BackSideBuffer);
    // The texture we're going to render to
    GLuint BackSideTexture;
    glGenTextures(1, &BackSideTexture);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, BackSideTexture);
    
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F, renderWidth, renderHeight, 0,GL_RGBA, GL_FLOAT, 0);
    
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // The depth buffer
    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderWidth, renderHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    
    GLuint FrontSideBuffer = 0;
    glGenFramebuffers(1, &FrontSideBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, FrontSideBuffer);
    // The texture we're going to render to
    GLuint FrontSideTexture;
    glGenTextures(1, &FrontSideTexture);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, FrontSideTexture);
    
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F, renderWidth, renderHeight, 0,GL_RGBA, GL_FLOAT, 0);
    
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // The depth buffer
    //    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderWidth, renderHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
    
    
    
    
    
    GLuint BackgroundBuffer = 0;
    glGenFramebuffers(1, &BackgroundBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, BackgroundBuffer);
    // The texture we're going to render to
    GLuint BackgroundTexture;
    glGenTextures(1, &BackgroundTexture);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, BackgroundTexture);
    
    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F, renderWidth, renderHeight, 0,GL_RGBA, GL_FLOAT, 0);
    
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // The depth buffer
    //    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderWidth, renderHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

    
    
    GLfloat * volume = new float[512*512*512];
    int num = 512*512*512;
    int dim2D = 512*512;
    
    for (int i = 0; i < num; i++) {
        int layer = i/dim2D;
        int layerCoord = i%dim2D;
        int column = layerCoord/512;
        int row = layerCoord%512;
        
        
        int c = column/(512.0/8.0);
        int r = row/(512.0/8.0);
        int l = layer/(512.0/8.0);
        
        bool fill = ((c%2) ^ (r%2)) ^ (l%2);
        
            volume[i] = fill*0.01;

    }
    
    GLuint VolumeTexture;
    glGenTextures(1, &VolumeTexture);
    glBindTexture(GL_TEXTURE_3D, VolumeTexture);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, 512, 512, 512, 0, GL_RED, GL_FLOAT, volume);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
    
    
    
    glBindFramebuffer(GL_FRAMEBUFFER, BackSideBuffer);
    
    
    
    
    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, BackSideTexture, 0);
    
    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;
    
    glClearColor(0.45, 0.50, 0.6, 0);
    
    float time = 0;
    while (!window->shouldClose()) {
        time+=0.1;
        // wipe the drawing surface clear
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        
        glm::mat4 volumeTrans = glm::translate( glm::vec3(0.0f,0.0f,0.0f));
        volumeTrans = glm::rotate(volumeTrans, 3.0f, glm::vec3(1,0,0.2));
        volumeTrans = glm::translate(volumeTrans, glm::vec3(0.2,0.5,-0.2));
        
        glm::mat4 volumeTransInv = glm::inverse(volumeTrans);
        

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        mapProgram->use();
        mapProgram->setUniform("projectionMatrix",*(mat4*)glm::value_ptr(glm::perspectiveFov((float)M_PI_4, 800.f, 800.f, 0.1f, 100.f)));
        mapProgram->setUniform("viewMatrix",*(mat4*)glm::value_ptr(glm::lookAt(
                                                                               glm::vec3(2,2,2), //Cam Position
                                                                               glm::vec3(0,0,0),  //Look at point
                                                                               glm::vec3(0.0f, 1.0f, 0.0f)))); //Up)));
        mapProgram->setUniform("modelMatrix",*(mat4*)&volumeTrans);
        mapProgram->setUniform("volumeModelMatrix",*(mat4*)& volumeTransInv);
        
        // Render to our framebuffer
        glViewport(0,0,renderWidth,renderHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        
        cube.bind();
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, BackSideTexture, 0);
        
        glBindFramebuffer(GL_FRAMEBUFFER, BackSideBuffer);
        glClearColor(0, 0, 0, 0);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
        
        glDrawElements(GL_TRIANGLES, cube.getCountForIndex("cube"), GL_UNSIGNED_INT, (void*)cube.getStartForIndex("cube"));
        
        
        glm::mat4 triTrans = glm::translate( glm::vec3(0.0f,0.0f,0.0f));
        triTrans = glm::scale(triTrans, glm::vec3(3,3,3));
        triTrans = glm::translate(triTrans, glm::vec3(-0.1,0,0.2));
        
        
        mapProgram->setUniform("modelMatrix",*(mat4*)glm::value_ptr(triTrans));
        triangle.bind();
        
        glDrawElements(GL_TRIANGLES, triangle.getCountForIndex("tri"), GL_UNSIGNED_INT, (void*)triangle.getStartForIndex("tri"));
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, FrontSideBuffer);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, FrontSideTexture, 0);
        
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
        
        glDisable(GL_CULL_FACE);
        
        cube.bind();
        
        mapProgram->setUniform("modelMatrix",*(mat4*)&volumeTrans);
        glCullFace(GL_BACK);
        glClearColor(0, 0, 0, 0);

        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES, cube.getCountForIndex("cube"), GL_UNSIGNED_INT, (void*)cube.getStartForIndex("cube"));
        
        
        
        
        glDisable(GL_CULL_FACE);
        prgrm->use();
        prgrm->setUniform("projectionMatrix",*(mat4*)glm::value_ptr(glm::perspectiveFov((float)M_PI_4, 800.f, 800.f, 0.1f, 100.f)));
        prgrm->setUniform("viewMatrix",*(mat4*)glm::value_ptr(glm::lookAt(
                                                                               glm::vec3(2,2,2), //Cam Position
                                                                               glm::vec3(0,0,0),  //Look at point
                                                                               glm::vec3(0.0f, 1.0f, 0.0f)))); //Up)));
        prgrm->setUniform("modelMatrix",*(mat4*)&triTrans);
        glBindFramebuffer(GL_FRAMEBUFFER, BackgroundBuffer);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, BackgroundTexture, 0);
        

        glClearColor(0.45, 0.50, 0.6, 1);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        triangle.bind();
        glDrawElements(GL_TRIANGLES, triangle.getCountForIndex("tri"), GL_UNSIGNED_INT, (void*)triangle.getStartForIndex("tri"));


        
        texprgm->use();
        
        texprgm->setUniform("time", &time, 1);
        
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0,0,fbWidth,fbHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        quad.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, BackSideTexture);
        texprgm->setUniform("BackMap", 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, FrontSideTexture);
        texprgm->setUniform("FrontMap", 1);
        
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, BackgroundTexture);
        texprgm->setUniform("Background", 2);
        
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_3D, VolumeTexture);
        texprgm->setUniform("VolumeMap", 3);
        
        glClearColor(0, 0, 0, 1);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, quad.getCountForIndex("quad"), GL_UNSIGNED_INT, (void*)quad.getStartForIndex("quad"));
        
        
        
        
        
        
        window->swapBuffers();
        window->checkEvents();
        
    }
    
    delete window;
    
    return 0;
}