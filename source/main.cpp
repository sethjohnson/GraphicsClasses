#define GLM_SWIZZLE

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <stdio.h>
#include "Shader.h"
#include "ShaderProgram.h"
#include "GeometryArray.h"
#include "WindowManager_GLFW.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Mesh.h>
#include <array>
#include "dcmtk/dcmimgle/dcmimage.h"

// assimp include files. These three are usually needed.
//#include <assimp/Importer.hpp>	//OO version Header!
//#include <assimp/PostProcess.h>
//#include <assimp/Scene.h>

//GeometryArray triangle;
Mesh * quad;
Mesh * cube;
Mesh * shape;
Mesh * pile;

int g_key;

static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) // checks to see if the escape key was pressed
        glfwSetWindowShouldClose(window, GL_TRUE);  // closes the
    g_key = key;
    
}
double mouse_x, mouse_y;
int mouse_button = 0;
static void
cursorpos_callback(GLFWwindow* window, double x, double y) {
    mouse_x = x;
    mouse_y = y;
}

static void
mousebutton_callback(GLFWwindow* window, int button, int action, int mods) {
    mouse_button = action;
}

GLuint
setupTexture(GLubyte* texels, GLubyte* scalarTexels, bool color, int imgWidth, int imgHeight, int numImgs)
{
    GLuint _textureID;
    if(color) {
        glEnable(GL_TEXTURE_3D);
        glGenTextures(1, &_textureID);
        glBindTexture(GL_TEXTURE_3D, _textureID);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, imgWidth+2, imgHeight+2, numImgs+2, 1, GL_RGBA, GL_UNSIGNED_BYTE, texels);
        glBindTexture(GL_TEXTURE_3D, NULL);
        glDisable(GL_TEXTURE_3D);
    }
    else {
        glEnable(GL_TEXTURE_3D);
        glGenTextures(1, &_textureID);
        glBindTexture(GL_TEXTURE_3D, _textureID);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE_ALPHA, imgWidth+2, imgHeight+2, numImgs+2, 1, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, texels);
        glBindTexture(GL_TEXTURE_3D, NULL);
        glDisable(GL_TEXTURE_3D);
    }
    
    return _textureID;
}


void
loadDicoms(std::vector<std::string> files, GLubyte * data, glm::dvec3 & dimensions)
{
//    _loadedFilePaths = files;
//    _loadedFromDicom = true;
//    _loadedWithColor = false;
    int numImgs = files.size();
    int curImageInd = 0;
    
    GLubyte *pixelData;
    DicomImage *image = new DicomImage(files[0].c_str());
    if (image != NULL) {
        if(image->getStatus() == EIS_Normal) {
            if(image->isMonochrome()) {
                image->setMinMaxWindow();
                pixelData = (Uint8 *)(image->getOutputData(8 /* bits */));
            }
        }
        else {
            std::cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << std::endl;
        }
    }
    
    int width = image->getWidth();
    int height = image->getHeight();
    delete image;
    
    int volX = width+2;
    int volZ = height+2;
    int volY = numImgs+2;
    
    GLubyte *texels = (GLubyte*)malloc((volX)*(volZ)*(volY)*2);
    GLubyte *scalarTexels = (GLubyte*)malloc((width)*(height)*(numImgs));
    GLubyte col;
    
    //clear the buffer to begin with
    for(int i = 0; i < (volX)*(volZ)*(volY)*2; i++) {
        texels[i] = 0;
    }
    
    for(int j = 0; j < volX; j++) {
        for(int k = 0; k < volZ; k++) {
            int index = ((0*volZ+k) * volX + j) * 2;
            texels[index] = 0;
            texels[index+1] = 0;
            
        }
    }
    for(int j = 0; j < volX; j++) {
        for(int k = 0; k < volZ; k++) {
            int index = (((volY-1)*volZ+k) * volX + j) * 2;
            texels[index] = 0;
            texels[index+1] = 0;
            
        }
    }
    
    
    for(int i = 1; i < volY-1; i++) {
        image = new DicomImage(files[curImageInd].c_str());
        if (image != NULL) {
            if (image->getStatus() == EIS_Normal) {
                if (image->isMonochrome()) {
                    image->setMinMaxWindow();
                    pixelData = (Uint8 *)(image->getOutputData(8 /* bits */));
                }
            }
            else {
                std::cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << std::endl;
            }
        }
        for(int j = 0; j < volX; j++) {
            for(int k = 0; k < volZ; k++) {
                int index = (((volY-(volY-i))*volZ+k) * volX + j) * 2;
                if(j == 0 || j == (volX-1) || k == 0 || k == (volZ-1)) {
                    texels[index] = 0;
                    texels[index+1] = 0;
                }
                else {
                    Uint8 col = pixelData[((height-1)-(k-1))*width + (j-1)];
                    texels[index] = col;
                    texels[index+1] = 255;
                    
                    int scalarIndex = (((((height-1)-(k-1)))*numImgs+(i-1)) * width + (j-1));
                    scalarTexels[scalarIndex] = col;
                }
            }
        }
        curImageInd++;
        delete image;
    }
    
    dimensions.x = width;
    dimensions.z = numImgs;
    dimensions.y = height;
    setupTexture(texels, scalarTexels, false, width, height, numImgs); 
    free(texels);
}



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
    

    
    
    pile = Mesh::CreateMeshFromFile("/Users/sethjohnson/pile.obj");
    
    shape = Mesh::CreateMeshFromFile("/Users/sethjohnson/Desktop/cube.obj");
    cube = Mesh::CreateMeshFromFile("/Users/sethjohnson/Desktop/cube.obj");
    quad = Mesh::CreateXYUnitQuad();
    
    std::map<std::string, Shader> shaders;
    
    shaders["BasicVertexShader.vs"] = ShaderType::CreateShaderFromFile("share/shaders/BasicVertexShader.vs", GL_VERTEX_SHADER);
    shaders["BasicVertexShader.fs"] = ShaderType::CreateShaderFromFile("share/shaders/BasicFragmentShader.fs", GL_FRAGMENT_SHADER);
    shaders["TextureQuad.vs"] = ShaderType::CreateShaderFromFile("share/shaders/TextureQuad.vs", GL_VERTEX_SHADER);
    shaders["TextureQuad.fs"] = ShaderType::CreateShaderFromFile("share/shaders/TextureQuad.fs", GL_FRAGMENT_SHADER);
    shaders["Volume.fs"] = ShaderType::CreateShaderFromFile("share/shaders/Volume.fs", GL_FRAGMENT_SHADER);
    shaders["Blend.fs"] = ShaderType::CreateShaderFromFile("share/shaders/Blend2.fs", GL_FRAGMENT_SHADER);

    shaders["Mesh.vs"] = ShaderType::CreateShaderFromFile("share/shaders/MeshVertexShader.vs", GL_VERTEX_SHADER);
    shaders["Mesh.fs"] = ShaderType::CreateShaderFromFile("share/shaders/MeshFragmentShader.fs", GL_FRAGMENT_SHADER);
    
    shaders["XYZMap.vs"] = ShaderType::CreateShaderFromFile("share/shaders/MeshXYZMap.vs", GL_VERTEX_SHADER);
    shaders["XYZMap.fs"] = ShaderType::CreateShaderFromFile("share/shaders/MeshXYZMap.fs", GL_FRAGMENT_SHADER);
    
    
    for (auto &s : shaders)
        s.second->compile();
    
    
    std::map<std::string, ShaderProgram*> shaderPrograms;
    
    shaderPrograms["BasicVertexShader"] = new ShaderProgram();
    shaderPrograms["BasicVertexShader"]->addShader(shaders["BasicVertexShader.vs"]);
    shaderPrograms["BasicVertexShader"]->addShader(shaders["BasicVertexShader.fs"]);
    
    shaderPrograms["TexureQuad"] = new ShaderProgram();
    shaderPrograms["TexureQuad"]->addShader(shaders["TextureQuad.vs"]);
    shaderPrograms["TexureQuad"]->addShader(shaders["TextureQuad.fs"]);
    
    shaderPrograms["XYZMap"] = new ShaderProgram();
    shaderPrograms["XYZMap"]->addShader(shaders["XYZMap.vs"]);
    shaderPrograms["XYZMap"]->addShader(shaders["XYZMap.fs"]);
    
    shaderPrograms["Volume"] = new ShaderProgram();
    shaderPrograms["Volume"]->addShader(shaders["TextureQuad.vs"]);
    shaderPrograms["Volume"]->addShader(shaders["Volume.fs"]);
    
    shaderPrograms["Composite"] = new ShaderProgram();
    shaderPrograms["Composite"]->addShader(shaders["TextureQuad.vs"]);
    shaderPrograms["Composite"]->addShader(shaders["Blend.fs"]);
    
    
    for (auto &s : shaderPrograms)
        s.second->link();
    
    
    GLint dims[4] = {0};
    glGetIntegerv(GL_VIEWPORT, dims);
    GLint fbWidth = dims[2];
    GLint fbHeight = dims[3];
    
    GLint renderWidth = fbWidth*.75;
    GLint renderHeight = fbHeight*.75;
    
    
    std::map<std::string, GLuint> buffers;
    std::map<std::string, GLuint> buffertextures;

    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, renderWidth, renderHeight);
    
    
    buffers["BackSide"] = 0;
    
    buffers["FrontSide"] = 0;
    
    buffers["Background"] = 0;

    buffers["Volume"] = 0;
    
    buffers["Composite"] = 0;
    
    for (auto &b : buffers)
    {
        glGenFramebuffers(1, &b.second);
        glBindFramebuffer(GL_FRAMEBUFFER, b.second);
        // The texture we're going to render to
        buffertextures[b.first];
        glGenTextures(1, &buffertextures[b.first]);
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, buffertextures[b.first]);
        
        // Give an empty image to OpenGL ( the last "0" )
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA32F, renderWidth, renderHeight, 0,GL_RGBA, GL_FLOAT, 0);
        
        // Poor filtering. Needed !
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // The depth buffer
        //    GLuint depthrenderbuffer;
        
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
        

    }
    
    
    
    const int dim = 256;
    GLfloat * volume = new float[dim*dim*dim];
    int num = dim*dim*dim;
    int dim2D = dim*dim;
    int half_dim = dim/2.0;
    for (int i = 0; i < num; i++) {
        int layer = i/dim2D;
        int layerCoord = i%dim2D;
        int column = layerCoord/dim;
        int row = layerCoord%dim;

        int c = column/(dim/8.0);
        int r = row/(dim/8.0);
        int l = layer/(dim/8.0);
        
        bool fill = ((c%2) ^ (r%2)) ^ (l%2);
        int item = c + r*8 + l*8*8;
        
        volume[i] = item*fill;
    }
    

    
    std::streampos size;
    char * memblock;
    
    std::ifstream file ("/Users/sethjohnson/Documents/visualization-by-sketching/data/BRAIN/live/T1_post.ieee", std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();
        
        std::cout << "the entire file content is in memory";
        
//        delete[] memblock;
    }
    else std::cout << "Unable to open file";
    
    GLuint VolumeTexture;
    glGenTextures(1, &VolumeTexture);
    glBindTexture(GL_TEXTURE_3D, VolumeTexture);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, dim, dim, dim, 0, GL_RED, GL_FLOAT, memblock);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
    
    
    
    glBindFramebuffer(GL_FRAMEBUFFER, buffers["BackSide"]);
    
    
    
    
    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffertextures["BackSide"], 0);
    
    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;
    
    
    glfwSetKeyCallback(((WindowManagerGLFW*)window)->getWindow(), key_callback);
    glfwSetCursorPosCallback(((WindowManagerGLFW*)window)->getWindow(), cursorpos_callback);
    glfwSetMouseButtonCallback(((WindowManagerGLFW*)window)->getWindow(), mousebutton_callback);

    glClearColor(0.45, 0.50, 0.6, 0);
    
    float time = 0;
    window->checkEvents();

    double last_x = mouse_x;
    double last_y = mouse_y;
    double delta_x, delta_y;
    double cam_phi = 45;
    double cam_theta = 45;
    
    while (!window->shouldClose()) {
        time+=0.1;
        delta_x = mouse_x-last_x;
        delta_y = mouse_y-last_y;
        last_x = mouse_x;
        last_y = mouse_y;
        
        if (mouse_button) {
            cam_phi += delta_y*0.1;
            cam_theta += delta_x*-0.1;
        }

        
        cam_phi = glm::clamp(cam_phi, (double)-(90-0.001), (double)(90-0.001));
        cam_theta = glm::mod(cam_theta, (double)360);
        
        glm::mat4 orientation = glm::mat4(1);
        orientation = glm::rotate(orientation, glm::radians((float)cam_theta), glm::vec3(0,1,0));
        orientation = glm::rotate(orientation, glm::radians((float)cam_phi), glm::vec3(0,0,1));
        glm::vec4 camera_pos(1,0,0,1);
        camera_pos = orientation*camera_pos;

        
        // wipe the drawing surface clear
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        
        glm::mat4 volumeTrans = glm::translate( glm::vec3(0.0f,0.0f,0.0f));
        volumeTrans = glm::rotate(volumeTrans, (float)M_PI, glm::vec3(0,0,1));
        //volumeTrans = glm::translate(volumeTrans, glm::vec3(0,0,0));
        
        glm::mat4 volumeTransInv = glm::inverse(volumeTrans);
        

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        shaderPrograms["XYZMap"]->use();
        shaderPrograms["XYZMap"]->setUniform("projectionMatrix",*(mat4*)glm::value_ptr(glm::perspectiveFov((float)M_PI_4, 800.f, 800.f, 0.1f, 100.f)));
        shaderPrograms["XYZMap"]->setUniform("viewMatrix",*(mat4*)glm::value_ptr(glm::lookAt(
                                                                               camera_pos.xyz(), //Cam Position
                                                                               glm::vec3(0,0,0),  //Look at point
                                                                               glm::vec3(0.0f, 1.0f, 0.0f)))); //Up)));
        shaderPrograms["XYZMap"]->setUniform("modelMatrix",*(mat4*)&volumeTrans);
        shaderPrograms["XYZMap"]->setUniform("volumeModelMatrix",*(mat4*)& volumeTransInv);
        
        // Render to our framebuffer
        glViewport(0,0,renderWidth,renderHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffertextures["BackSide"], 0);
        
        glBindFramebuffer(GL_FRAMEBUFFER, buffers["BackSide"]);
        glClearColor(0, 0, 0, 0);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
        
        cube->draw();
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        
        glm::mat4 triTrans = glm::translate( glm::vec3(0.0f,0.0f,0.0f));
        triTrans = glm::scale(triTrans, glm::vec3(0.2,0.2,0.2));
        triTrans = glm::rotate(triTrans,4.0f, glm::vec3(1,1,1));
        triTrans = glm::translate(triTrans, glm::vec3(0.3f,0.0f,0.0f));
        
        
        shaderPrograms["XYZMap"]->setUniform("modelMatrix",*(mat4*)glm::value_ptr(triTrans));
        shape->draw();
        


        
        glBindFramebuffer(GL_FRAMEBUFFER, buffers["FrontSide"]);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffertextures["FrontSide"], 0);
        
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
        
        
        
        shaderPrograms["XYZMap"]->setUniform("modelMatrix",*(mat4*)&volumeTrans);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);        glClearColor(0, 0, 0, 0);

        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube->draw();
        
        
        glDisable(GL_CULL_FACE);
        shaderPrograms["BasicVertexShader"]->use();
        shaderPrograms["BasicVertexShader"]->setUniform("projectionMatrix",*(mat4*)glm::value_ptr(glm::perspectiveFov((float)M_PI_4, 800.f, 800.f, 0.1f, 100.f)));
        shaderPrograms["BasicVertexShader"]->setUniform("viewMatrix",*(mat4*)glm::value_ptr(glm::lookAt(
                                                                          camera_pos.xyz(), //Cam Position
                                                                          glm::vec3(0,0,0),  //Look at point
                                                                          glm::vec3(0.0f, 1.0f, 0.0f)))); //Up)));
        shaderPrograms["BasicVertexShader"]->setUniform("modelMatrix",*(mat4*)&triTrans);
        glBindFramebuffer(GL_FRAMEBUFFER, buffers["Background"]);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffertextures["Background"], 0);
        

        glClearColor(0.45, 0.50, 0.6, 1);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shape->draw();



        
        shaderPrograms["Volume"]->use();
  
        glBindFramebuffer(GL_FRAMEBUFFER, buffers["Volume"]);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffertextures["Volume"], 0);

        glViewport(0,0,renderWidth,renderHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, buffertextures["BackSide"]);
        shaderPrograms["Volume"]->setUniform("BackMap", 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, buffertextures["FrontSide"]);
        shaderPrograms["Volume"]->setUniform("FrontMap", 1);
        
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_3D, VolumeTexture);
        shaderPrograms["Volume"]->setUniform("VolumeMap", 2);
        
        glClearColor(0, 0, 0, 1);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        quad->draw();
        
        
        
        shaderPrograms["Composite"]->use();
        
        glBindFramebuffer(GL_FRAMEBUFFER, buffers["Composite"]);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffertextures["Composite"], 0);
        
        glViewport(0,0,renderWidth,renderHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, buffertextures["Background"]);
        shaderPrograms["Composite"]->setUniform("texBackground", 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, buffertextures["Volume"]);
        shaderPrograms["Composite"]->setUniform("texForeground", 1);

        glClearColor(1, 0, 0, 1);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        quad->draw();


        

        
        
        
        shaderPrograms["TexureQuad"]->use();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0,0,fbWidth,fbHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        glActiveTexture(GL_TEXTURE0);
        switch (g_key) {
            case GLFW_KEY_1:
                glBindTexture(GL_TEXTURE_2D, buffertextures["BackSide"]);

                break;
            case GLFW_KEY_2:
                glBindTexture(GL_TEXTURE_2D, buffertextures["FrontSide"]);
                break;
            case GLFW_KEY_3:
                glBindTexture(GL_TEXTURE_2D, buffertextures["Volume"]);

                break;
            case GLFW_KEY_4:
                glBindTexture(GL_TEXTURE_2D, buffertextures["Background"]);

                break;
            case GLFW_KEY_5:
                glBindTexture(GL_TEXTURE_2D, buffertextures["Composite"]);
                break;
                
            default:
                glBindTexture(GL_TEXTURE_2D, buffertextures["Composite"]);
                break;
        }
        shaderPrograms["TexureQuad"]->setUniform("tex", 0);
        glClearColor(1, 0, 0, 1);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        quad->draw();

        
        window->swapBuffers();
        window->checkEvents();
        
    }
    
    delete window;
    
    return 0;
}