#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <vector>
#include <string>
class ShaderType;

typedef std::shared_ptr<GLuint> ContextPtr;
typedef std::shared_ptr<ShaderType> Shader;

class ShaderType {
    std::string m_content;
    static std::string readFile(std::string fileName);
    static ContextPtr compileShader(std::string shaderText, GLenum shaderType);
    GLenum m_shaderType;
    ContextPtr m_object;
    ShaderType() : m_object(new GLuint(0)) {}
public:
    
    static Shader CreateShaderFromText(std::string contents, GLenum shaderType);
    static Shader CreateShaderFromFile(std::string path, GLenum shaderType);

    const std::string getContents();
    void compile();
    const ContextPtr getObject();
};

#endif