#include <BufferObject.h>

void Buffer::genObject()
{
    glGenBuffers(1, &*m_object);
}
void Buffer::deleteObject()
{
    glDeleteBuffers(1, &*m_object);
}

void Buffer::bind()
{
    glBindBuffer(m_target, *m_object);
}

void Buffer::setTarget(GLenum target)
{
    m_target = target;
}
