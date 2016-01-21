#include <ArrayObject.h>

void Array::genObject()
{
    glGenVertexArrays(1, &*m_object);

}
void Array::deleteObject()
{
    glDeleteVertexArrays(1, &*m_object);
}

void Array::bind()
{
    glBindVertexArray(*m_object);
}