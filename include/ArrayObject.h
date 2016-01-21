#ifndef ARRAY_H
#define ARRAY_H

#include <ContextObject.h>

class Array : public ContextObject {
public:
    virtual void genObject();
    virtual void deleteObject();
    virtual void bind();
    
};

#endif