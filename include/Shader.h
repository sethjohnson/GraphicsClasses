#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <ContextObject.h>

class ShaderType;

typedef std::shared_ptr<ShaderType> Shader;

class ShaderType : public ContextObject {

public:
    static Shader CreateShaderFromText(std::string contents, GLenum shaderType);
    static Shader CreateShaderFromFile(std::string path, GLenum shaderType);
    virtual void genObject();

    virtual void deleteObject();
    
    const std::string getContents();
    void compile();
    
private:
    std::string m_content;
    static std::string readFile(std::string fileName);
    static ContextPtr compileShader(std::string shaderText, GLenum shaderType);
    GLenum m_shaderType;
    ShaderType() : ContextObject(){}
    
};

#endif