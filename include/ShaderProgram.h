#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <map>
#include <GL/glew.h>
#include "Shader.h"
#include <list>
#include <ContextObject.h>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;



class ShaderProgram : public ContextObject
{
public:
    ContextPtr getObject();
    void addShader(Shader & shader);
    void link();
    void use();
    ShaderProgram() : m_object(new GLuint(0)) {}
    void release();
    void setUniform(const std::string& name, mat4 matrix);
    void setUniform(const std::string& name, vec3 vector);
    void setUniform(const std::string& name, GLuint id);
    void setUniform(const std::string& name, float* values, int numValues);
    void setUniform(const std::string& name, vec3* values, int numValues);
    void setUniform(const std::string& name, vec4* values, int numValues);
protected:
    ContextPtr m_object;
    std::list<Shader> m_shaders;
    virtual void deleteObject() {}
    virtual void genObject() {}
};

#endif