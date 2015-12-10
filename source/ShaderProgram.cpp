#include "ShaderProgram.h"

void ShaderProgram::addShader(Shader & shader)
{
    m_shaders.push_back(shader);
}

void ShaderProgram::use()
{
    glUseProgram(*getObject());
}

void ShaderProgram::link()
{
    
    *m_object = glCreateProgram();
    
    
    for (std::list<Shader>::iterator shader_i=m_shaders.begin(); shader_i != m_shaders.end(); ++shader_i)
        glAttachShader(*getObject(), *(*shader_i)->getObject());
    
    glLinkProgram(*getObject() );
}

ContextPtr ShaderProgram::getObject()
{
    return m_object;
}