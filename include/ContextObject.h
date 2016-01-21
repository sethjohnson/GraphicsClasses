#ifndef CONTEXT_OBJECT
#define CONTEXT_OBJECT
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <iostream>

typedef std::shared_ptr<GLuint> ContextPtr;


class ContextObject
{
protected:
    ContextPtr m_object;
    
public:
    ContextObject();
    virtual void genObject() = 0;
    const ContextPtr getObject() const;
    virtual void deleteObject() = 0;
    virtual ~ContextObject();
    
};

#endif