//
//  GeometryArray.cpp
//  ExampleVR
//
//  Created by Seth Johnson on 7/29/15.
//
//

#include "GeometryArray.h"
#include <iostream>


using std::vector;
using std::map;



void GeometryArray::listContents()
{
    for(auto const &e : m_attributes) {
        // ent1.first is the first key
        std::cout << "Attribute " <<e.first << std::endl;
    }
}


void GeometryArray::loadAttributes()
{

    
    
    glBindVertexArray(*m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER,  *m_VBO);
    
    size_t byte_size = 0;
    size_t total_bytes = 0;
    
    for(auto const &e : m_attributes) {
        byte_size = e.second->getElementSize()*e.second->getElementCount();
        total_bytes += byte_size;
        
    }

    glBufferData(GL_ARRAY_BUFFER,
                 total_bytes,
                 NULL, GL_STATIC_DRAW);
    
    byte_size = 0;
    total_bytes = 0;
    for(auto const &e : m_attributes) {
        
        AttribType type = e.first;
        RawData * data = e.second;
        
        byte_size = data->getElementSize() * data->getElementCount();
        glVertexAttribPointer(type, data->getElementDimension(), GL_FLOAT, GL_FALSE,data->getElementSize(), (void*) total_bytes);
        glEnableVertexAttribArray(type);
        
        glBufferSubData(GL_ARRAY_BUFFER,
                        total_bytes,
                        byte_size,
                        data->getData().get());
        
        total_bytes += byte_size;
        
    }
    
    
    

}
void GeometryArray::loadIndices()
{
    glBindVertexArray(*m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  *m_IBO);
    
    size_t byte_size = 0;
    size_t total_bytes = 0;
    size_t total_elements = 0;
    for(auto const &e : m_indices) {
        byte_size = e.second->getElementSize()*e.second->getElementCount();
        total_bytes += byte_size;
    }
    


    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 total_bytes,
                 NULL, GL_STATIC_DRAW);

    byte_size = 0;
    total_bytes = 0;
    for(auto const &e : m_indices) {
        byte_size = e.second->getElementSize()*e.second->getElementCount();
        
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                        total_bytes,
                        byte_size,
                        e.second->getData().get());
        
        m_indexBounds[e.first] = std::make_pair(total_elements, e.second->getElementCount());
        total_bytes += byte_size;
        total_elements += e.second->getElementCount();
    }
}

void GeometryArray::addIndex(std::vector<GLuint> & array, std::string indexName)
{
    GLuint* dataCopy = new GLuint[array.size()];
    std::copy ( array.data(), array.data()+array.size(), dataCopy );
    
    m_indices[indexName] = new AttributeArray((void*)(dataCopy), (size_t)sizeof(size_t), (size_t)array.size(), 1, TYPE_INT);
    
}

size_t GeometryArray::getStartForIndex(std::string indexName)
{
    return m_indexBounds[indexName].first;
}
size_t GeometryArray::getCountForIndex(std::string indexName)
{
    return m_indexBounds[indexName].second;
}



void GeometryArray::generateObjects()
{

    
    glGenVertexArrays(1, m_VAO.get());
    glGenBuffers(1, m_VBO.get());
    glGenBuffers(1, m_IBO.get());

  }

void GeometryArray::bind()
{
    glBindVertexArray(*m_VAO);

}
