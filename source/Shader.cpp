#include <Shader.h>
#include <fstream>
#include <iostream>
#include <sstream>

using std::ifstream;
using std::ios;


Shader ShaderType::CreateShaderFromFile(std::string path, GLenum shaderType)
{
    return CreateShaderFromText(readFile(path), shaderType);
}

Shader ShaderType::CreateShaderFromText(std::string contents, GLenum shaderType)
{
    Shader s = NULL;
    if (shaderType == GL_FRAGMENT_SHADER || shaderType == GL_VERTEX_SHADER || shaderType == GL_GEOMETRY_SHADER)
    {
        s.reset( new ShaderType());
        s->m_content = contents;
        s->m_shaderType = shaderType;
    }

    return s;
}




std::string ShaderType::readFile(std::string fileName)
{
    std::stringstream ss;
    ifstream inFile(fileName, ios::in);
    std::string contents = "";
    
    if (!inFile)
    {
        std::cerr << "Failed to load " << fileName << std::endl;
    }
    else
    {
        ss << inFile.rdbuf();
        inFile.close();
        contents = ss.str();
    }
    return contents;
}

ContextPtr ShaderType::compileShader(std::string shaderText, GLenum shaderType)
{
    ContextPtr shaderObject(new GLuint(0));
    *shaderObject = glCreateShader(shaderType);
    const char *text = shaderText.c_str();
    
    glShaderSource(*shaderObject, 1, &text, NULL);  //Read source
    glCompileShader(*shaderObject); // Compile shaders
    
    GLint  compiled;
    glGetShaderiv(*shaderObject, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printf("Shader failed to compile:\n");
        if (1) {
            GLint logMaxSize, logLength;
            glGetShaderiv(*shaderObject, GL_INFO_LOG_LENGTH, &logMaxSize);
            printf("printing error message of %d bytes\n", logMaxSize);
            char* logMsg = new char[logMaxSize];
            glGetShaderInfoLog(*shaderObject, logMaxSize, &logLength, logMsg);
            printf("%d bytes retrieved\n", logLength);
            printf("=========\n%s\n=========\n", shaderText.c_str());
            printf("error message: %s\n", logMsg);
            delete[] logMsg;
        }
        shaderObject.reset();
        
    }
    
    return shaderObject;
}

const std::string ShaderType::getContents()
{
    return std::string(m_content);
}

void ShaderType::compile()
{
    ContextPtr shaderIndex = compileShader(getContents(), m_shaderType);
    m_object = shaderIndex;
}


const ContextPtr ShaderType::getObject()
{
    return m_object;
}

