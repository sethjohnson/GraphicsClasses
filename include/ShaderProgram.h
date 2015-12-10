#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <map>
#include <GL/glew.h>
#include "Shader.h"
#include <list>
#include "Shader.h"

class ShaderProgram
{
public:
    ContextPtr getObject();
    void addShader(Shader & shader);
    void link();
    void use();
    ShaderProgram() : m_object(new GLuint(0)) {}

protected:
    ContextPtr m_object;
    std::list<Shader> m_shaders;
    
};

#endif