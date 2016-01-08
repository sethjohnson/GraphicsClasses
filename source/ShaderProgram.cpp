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

void ShaderProgram::setUniform(const std::string& name, float* values, int numValues)
{
    GLint loc = glGetUniformLocation(*m_object, name.c_str());
    if (loc != -1)
    {
        glUniform1fv(loc, numValues,values);
    }
    
}

void ShaderProgram::setUniform(const std::string& name, vec3* values, int numValues)
{
    GLint loc = glGetUniformLocation(*m_object, name.c_str());
    if (loc != -1)
    {
        glUniform3fv(loc, numValues, (float*)values);
    }
    
}

void ShaderProgram::setUniform(const std::string& name, mat4 matrix)
{
    GLint loc = glGetUniformLocation(*m_object, name.c_str());
    if (loc != -1)
    {
        glUniformMatrix4fv(loc, 1, false, (float*)&matrix);
    }
    
}

void ShaderProgram::setUniform(const std::string& name, GLuint id)
{
    GLint loc = glGetUniformLocation(*m_object, name.c_str());
    if (loc != -1)
    {
        glUniform1i(loc, id);
    }
    
}