//
//  GeometryArray.h
//  ExampleVR
//
//  Created by Seth Johnson on 7/29/15.
//
//

#ifndef __ExampleVR__GeometryArray__
#define __ExampleVR__GeometryArray__

#include <map>
#include <vector>
#include "GL/glew.h"
#include "AttributeArray.h"
#include <utility>


typedef std::shared_ptr<GLuint> ContextPtr;


typedef enum  {
    ATTRIB_INDEX = -1,
    ATTRIB_COLOR = 1,
    ATTRIB_POSITION = 0,
    ATTRIB_NORMAL = 2,
    ATTRIB_TEXCOORD = 3,
    ATTRIB_TANGENT = 4,
    ATTRIB_MATERIAL = 5,
    
} AttribType;


class GeometryArray {
    std::map<std::string, AttributeArray*> m_indices;
    std::map<std::string, std::pair<size_t, size_t>> m_indexBounds;
    
    ContextPtr m_VBO;
    ContextPtr m_VAO;
    ContextPtr m_IBO;
    

public:
    std::map<AttribType, AttributeArray*> m_attributes;

    GeometryArray() : m_VBO(new GLuint(0)), m_VAO(new GLuint(0)), m_IBO(new GLuint(0)){}
    template<class T> void addAttribute(std::vector<T>&, AttribType, size_t, DataType);
    void addIndex(std::vector<GLuint> &, std::string indexName);
    void listContents();
    size_t getStartForIndex(std::string indexName);
    size_t getCountForIndex(std::string indexName);
    void loadAttributes();
    void loadIndices();

    void generateObjects();
    
    void bind();
    

};


template<class T> void GeometryArray::addAttribute(std::vector<T> & array, AttribType attribType, size_t dimension, DataType GLType)
{
    T* dataCopy = new T[array.size()];
    std::copy ( array.data(), array.data()+array.size(), dataCopy );
    m_attributes[attribType] = new AttributeArray((void*)(dataCopy), (size_t)sizeof(T), (size_t)array.size(), dimension, GLType);
    
}

#endif /* defined(__ExampleVR__GeometryArray__) */
