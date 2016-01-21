#ifndef BUFFER_H
#define BUFFER_H

#include <ContextObject.h>

class Buffer : public ContextObject {
public:
    virtual void genObject();
    virtual void deleteObject();
    virtual void bind();
    void setTarget(GLenum target);
protected:
    GLenum m_target;
};

#endif