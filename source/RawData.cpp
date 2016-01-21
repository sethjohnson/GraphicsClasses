#include "RawData.h"
#include <algorithm>    // std::copy


RawData::RawData() : m_data(nullptr)
{
    
}

RawData::RawData(const void * data, size_t elementSize, size_t elementCount, size_t elementDimension, unsigned int elementType) : RawData() {
    
    m_elementSize = (elementSize);
    m_elementCount = (elementCount);
    m_elementDimension = (elementDimension);
    m_elementType = (elementType);
    
    setData(data);
}

size_t RawData::getElementSize()
{
    return m_elementSize;
}

size_t RawData::getElementCount()
{
    return m_elementCount;
}

size_t RawData::getElementDimension()
{
    return m_elementDimension;
}

std::shared_ptr<char> RawData::getData()
{
    return m_data;
}

unsigned int RawData::getElementType()
{
    return m_elementType;
}

RawData::~RawData()
{
    
}

void RawData::setData(const void* value)
{
    if (m_data.get() == nullptr) {
        m_data = std::shared_ptr<char>(new char[m_elementSize*m_elementCount]);
    }
    
    if (value != nullptr) {
        std::copy((char*)value, (char*)value+m_elementSize*m_elementCount, m_data.get());
    }
    
    
}