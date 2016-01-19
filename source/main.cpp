
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <stdio.h>
#include "Shader.h"
#include "ShaderProgram.h"
#include "GeometryArray.h"
#include "WindowManager_GLFW.h"
#include <iostream>
#define GLM_SWIZZLE
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
// assimp include files. These three are usually needed.
//#include <assimp/Importer.hpp>	//OO version Header!
//#include <assimp/PostProcess.h>
//#include <assimp/Scene.h>

GeometryArray triangle;
GeometryArray quad;
GeometryArray cube;
GeometryArray shape;

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

void loadModel(std::string path)
{
//    std::vector<MeshRef> meshes;
    
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
//        return meshes;
    }
    
    std::cout << scene->mNumMeshes << std::endl;
    
    for (int meshNum = 0; meshNum < scene->mNumMeshes; meshNum++)
    {
        aiMesh *mesh = scene->mMeshes[meshNum];
        
        
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        
        std::vector<glm::vec2> texCoords;
        
        bool hasTexCoords = mesh->mTextureCoords[0];
        
        for (int i = 0; i < mesh->mNumVertices; i++) {
            glm::vec3 vert;
            vert.x = mesh->mVertices[i].x;
            vert.y = mesh->mVertices[i].y;
            vert.z = mesh->mVertices[i].z;
            vertices.push_back(vert);
            
            if (mesh->HasNormals())
            {
                glm::vec3 norm;
                norm.x = mesh->mNormals[i].x;
                norm.y = mesh->mNormals[i].y;
                norm.z = mesh->mNormals[i].z;
                normals.push_back(norm);
            }
            
            if (hasTexCoords)
            {
                texCoords.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
            }
        }
        
        std::vector<unsigned int> indices;
        for(int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // Retrieve all indices of the face and store them in the indices vector
            for(int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        
        std::string directory = "";
        int lastFolderIndex = path.find_last_of('/');
        if (lastFolderIndex < 0)
        {
            lastFolderIndex = path.find_last_of('\\');
        }
        
        if (lastFolderIndex >= 0)
        {
            directory = path.substr(0, lastFolderIndex+1);
        }
        
        shape.addIndex(indices, "shape");
        shape.addAttribute(vertices, ATTRIB_POSITION, 3, TYPE_FLOAT);

        if (mesh->HasNormals())
        {
            shape.addAttribute(normals, ATTRIB_NORMAL, 3, TYPE_FLOAT);

        }

        
        if (hasTexCoords)
        {
            shape.addAttribute(texCoords, ATTRIB_TEXCOORD, 2, TYPE_FLOAT);
        }
        
        shape.generateObjects();
        
        shape.bind();
        

        shape.loadAttributes();
        shape.loadIndices();
        
        shape.bind();
//        if (mesh->mMaterialIndex >= 0)
//        {
//            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
//            int texCount = material->GetTextureCount(aiTextureType_DIFFUSE);
//            std::cout << texCount << " count" << std::endl;
//            if (texCount > 0)
//            {
//                aiString str;
//                material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
//                std::cout << std::string(str.C_Str()) << std::endl;
//                finalMesh->setTexture(TextureRef(new SOILTexture(directory + std::string(str.C_Str()))));
//            }
//        }
//        
//        meshes.push_back(finalMesh);
    }

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
    
    /* OTHER STUFF GOES HERE NEXT */
    vec3 triangle_vertices_array[] = {
        {0.0f,  0.5f,  0.0f},
        {0.5f, -0.5f,  0.0f},
        {-0.5f, -0.5f,  0.0f}
    };
    vec3 triangle_color_array[] = {
        {0.5,  0.5,  0.5},
        {0.6,  0.6,  0.6},
        {0.4,  0.4f,  0.4}
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
    
    
    loadModel("/Users/sethjohnson/Desktop/monkey.obj");
    
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
        glm::vec4 camera_pos(1.5,0,0,1);
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
        
        cube.bind();
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffertextures["BackSide"], 0);
        
        glBindFramebuffer(GL_FRAMEBUFFER, buffers["BackSide"]);
        glClearColor(1, 0, 0, 0);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
        
        glDrawElements(GL_TRIANGLES, cube.getCountForIndex("cube"), GL_UNSIGNED_INT, (void*)cube.getStartForIndex("cube"));
        glDisable(GL_CULL_FACE);

        
        glm::mat4 triTrans = glm::translate( glm::vec3(0.0f,0.0f,0.0f));
        triTrans = glm::scale(triTrans, glm::vec3(0.2,0.2,0.2));
        triTrans = glm::rotate(triTrans,4.0f, glm::vec3(1,1,1));
        triTrans = glm::translate(triTrans, glm::vec3(0.3f,0.0f,0.0f));
        
        
        shaderPrograms["XYZMap"]->setUniform("modelMatrix",*(mat4*)glm::value_ptr(triTrans));
        shape.bind();
        //glDrawElements(GL_TRIANGLES, shape.getCountForIndex("shape"), GL_UNSIGNED_INT, (void*)shape.getStartForIndex("shape"));
        

        
        
        glBindFramebuffer(GL_FRAMEBUFFER, buffers["FrontSide"]);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffertextures["FrontSide"], 0);
        
        glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
        
        
        cube.bind();
        
        shaderPrograms["XYZMap"]->setUniform("modelMatrix",*(mat4*)&volumeTrans);
        glCullFace(GL_BACK);
        glClearColor(1, 0, 0, 0);

        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, cube.getCountForIndex("cube"), GL_UNSIGNED_INT, (void*)cube.getStartForIndex("cube"));
        
        
        
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
        shape.bind();
        //glDrawElements(GL_TRIANGLES, shape.getCountForIndex("shape"), GL_UNSIGNED_INT, (void*)shape.getStartForIndex("shape"));


        
        shaderPrograms["Volume"]->use();
  
        glBindFramebuffer(GL_FRAMEBUFFER, buffers["Volume"]);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffertextures["Volume"], 0);

        glViewport(0,0,renderWidth,renderHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        quad.bind();
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

        glDrawElements(GL_TRIANGLES, quad.getCountForIndex("quad"), GL_UNSIGNED_INT, (void*)quad.getStartForIndex("quad"));
        
        
        
        shaderPrograms["Composite"]->use();
        
        glBindFramebuffer(GL_FRAMEBUFFER, buffers["Composite"]);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffertextures["Composite"], 0);
        
        glViewport(0,0,renderWidth,renderHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        quad.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, buffertextures["Background"]);
        shaderPrograms["Composite"]->setUniform("texBackground", 0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, buffertextures["Volume"]);
        shaderPrograms["Composite"]->setUniform("texForeground", 1);

        glClearColor(1, 0, 0, 1);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glDrawElements(GL_TRIANGLES, quad.getCountForIndex("quad"), GL_UNSIGNED_INT, (void*)quad.getStartForIndex("quad"));


        

        
        
        
        shaderPrograms["TexureQuad"]->use();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0,0,fbWidth,fbHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
        quad.bind();
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
        
        glDrawElements(GL_TRIANGLES, quad.getCountForIndex("quad"), GL_UNSIGNED_INT, (void*)quad.getStartForIndex("quad"));

        
        window->swapBuffers();
        window->checkEvents();
        
    }
    
    delete window;
    
    return 0;
}