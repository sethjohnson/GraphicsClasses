#include <ContextObject.h>

const ContextPtr ContextObject::getObject() const
{
    return m_object;
}

ContextObject::~ContextObject()
{
    m_object.reset();
}

ContextObject::ContextObject()
{
    m_object.reset(new (GLuint)(0));
}