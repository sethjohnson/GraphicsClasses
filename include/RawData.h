#ifndef RAWDATA_H
#define RAWDATA_H

#include <iostream>



class RawData {
protected:
    std::shared_ptr<char> m_data;
    size_t m_elementSize;
    size_t m_elementCount;
    size_t m_elementDimension;
    unsigned int m_elementType;
public:
    RawData();
    RawData(const void * data, size_t elementSize, size_t elementCount, size_t elementDimension, unsigned int elementType);
    
    ~RawData();
    virtual void setData(const void* value);
    virtual size_t getElementSize();
    virtual size_t getElementCount();
    virtual size_t getElementDimension();
    virtual unsigned int getElementType();
    virtual std::shared_ptr<char> getData();
    
};
#endif